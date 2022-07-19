#pragma once

#include "rfcommon/ListenerDispatcher.hpp"
#include "rfcommon/Vector.hpp"

namespace rfcommon {
    class Session;
    class PlayerState;
}

class StatsListener;

class StatsModel
{
public:
    static const int MAX_FIGHTERS = 2; 

    /*!
     * \brief Resets all of the stats to their default value. Call this
     * once before calling updateStatistics().
     */
    void resetStatistics(const rfcommon::Session* session);

    /*!
     * \brief Incrementally updates all of the statistics from a single
     * frame of data.
     */
    void updateStatistics(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states);

    double totalDamageTaken(int fighterIdx) const 
        { return totalDamageTaken_[fighterIdx]; }

    rfcommon::ListenerDispatcher<StatsListener> dispatcher;

private:
    void updateDamageTaken(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states);
    void updateDamagesAtDeath(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states);
    void updateFirstBlood(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states);
    void updateStageControl(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states);

private:
    double totalDamageTaken_[MAX_FIGHTERS];
    double totalDamageDealt_[MAX_FIGHTERS];
    double oldDamage_[MAX_FIGHTERS];

    int oldStocks_[MAX_FIGHTERS];
    rfcommon::SmallVector<double, 4> damagesAtDeath_[MAX_FIGHTERS];

    int firstBloodFighterIdx_;

    int stageControl_[MAX_FIGHTERS];
    int isInNeutralState_[MAX_FIGHTERS];
};
