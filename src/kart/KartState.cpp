#include <rk_types.h>
#include <decomp.h>

#include "KartState.hpp"
#include "KartMove.hpp"
#include <system/Competition.hpp>
#include <system/RaceMode.hpp>

//https://decomp.me/scratch/7prZZ
namespace Kart {
extern bool isPlayerOnlineLocal;
extern bool isPlayerOnlineRemote;

MARK_FLOW_CHECK(0x805943b4);
KartState::KartState(KartSettings* settings) {
  using namespace System;

  mAirtime = 0;
  _24 = 0.0f;
  mCannonPointId = 0;
  mStartBoostIdx = 0;
  mUp.setZero();
  mProxy = new KartObjectProxy;

  RaceConfig::Player::Type playerType = RaceConfig::spInstance->mRaceScenario.mPlayers[settings->playerIdx].mPlayerType;
  switch (playerType) {
  case RaceConfig::Player::TYPE_REAL_LOCAL:
    set(KART_FLAG_LOCAL);
    break;
  case RaceConfig::Player::TYPE_CPU:
    set(KART_FLAG_CPU);
    break;
  case RaceConfig::Player::TYPE_GHOST:
    set(KART_FLAG_GHOST);
    break;
  }

  if (isPlayerOnlineLocal) {
    set(KART_FLAG_ONLINE_LOCAL);
  } else if (isPlayerOnlineRemote) {
    set(KART_FLAG_ONLINE_REMOTE);
  }

  KPadController* controller = RaceManager::spInstance->players[settings->playerIdx]->kpadPlayer->mController;
  bool isAuto;
  if (!controller) {
    isAuto = false;
  } else {
    isAuto = controller->mDriftIsAuto;
  }

  if (isAuto) {
    set(KART_FLAG_AUTOMATIC_DRIFT);
  }

  if (RaceConfig::spInstance->mRaceScenario.mSettings.mGameMode == RaceConfig::Settings::GAMEMODE_AWARDS &&
      RaceConfig::spInstance->mRaceScenario.mSettings.mCameraMode == RaceConfig::Settings::CAMERA_MODE_LOSS) {
    set(KART_FLAG_SET_SPEED_ZERO);
    set(KART_FLAG_DEMO_LOSS);
  }
}

void KartState::init() {
  reset();
  resetOob();
}

void KartState::reset() {
  mFlags.field(3) = 0;
  mFlags.field(2) = 0;
  mFlags.field(1) = 0;
  mFlags.field(0) = 0;
  mAirtime = 0;
  _24 = 0.0f;
  mUp.setZero();
  _40.setZero();
  _58 = 0;
  _5c = 0;
  mHwgTimer = 0;
  m_70 = 0;
  mBoostRampType = -1;
  mJumpPadType = -1;
  _84 = 0;
  mStartBoostCharge = 0.0f;
  mStick.y = 0.0f;
  mStick.x = 0.0f;
  _a4 = 0;
  _4c = m_a8 = EGG::Vector3f::zero;
  _a6 = 0;
}

void KartState::resetOob() {
  mWipeState = -1;
  mWipeFrame = -1;
}

extern "C" s16 lbl_1_data_3958[];

void KartState::updateWipe() {
  if (mWipeState == -1) {
    return;
  }

  mWipeRatio = (f32)++mWipeFrame / (f32)lbl_1_data_3958[mWipeState];
  if (1.0f < mWipeRatio) {
    mWipeRatio = 1.0f;
  }
  if (mWipeFrame > lbl_1_data_3958[mWipeState]) {
    mWipeState = -1;
  }
}

void KartState::startWipe(int wipeState) {
  mWipeState = wipeState;
  mWipeFrame = 0;
}

void KartState::resetCollisionFlags() {
  mFlags.field(0) &= ~(
      KART_FLAG_MASK(KART_FLAG_ACCELERATE) |
      KART_FLAG_MASK(KART_FLAG_BRAKE) |
      KART_FLAG_MASK(KART_FLAG_DRIFT_INPUT) |
      KART_FLAG_MASK(KART_FLAG_HOP_START) |
      KART_FLAG_MASK(KART_FLAG_ACCELERATE_START) |
      KART_FLAG_MASK(KART_FLAG_GROUND_START) |
      KART_FLAG_MASK(KART_FLAG_STICK_LEFT) |
      KART_FLAG_MASK(KART_FLAG_WALL_COLLISION_START) |
      KART_FLAG_MASK(KART_FLAG_AIR_START) |
      KART_FLAG_MASK(KART_FLAG_STICK_RIGHT));
  mFlags.field(1) &= ~KART_FLAG_MASK(KART_FLAG_ZIPPER_INVISIBLE_WALL);
  mFlags.field(2) &= ~(
      KART_FLAG_MASK(KART_FLAG_STH_4C) |
      KART_FLAG_MASK(KART_FLAG_DISABLE_Y_SUS_FORCE) |
      KART_FLAG_MASK(KART_FLAG_STH_5E) |
      KART_FLAG_MASK(KART_FLAG_STH_5F));
  mStick.y = 0.0f;
  mStick.x = 0.0f;
}

extern "C" f32 lbl_1_data_3918[];

void KartState::updateStartBoostCharge() {
  const f32* data = lbl_1_data_3918;
  if (on(KART_FLAG_CHARGE_START_BOOST)) {
    f32 diff = data[0] - data[1];
    mStartBoostCharge += data[0] - diff * mStartBoostCharge;
  } else {
    mStartBoostCharge *= data[2];
  }

  if (0.0f > mStartBoostCharge) {
    mStartBoostCharge = 0.0f;
  } else if (1.0f < mStartBoostCharge) {
    mStartBoostCharge = 1.0f;
  }

  mProxy->kartMove()->setStartBoostCharge(10.0f * mStartBoostCharge);
}

struct StartBoostEntry {
  f32 minCharge;
  s16 frames;
  s16 _pad;
};

extern "C" StartBoostEntry lbl_1_data_3928[];
extern "C" s32 RaceInfo_getCountdown(System::RaceManager*);
extern "C" void fn_1_78CFC(void*);
extern "C" void PlayerSub10_applyStartBoost(KartMove*, s16);
extern "C" void fn_1_2D780(System::RaceMode*, s32);
extern "C" void fn_1_8140C(KartObjectProxy*, s32);

void KartState::computeStartBoost() {
  if (RaceInfo_getCountdown(System::RaceManager::spInstance) != 0) {
    return;
  }

  if (!((const KartObjectProxy*)mProxy)->isCpu()) {
    System::CourseId courseId =
        System::RaceConfig::spInstance->mRaceScenario.mSettings.mCourseId;
    if (courseId != System::CHAIN_CHOMP_ROULETTE &&
        courseId != System::FUNKY_STADIUM) {
      if (on(KART_FLAG_ACCELERATE)) {
        if (mStartBoostCharge > lbl_1_data_3928[5].minCharge) {
          mStartBoostIdx = -1;
        } else if (mStartBoostCharge > lbl_1_data_3928[0].minCharge) {
          const StartBoostEntry* entries = lbl_1_data_3928;
          for (u8 idx = 1; idx <= 5; ++idx) {
            const StartBoostEntry* entry = &entries[idx];
            if (mStartBoostCharge > entry[-1].minCharge &&
                mStartBoostCharge <= entry[0].minCharge) {
              mStartBoostIdx = idx;
              GpStats* gpStats = mProxy->kartSettings()->gpStats;
              if (gpStats != nullptr) {
                gpStats->startBoostSuccessful = true;
              }
              break;
            }
          }
        }
      }
    }
  }

  applyStartBoost(mStartBoostIdx);
  mFlags.field(3) &= ~KART_FLAG_MASK(KART_FLAG_CHARGE_START_BOOST);
}

static inline void checkTournamentObjective(
    System::RaceConfig* config, System::RaceManager* manager) {
  if (config->mRaceScenario.mSettings.mGameMode ==
          System::RaceConfig::Settings::GAMEMODE_MISSION_TOURNAMENT &&
      (int)config->mRaceScenario.mCompetitionSettings.objective == 10) {
    static_cast<System::RaceModeCompetition*>(manager->raceMode)
        ->mObjectiveStatus = 2;
  }
}

static inline void checkTournamentObjective(
    System::RaceConfig* config, System::RaceManager* manager, int boost) {
  if (config->mRaceScenario.mSettings.mGameMode ==
          System::RaceConfig::Settings::GAMEMODE_MISSION_TOURNAMENT &&
      (int)config->mRaceScenario.mCompetitionSettings.objective == 10) {
    fn_1_2D780(manager->raceMode, boost);
  }
}

void KartState::applyStartBoost(int startBoostIdx) {
  if (startBoostIdx == -1) {
    KartMove* move = mProxy->kartMove();
    fn_1_78CFC(move->kartBurnout());
    checkTournamentObjective(
        System::RaceConfig::spInstance, System::RaceManager::spInstance);
    if (mProxy->kartState()->on(KART_FLAG_ONLINE_LOCAL)) {
      fn_1_8140C(mProxy, 0x1c);
    }
  } else if (startBoostIdx > 0) {
    KartMove* move = mProxy->kartMove();
    PlayerSub10_applyStartBoost(move, lbl_1_data_3928[startBoostIdx].frames);
    checkTournamentObjective(
        System::RaceConfig::spInstance, System::RaceManager::spInstance,
        startBoostIdx);
    if (mProxy->kartState()->on(KART_FLAG_ONLINE_LOCAL)) {
      fn_1_8140C(mProxy, 0x1b);
      KartNetSender* netSender = mProxy->kartNetSender();
      netSender->mStartBoostIdx = startBoostIdx;
    }
  } else {
    checkTournamentObjective(
        System::RaceConfig::spInstance, System::RaceManager::spInstance);
  }
}

} // namespace Kart
