#pragma once

#define STATISTIC_TYPES_LIST \
    X(AVERAGE_DAMAGE_PER_OPENING, "Average Damage / Opening") \
    X(AVERAGE_KILL_PERCENT, "Average Kill %") \
    X(EARLIEST_KILL, "Earliest Kill") \
    X(LATEST_DEATH, "Latest Death") \
    X(NEUTRAL_WINS, "Neutral Wins") \
    X(NEUTRAL_WIN_PERCENT, "Neutral Win %") \
    X(OPENINGS_PER_KILL, "Openings / Kill") \
    X(STOCKS_TAKEN, "Stocks Taken") \
    X(STAGE_CONTROL_PERCENT, "Stage Control") \
    X(TOTAL_DAMAGE_DEALT, "Total Damage Dealt")

enum StatisticType
{
#define X(type, str) STAT_##type,
    STATISTIC_TYPES_LIST
#undef X
    STAT_COUNT
};

const char* statisticTypeToString(StatisticType type);
