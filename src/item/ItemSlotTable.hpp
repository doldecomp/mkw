#pragma once

#include <rk_types.h>
#include <egg/core/eggDisposer.hpp>

namespace Item {

struct ItemSlotChanceRow {
    u16 itemChances[19];
};

struct ItemSlotTableHolder {
    u32 count;                  // 0x0
    ItemSlotChanceRow *data;    // 0x4
};

class ItemSlotTable : public EGG::Disposer {
public:
    ItemSlotTable();
    virtual ~ItemSlotTable();

    static ItemSlotTable *createInstance();
    static void destroyInstance();

    void resetLightningTimer();
    void resetBlueShellTimer();
    void resetBlooperTimer();
    void resetPowTimer();
    void updateTimers();
    bool checkSpawnTimer(int itemObjId, int param_3);
    void scaleTable(ItemSlotTableHolder *holder);

    static ItemSlotTable *sInstance;

    ItemSlotTableHolder mPlayerChances;          // 0x10
    ItemSlotTableHolder mCpuChances;             // 0x18
    ItemSlotTableHolder mSpecialChances;         // 0x20
    int *mItemsInWheel;                          // 0x28
    int *mSpecialBoxItemsInWheel;                // 0x2C
    void *mSomethingSpecialChances;              // 0x30
    s32 mLightningTimer;                         // 0x34
    s32 mBlueShellTimer;                         // 0x38
    s32 mBlooperTimer;                           // 0x3C
    s32 mPowTimer;                               // 0x40
    u32 mPlayerCount;                            // 0x44
};

} // namespace Item
