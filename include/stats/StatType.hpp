#pragma once

#define STAT_TYPES_LIST \
    X(AVERAGE_DEATH_PERCENT, "Average Death%") \
    X(EARLIEST_DEATH, "Earliest Death") \
    X(LATEST_DEATH, "Latest Death") \
    X(NEUTRAL_WINS, "Neutral Wins") \
    X(NEUTRAL_LOSSES, "Neutral Losses") \
    X(NON_KILLING_NEUTRAL_WINS, "Non-Killing Neutral Wins") \
    X(STOCKS_TAKEN, "Stocks Taken") \
    X(NEUTRAL_WIN_PERCENT, "Neutral Win %") \
    X(AVERAGE_DAMAGE_PER_OPENING, "Average Damage / Opening") \
    X(OPENINGS_PER_KILL, "Openings / Kill") \
    X(STAGE_CONTROL_PERCENT, "Stage Control") \
    X(TOTAL_DAMAGE_DEALT, "Total Damage Dealt") \
    X(TOTAL_DAMAGE_RECEIVED, "Total Damage Received") \
    X(MOST_COMMON_NEUTRAL_OPENING_MOVE, "Most Common Neutral Opener") \
    X(MOST_COMMON_KILL_MOVE, "Most Common Kill Move") \
    X(MOST_COMMON_NEUTRAL_OPENING_MOVE_INTO_KILL, "Most Common Neutral Opener into Kill")

enum StatType
{
#define X(type, str) STAT_##type,
    STAT_TYPES_LIST
#undef X
    STAT_COUNT
};

const char* statTypeToString(StatType type);
StatType stringToStatType(const char* s);
