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
    case STAT_AVERAGE_DAMAGE_PER_OPENING : return QString::number(stats_->avgDamagePerOpening(fighterIdx), 'g', 1) + "%";
    case STAT_AVERAGE_KILL_PERCENT       : return QString::number(stats_->avgKillPercent(fighterIdx), 'g', 1) + "%";
    case STAT_EARLIEST_KILL              : return QString::number(stats_->earliestKillPercent(fighterIdx), 'g', 1) + "%";
    case STAT_LATEST_DEATH               : return QString::number(stats_->latestDeathPercent(fighterIdx), 'g', 1) + "%";
    case STAT_NEUTRAL_WINS               : return QString::number(stats_->numNeutralWins(fighterIdx));
    case STAT_NEUTRAL_WIN_PERCENT        : return QString::number(stats_->neutralWinPercent(fighterIdx), 'g', 1) + "%";
    case STAT_OPENINGS_PER_KILL          : return QString::number(stats_->numOpeningsPerKill(fighterIdx));
    case STAT_STOCKS_TAKEN               : return QString::number(stats_->numStocksTaken(fighterIdx));
    case STAT_STAGE_CONTROL_PERCENT      : return QString::number(stats_->stageControlPercent(fighterIdx), 'g', 1) + "%";
    case STAT_TOTAL_DAMAGE_DEALT         : return QString::number(stats_->totalDamageDealt(fighterIdx), 'g', 1) + "%";
    }

    return "";
}
