#include "stats/util/StatsFormatter.hpp"
#include "stats/models/StatsCalculator.hpp"
#include <utility>

// ----------------------------------------------------------------------------
StatsFormatter::StatsFormatter(const StatsCalculator* stats)
    : stats_(stats)
{
}

// ----------------------------------------------------------------------------
QString StatsFormatter::playerStatAsString(int fighterIdx, StatType type) const
{
    switch (type)
    {
    case STAT_AVERAGE_DAMAGE_PER_OPENING : return QString::number(stats_->avgDamagePerOpening(fighterIdx), 'f', 1) + "%";
    case STAT_AVERAGE_DEATH_PERCENT      : return QString::number(stats_->avgDeathPercent(fighterIdx), 'f', 1) + "%";
    case STAT_EARLIEST_DEATH             : return QString::number(stats_->earliestDeathPercent(fighterIdx), 'f', 1) + "%";
    case STAT_LATEST_DEATH               : return QString::number(stats_->latestDeathPercent(fighterIdx), 'f', 1) + "%";
    case STAT_NEUTRAL_WINS               : return QString::number(stats_->numNeutralWins(fighterIdx));
    case STAT_NEUTRAL_WIN_PERCENT        : return QString::number(stats_->neutralWinPercent(fighterIdx), 'f', 1) + "%";
    case STAT_OPENINGS_PER_KILL          : return QString::number(stats_->numOpeningsPerKill(fighterIdx));
    case STAT_STOCKS_TAKEN               : return QString::number(stats_->numStocksTaken(fighterIdx));
    case STAT_STAGE_CONTROL_PERCENT      : return QString::number(stats_->stageControlPercent(fighterIdx), 'f', 1) + "%";
    case STAT_TOTAL_DAMAGE_DEALT         : return QString::number(stats_->totalDamageDealt(fighterIdx), 'f', 1) + "%";
    case STAT_TOTAL_DAMAGE_RECEIVED      : return QString::number(stats_->totalDamageTaken(fighterIdx), 'f', 1) + "%";
    }

    return "";
}
