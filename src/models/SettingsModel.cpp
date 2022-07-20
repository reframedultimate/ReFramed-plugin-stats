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
void SettingsModel::setStatEnabled(StatType type, bool enable)
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
    dispatcher.dispatch(&SettingsListener::onSettingsStatTypesChanged);
}

// ----------------------------------------------------------------------------
bool SettingsModel::statEnabled(StatType type) const
{
    return statEnabled_[type];
}

// ----------------------------------------------------------------------------
void SettingsModel::setStatAtIndex(int idx, StatType type)
{
    if (statAtIndex_[idx] == type)
        return;

    int currentIdx = 0;
    for (; statAtIndex_[currentIdx] != type; ++currentIdx)
        if (currentIdx == statAtIndex_.count())
            return;  // This stat wasn't enabled

    statAtIndex_.take(currentIdx);
    statAtIndex_.insert(idx, type);

    dispatcher.dispatch(&SettingsListener::onSettingsStatTypesChanged);
}

// ----------------------------------------------------------------------------
StatType SettingsModel::statAtIndex(int idx) const
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
    dispatcher.dispatch(&SettingsListener::onSettingsOBSChanged);
}

// ----------------------------------------------------------------------------
void SettingsModel::setAdditionalNewlinesOBS(int lines)
{
    additionalNewlinesOBS_ = lines;
    dispatcher.dispatch(&SettingsListener::onSettingsOBSChanged);
}

// ----------------------------------------------------------------------------
void SettingsModel::setDestinationFolderOBS(const QString& dir)
{
    destinationFolderOBS_ = dir;
    dispatcher.dispatch(&SettingsListener::onSettingsOBSChanged);
}
