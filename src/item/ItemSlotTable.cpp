#include <item/ItemSlotTable.hpp>
#include <system/RaceManager.hpp>

extern "C" u32 RaceInfo_getCountdown(void *);

namespace Item {

ItemSlotTable *ItemSlotTable::sInstance = nullptr;

void ItemSlotTable::resetLightningTimer() {
    mLightningTimer = 1800;
}

void ItemSlotTable::resetBlueShellTimer() {
    mBlueShellTimer = 1800;
}

void ItemSlotTable::resetBlooperTimer() {
    mBlooperTimer = 900;
}

void ItemSlotTable::resetPowTimer() {
    mPowTimer = 1200;
}

bool ItemSlotTable::checkSpawnTimer(int itemObjId, int param_3) {
    s32 threshold = (param_3 != 0) ? 300 : 0;
    switch (itemObjId) {
    case 6:
        return mLightningTimer > threshold;
    case 5:
        return mBlueShellTimer > threshold;
    case 10:
        return mBlooperTimer > threshold;
    case 11:
        return mPowTimer > threshold;
    default:
        return false;
    }
}

extern const u16 sPlayerCountPositionRemap[12][12];

void ItemSlotTable::scaleTable(ItemSlotTableHolder *holder) {
    ItemSlotChanceRow *data = holder->data;
    ItemSlotChanceRow *dest = data;
    for (s32 pos = 0; pos < (s32)mPlayerCount; pos++) {
        u16 remappedPos = sPlayerCountPositionRemap[mPlayerCount - 1][pos];
        ItemSlotChanceRow *src = &data[(u16)(remappedPos - 1)];
        for (int i = 0; i < 19; i++) {
            dest->itemChances[i] = src->itemChances[i];
        }
        dest++;
    }
    for (s32 pos = mPlayerCount; pos < 12; pos++) {
        for (int i = 0; i < 19; i++) {
            dest->itemChances[i] = 0;
        }
        dest++;
    }
}

void ItemSlotTable::updateTimers() {
    if (RaceInfo_getCountdown(System::RaceManager::spInstance) == 0) {
        mLightningTimer = 1800;
        mBlueShellTimer = 1800;
        mBlooperTimer = 900;
        mPowTimer = 1200;
    }

    if (mLightningTimer > 0) {
        mLightningTimer--;
    }
    if (mBlueShellTimer > 0) {
        mBlueShellTimer--;
    }
    if (mBlooperTimer > 0) {
        mBlooperTimer--;
    }
    if (mPowTimer > 0) {
        mPowTimer--;
    }
}

} // namespace Item
