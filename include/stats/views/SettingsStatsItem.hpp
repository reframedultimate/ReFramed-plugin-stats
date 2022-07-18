#pragma once

#include "stats/models/StatisticType.hpp"
#include <QLabel>

class SettingsStatsItem : public QLabel
{
    Q_OBJECT

public:
    explicit SettingsStatsItem(StatisticType type, QWidget* parent=nullptr);

    StatisticType type() const
        { return type_; }

    const char* typeAsString() const
        { return statisticTypeToString(type_); }

    void setDragInProgress();
    void setDragCancelled();

private:
    const StatisticType type_;
};
