#include "stats/views/SettingsStatsItem.hpp"

#include <QLabel>

// ----------------------------------------------------------------------------
SettingsStatsItem::SettingsStatsItem(StatisticType type, QWidget* parent)
    : QLabel(statisticTypeToString(type), parent)
    , type_(type)
{
}

// ----------------------------------------------------------------------------
void SettingsStatsItem::setDragInProgress()
{
    setText("...");
}

// ----------------------------------------------------------------------------
void SettingsStatsItem::setDragCancelled()
{
    setText(typeAsString());
}
