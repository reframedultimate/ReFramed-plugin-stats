#include "stats/models/SettingsModel.hpp"
#include "stats/listeners/SettingsListener.hpp"

// ----------------------------------------------------------------------------
bool SettingsModel::load(const char* filename)
{
    return false;
}

// ----------------------------------------------------------------------------
bool SettingsModel::save(const char* filename)
{
    return false;
}

// ----------------------------------------------------------------------------
void SettingsModel::setStatEnabled(StatisticType type, bool enable)
{
    if (statEnabled_[type] == enable)
        return;

    if (enable)
        statAtIndex_.push(type);
    else
    {
        for (auto it = statAtIndex_.begin(); it != statAtIndex_.end(); ++it)
            if (*it == type)
            {
                statAtIndex_.erase(it);
                break;
            }
    }

    statEnabled_[type] = enable;
    dispatcher.dispatch(&SettingsListener::onSettingsChanged);
}

// ----------------------------------------------------------------------------
bool SettingsModel::statEnabled(StatisticType type) const
{
    return statEnabled_[type];
}

// ----------------------------------------------------------------------------
void SettingsModel::setStatAtIndex(int idx, StatisticType type)
{
    if (statAtIndex_[idx] == type)
        return;

    int currentIdx = 0;
    for (; statAtIndex_[currentIdx] != type; ++currentIdx)
        if (currentIdx == statAtIndex_.count())
            return;  // This stat wasn't enabled

    statAtIndex_.take(currentIdx);
    statAtIndex_.insert(idx, type);

    dispatcher.dispatch(&SettingsListener::onSettingsChanged);
}

// ----------------------------------------------------------------------------
StatisticType SettingsModel::statAtIndex(int idx) const
{
    return statAtIndex_[idx];
}

// ----------------------------------------------------------------------------
int SettingsModel::numStatsEnabled() const
{
    return statAtIndex_.count();
}

// ----------------------------------------------------------------------------
void SettingsModel::setExportToOBS(bool enable)
{
    exportToOBS_ = enable;
    dispatcher.dispatch(&SettingsListener::onSettingsChanged);
}

// ----------------------------------------------------------------------------
bool SettingsModel::exportToOBS() const
{
    return exportToOBS_;
}
