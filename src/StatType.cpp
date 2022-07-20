#include "stats/StatType.hpp"

const char* statTypeToString(StatType type)
{
    static const char* table[] = {
#define X(type, str) str,
        STAT_TYPES_LIST
#undef X
    };

    return table[type];
}
