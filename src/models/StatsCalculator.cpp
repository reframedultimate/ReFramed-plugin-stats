#include "stats/models/StatsCalculator.hpp"
#include "stats/listeners/StatsCalculatorListener.hpp"
#include "rfcommon/Session.hpp"
#include "rfcommon/PlayerState.hpp"

// TODO Extract this info from the global mapping info structure, once
// ReFramed's refactoring is done and this is possible.
enum StatusKinds
{
    FIGHTER_STATUS_KIND_LANDING = 22,
    FIGHTER_STATUS_KIND_PASSIVE = 103,
    FIGHTER_STATUS_KIND_PASSIVE_FB = 104,
    FIGHTER_STATUS_KIND_WAIT = 0,
    FIGHTER_STATUS_KIND_GUARD_ON = 27,
    FIGHTER_STATUS_KIND_JUMP_SQUAT = 10,
    FIGHTER_STATUS_KIND_WALK = 1,
    FIGHTER_STATUS_KIND_DASH = 3,
    FIGHTER_STATUS_KIND_REBIRTH = 182,
    FIGHTER_STATUS_KIND_SHIELD_BREAK_FLY = 92
};

// ----------------------------------------------------------------------------
StatsCalculator::StatsCalculator()
{
    StatsCalculator::resetStatistics();
}

// ----------------------------------------------------------------------------
void StatsCalculator::resetStatistics()
{
    for (int i = 0; i != MAX_FIGHTERS; ++i)
    {
        // Reset damage counters
        totalDamageTaken_[i] = 0.0;
        totalDamageDealt_[i] = 0.0;
        oldDamage_[i] = 0.0;

        // Reset stock counters
        oldStocks_[i] = 0;
        damagesAtDeath_[i].clearCompact();

        // Reset first blood
        firstBloodFighterIdx_ = -1;

        // Reset stage control counters
        stageControl_[i] = 0;
        isInNeutralState_[i] = 0;
    }

    dispatcher.dispatch(&StatsCalculatorListener::onStatsUpdated);
}

// ----------------------------------------------------------------------------
void StatsCalculator::updateStatistics(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states)
{
    // We only care about 1v1 for now
    if (states.count() != 2)
        return;

    updateDamageTaken(states);
    updateDamagesAtDeath(states);
    updateFirstBlood(states);
    updateStageControl(states);

    dispatcher.dispatch(&StatsCalculatorListener::onStatsUpdated);
}

// ----------------------------------------------------------------------------
double StatsCalculator::avgDamagePerOpening(int fighterIdx) const
{
    return 0.0;
}

// ----------------------------------------------------------------------------
double StatsCalculator::avgDeathPercent(int fighterIdx) const
{
    if (damagesAtDeath_[fighterIdx].count() == 0)
        return 0.0;

    double sum = 0.0;
    for (double percent : damagesAtDeath_[fighterIdx])
        sum += percent;
    sum /= damagesAtDeath_[fighterIdx].count();
    return sum;
}

// ----------------------------------------------------------------------------
double StatsCalculator::earliestDeathPercent(int fighterIdx) const
{
    if (damagesAtDeath_[fighterIdx].count() == 0)
        return 0.0;

    double low = std::numeric_limits<double>::max();
    for (double percent : damagesAtDeath_[fighterIdx])
        if (low > percent)
            low = percent;
    return low;
}

// ----------------------------------------------------------------------------
double StatsCalculator::latestDeathPercent(int fighterIdx) const
{
    if (damagesAtDeath_[fighterIdx].count() == 0)
        return 0.0;

    double high = 0.0;
    for (double percent : damagesAtDeath_[fighterIdx])
        if (high < percent)
            high = percent;
    return high;
}

// ----------------------------------------------------------------------------
int StatsCalculator::numNeutralWins(int fighterIdx) const
{
    return 0.0;
}

// ----------------------------------------------------------------------------
double StatsCalculator::neutralWinPercent(int fighterIdx) const
{
    // playerpunishes / (playerpunishes + opponentpunishes)
    return 0.0;
}

// ----------------------------------------------------------------------------
int StatsCalculator::numOpeningsPerKill(int fighterIdx) const
{
    // round(playerpunishes / playerkillingpunishes, 2)
    return 0;
}

// ----------------------------------------------------------------------------
int StatsCalculator::numStocksTaken(int fighterIdx) const
{
    // playerkillingpunishes
    return 0;
}

// ----------------------------------------------------------------------------
double StatsCalculator::stageControlPercent(int fighterIdx) const
{
    int totalStageControl = 0;
    for (int stageControl : stageControl_)
        totalStageControl += stageControl;

    if (totalStageControl == 0)
        return 0.0;

    return static_cast<double>(stageControl_[fighterIdx]) * 100.0 / totalStageControl;
}

// ----------------------------------------------------------------------------
void StatsCalculator::updateDamageTaken(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states)
{
    for (int i = 0; i != states.count(); ++i)
    {
        const double deltaDamage = states[i].damage() - oldDamage_[i];
        oldDamage_[i] = states[i].damage();

        // Only care about cases where damage increases. Note that delta can be negative
        // when a player heals or respawns.
        if (deltaDamage > 0.0)
        {
            totalDamageTaken_[i] += deltaDamage;

            // Figure out which player dealt the damage. If there are more than 2 
            // players we have to rely on attackConnected().
            // XXX There is currently no way to distinguish self-damage from projectile
            // damage. attackConnected() is not true for projectiles.
            for (int j = 0; j != states.count(); ++j)
            {
                if (i == j)
                    continue;

                if (states.count() == 2)  // Is 1v1
                {
                    // This ignores self damage but it's close enough
                    totalDamageDealt_[j] += deltaDamage;
                }
                else
                {
                    // This ignores projectile damage
                    if (states[j].attackConnected())
                        totalDamageDealt_[j] += deltaDamage;
                }
            }

            // Damage but accounting for heals
            // if (states[i].damage() != oldDamage_[i] && states[i].status() != FIGHTER_STATUS_KIND_REBIRTH)
            //     damageTaken_[i] += states[i].damage() - oldDamage_[i];
        }

    }
}

// ----------------------------------------------------------------------------
void StatsCalculator::updateDamagesAtDeath(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states)
{
    for (int i = 0; i != states.count(); ++i)
    {
        // This is either the first frame, or the player somehow gained a stock (items lol?)
        if (oldStocks_[i] < states[i].stocks())
            oldStocks_[i] = states[i].stocks();

        // Store player damage at death
        if (states[i].stocks() < oldStocks_[i])
        {
            damagesAtDeath_[i].push(states[i].damage());
            oldStocks_[i] = states[i].stocks();
        }
    }
}

// ----------------------------------------------------------------------------
void StatsCalculator::updateFirstBlood(const rfcommon::SmallVector<rfcommon::PlayerState, 8> & states)
{
    if (firstBloodFighterIdx_ == -1)
    {
        if (states[0].stocks() > states[1].stocks())
            firstBloodFighterIdx_ = 0;
        if (states[0].stocks() < states[1].stocks())
            firstBloodFighterIdx_ = 1;
    }
}

// ----------------------------------------------------------------------------
static bool isTouchingGround(const rfcommon::PlayerState& state)
{
    const rfcommon::FighterStatus landStates[] = {
        FIGHTER_STATUS_KIND_LANDING,
        FIGHTER_STATUS_KIND_PASSIVE,
        FIGHTER_STATUS_KIND_PASSIVE_FB,
        FIGHTER_STATUS_KIND_WAIT,
        FIGHTER_STATUS_KIND_GUARD_ON,
        FIGHTER_STATUS_KIND_JUMP_SQUAT,
        FIGHTER_STATUS_KIND_WALK,
        FIGHTER_STATUS_KIND_DASH
    };
    
    for (int i = 0; i != sizeof(landStates) / sizeof(*landStates); ++i)
        if (state.status() == landStates[i])
            return true;

    return false;
}
void StatsCalculator::updateStageControl(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states)
{
    // Got hit? No longer in neutral state
    for (int i = 0; i != states.count(); ++i)
        if (states[i].hitstun() > 0.0)
            isInNeutralState_[i] = 0;

    // If player is no longer in hitstun and touches the ground, we put them back
    // into neutral state
    for (int i = 0; i != states.count(); ++i)
        if (states[i].hitstun() == 0.0 && isTouchingGround(states[i]))
            isInNeutralState_[i] = 1;

    // Figure out which player is in neutral and closest to stage center
    int playerInStageControl = -1;
    double distanceToCenter = std::numeric_limits<double>::max();
    for (int i = 0; i != states.count(); ++i)
    {
        if (isInNeutralState_[i] && std::abs(states[i].posx()) < distanceToCenter)
        {
            distanceToCenter = std::abs(states[i].posx());
            playerInStageControl = i;
        }
    }

    if (playerInStageControl > -1)
        stageControl_[playerInStageControl]++;
}
