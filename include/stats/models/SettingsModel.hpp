#pragma once

#include "rfcommon/ListenerDispatcher.hpp"
#include "stats/models/StatisticType.hpp"

class SettingsListener;

class SettingsModel
{
public:
    bool load(const char* filename);
    bool save(const char* filename);

    void setStatEnabled(StatisticType type, bool enable)
        { statEnabled_[type] = enable; }
    bool statEnabled(StatisticType type) const
        { return statEnabled_[type]; }

    void setStatPosition(StatisticType type, int idx)
        { statPosition_[type] = idx; }
    int statPosition(StatisticType type) const
        { return statPosition_[type]; }

    void setExportToOBS(bool enable)
        { exportToOBS_ = enable; }
    bool exportToOBS() const
        { return exportToOBS_; }

    rfcommon::ListenerDispatcher<SettingsListener> dispatcher;

private:
    bool statEnabled_[STAT_COUNT] = {
#define X(type, str) true,
        STATISTIC_TYPES_LIST
#undef X
    };

    int statPosition_[STAT_COUNT] = {
#define X(type, str) STAT_##type,
        STATISTIC_TYPES_LIST
#undef X
    };

    bool exportToOBS_ = false;
};
