#include "stats/models/StatisticType.hpp"

const char* statisticTypeToString(StatisticType type)
{
    static const char* table[] = {
#define X(type, str) str,
        STATISTIC_TYPES_LIST
#undef X
    };

    return table[type];
}
