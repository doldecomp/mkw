#pragma once

#include <rk_types.h>

namespace System {
class RaceManager;

struct RaceMode {
    virtual u32 canEndRace();
    RaceManager* raceManager;
};

struct RaceModeCompetition : public RaceMode {
  u32 _08;
  u32 mObjectiveStatus;
};

class KrtFile;

struct RaceModeGrandPrix: public RaceMode {
    KrtFile* krtFile[1];

    u32 canEndRace();
    RaceModeGrandPrix(RaceManager* raceManager_);
};
}
