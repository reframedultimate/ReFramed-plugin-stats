#include "stats/widgets/SettingsStatsItem.hpp"

#include <QLabel>

// ----------------------------------------------------------------------------
SettingsStatsItem::SettingsStatsItem(StatType type, QWidget* parent)
    : QLabel(statTypeToString(type), parent)
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
