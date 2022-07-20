#pragma once

#include "rfcommon/ListenerDispatcher.hpp"
#include "stats/StatType.hpp"

#include <QString>
#include <QDir>

class SettingsListener;

class SettingsModel
{
public:
    SettingsModel(const QString& settingsFile);

    bool load();
    bool save();

    void setStatEnabled(StatType type, bool enable);
    bool statEnabled(StatType type) const;

    void setStatAtIndex(int idx, StatType type);
    StatType statAtIndex(int idx) const;

    int numStatsEnabled() const;

    void setExportToOBS(bool enable);
    bool exportToOBS() const { return exportToOBS_; }

    void setAdditionalNewlinesOBS(int lines);
    int additionalNewlinesOBS() const { return additionalNewlinesOBS_;  }

    void setDestinationFolderOBS(const QDir& dir);
    const QDir& destinationFolderOBS() const { return destinationFolderOBS_; }

    rfcommon::ListenerDispatcher<SettingsListener> dispatcher;

private:
    bool statEnabled_[STAT_COUNT] = {
#define X(type, str) true,
        STAT_TYPES_LIST
#undef X
    };

    rfcommon::SmallVector<StatType, STAT_COUNT> statAtIndex_ = {
#define X(type, str) STAT_##type,
        STAT_TYPES_LIST
#undef X
    };

    QString settingsFile_;
    QDir destinationFolderOBS_;
    bool exportToOBS_ = false;
    int additionalNewlinesOBS_ = 0;
};
