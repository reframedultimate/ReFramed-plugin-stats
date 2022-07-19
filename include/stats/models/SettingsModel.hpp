#pragma once

#include "rfcommon/ListenerDispatcher.hpp"
#include "stats/models/StatisticType.hpp"

class SettingsListener;

class SettingsModel
{
public:
    bool load(const char* filename);
    bool save(const char* filename);

    void setStatEnabled(StatisticType type, bool enable);
    bool statEnabled(StatisticType type) const;

    void setStatAtIndex(int idx, StatisticType type);
    StatisticType statAtIndex(int idx) const;

    int numStatsEnabled() const;

    void setExportToOBS(bool enable);
    bool exportToOBS() const;

    rfcommon::ListenerDispatcher<SettingsListener> dispatcher;

private:
    bool statEnabled_[STAT_COUNT] = {
#define X(type, str) true,
        STATISTIC_TYPES_LIST
#undef X
    };

    rfcommon::SmallVector<StatisticType, STAT_COUNT> statAtIndex_ = {
#define X(type, str) STAT_##type,
        STATISTIC_TYPES_LIST
#undef X
    };

    bool exportToOBS_ = false;
};
