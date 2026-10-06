#include "KartSettings.hpp"

#include <system/RaceConfig.hpp>

namespace Kart {

KartSettings::KartSettings(
    s8 playerIdx,
    System::VehicleId vehicle,
    System::CharacterId character,
    u32 isBike,
    KartParam* kartParam,
    void* arg6,
    KartDriverDispParams* kartDriverDispParams,
    KartPartsDispParams* kartPartsDispParams,
    BikePartsDispParams* bikePartsDispParams,
    DriverDispParams* driverDispParams) {
  this->isBike = isBike;
  this->vehicle = vehicle;
  this->character = character;
  this->playerIdx = playerIdx;
  this->kartParam = kartParam;
  this->_18 = arg6;
  this->kartDriverDispParams = kartDriverDispParams;
  this->kartPartsDispParams = kartPartsDispParams;
  this->bikePartsDispParams = bikePartsDispParams;
  this->driverDispParams = driverDispParams;
  this->gpStats = nullptr;
  this->raceStats = nullptr;

  if (System::RaceConfig::spInstance->mRaceScenario.mPlayers[(u8)playerIdx].mPlayerType ==
      System::RaceConfig::Player::TYPE_REAL_LOCAL) {
    this->raceStats = new RaceStats;
    if (System::RaceConfig::spInstance->mRaceScenario.mSettings.mGameMode ==
        System::RaceConfig::Settings::GAMEMODE_GRAND_PRIX) {
      this->gpStats = new GpStats;
    }
  }
}

} // namespace Kart
