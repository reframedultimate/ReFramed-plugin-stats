#pragma once

#include "rfcommon/ListenerDispatcher.hpp"
#include "stats/models/StatisticType.hpp"

#include <QString>

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
    bool exportToOBS() const { return exportToOBS_; }

    void setAdditionalNewlinesOBS(int lines);
    int additionalNewlinesOBS() const { return additionalNewlinesOBS_;  }

    void setDestinationFolderOBS(const QString& dir);
    const QString& destinationFolderOBS() const { return destinationFolderOBS_; }

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
    int additionalNewlinesOBS_ = 0;
    QString destinationFolderOBS_;
};
