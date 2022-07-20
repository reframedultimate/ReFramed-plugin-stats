#pragma once

#include "rfcommon/ListenerDispatcher.hpp"
#include "rfcommon/Vector.hpp"

namespace rfcommon {
    class Session;
    class PlayerState;
}

class StatsCalculatorListener;

class StatsCalculator
{
public:
    static const int MAX_FIGHTERS = 2; 

    StatsCalculator();

    /*!
     * \brief Resets all of the stats to their default value. Call this
     * once before calling updateStatistics().
     */
    void resetStatistics();

    /*!
     * \brief Incrementally updates all of the statistics from a single
     * frame of data.
     */
    void updateStatistics(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states);

    double avgDamagePerOpening(int fighterIdx) const { return 0.0; }
    double avgKillPercent(int fighterIdx) const { return 0.0; }
    double earliestKillPercent(int fighterIdx) const { return 0.0; }
    double latestDeathPercent(int fighterIdx) const { return 0.0; }
    int numNeutralWins(int fighterIdx) const { return 0; }
    double neutralWinPercent(int fighterIdx) const { return 0.0; }
    int numOpeningsPerKill(int fighterIdx) const { return 0; }
    int numStocksTaken(int fighterIdx) const { return 0; }
    double stageControlPercent(int fighterIdx) const { return 0.0; }

    /*!
     * \brief Total amount of damage (percent) a player dealt.
     *
     * In 1v1, this is the total amount of damage the opponent received.
     * This metric currently counts self damage as well (in 1v1) as there is
     * no way to distinguish self damage from projectile damage.
     *
     * With 3 or more players, this statistic doesn't include projectile damage
     * because it's not possible to distinguish self damage from projectile damage.
     */
    double totalDamageDealt(int fighterIdx) const 
        { return totalDamageDealt_[fighterIdx]; }

    /*!
     * \brief Total amount of damage (percent) received.
     */
    double totalDamageTaken(int fighterIdx) const 
        { return totalDamageTaken_[fighterIdx]; }

    rfcommon::ListenerDispatcher<StatsCalculatorListener> dispatcher;

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
