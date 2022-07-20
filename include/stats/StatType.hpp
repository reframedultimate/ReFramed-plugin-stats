#pragma once

#define STAT_TYPES_LIST \
    X(AVERAGE_DAMAGE_PER_OPENING, "Average Damage / Opening") \
    X(AVERAGE_DEATH_PERCENT, "Average Death%") \
    X(EARLIEST_DEATH, "Earliest Death") \
    X(LATEST_DEATH, "Latest Death") \
    X(NEUTRAL_WINS, "Neutral Wins") \
    X(NEUTRAL_WIN_PERCENT, "Neutral Win %") \
    X(OPENINGS_PER_KILL, "Openings / Kill") \
    X(STOCKS_TAKEN, "Stocks Taken") \
    X(STAGE_CONTROL_PERCENT, "Stage Control") \
    X(TOTAL_DAMAGE_DEALT, "Total Damage Dealt") \
    X(TOTAL_DAMAGE_RECEIVED, "Total Damage Received")

enum StatType
{
#define X(type, str) STAT_##type,
    STAT_TYPES_LIST
#undef X
    STAT_COUNT
};

const char* statTypeToString(StatType type);
StatType stringToStatType(const char* s);
