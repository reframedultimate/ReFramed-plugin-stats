#include "stats/models/SettingsModel.hpp"
#include "stats/listeners/SettingsListener.hpp"

#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QFile>

// ----------------------------------------------------------------------------
SettingsModel::SettingsModel(const QString& settingsFile)
    : settingsFile_(settingsFile)
{
    load();
}

// ----------------------------------------------------------------------------
bool SettingsModel::load()
{
    QFile file(settingsFile_);
    if (file.open(QIODevice::ReadOnly) == false)
        return false;

    QJsonObject data = QJsonDocument::fromJson(file.readAll()).object();
    
    const QJsonObject obs = data["obs"].toObject();
    exportToOBS_ = obs["enabled"].toBool(false);
    additionalNewlinesOBS_ = obs["extranewlines"].toInt(0);
    destinationFolderOBS_ = obs["dir"].toString("");

    const QJsonObject stats = data["stats"].toObject();
    if (stats.contains("enabled"))
    {
        // Reset arrays
        statAtIndex_.clear();
        for (int i = 0; i != STAT_COUNT; ++i)
            statEnabled_[i] = false;

        const QJsonArray enabledStats = stats["enabled"].toArray();
        for (const auto& statNameRef : enabledStats)
        {
            const QString statName = statNameRef.toString("");
            if (statName == "")
                continue;

            QByteArray ba = statName.toLatin1();
            StatType type = stringToStatType(ba.constData());
            if (type == STAT_COUNT)
                continue;

            statEnabled_[type] = true;
            statAtIndex_.push(type);
        }
    }
    else
    {
        // Enable everything by default
        statAtIndex_.clear();
        for (int i = 0; i != STAT_COUNT; ++i)
        {
            statEnabled_[i] = true;
            statAtIndex_.push(static_cast<StatType>(i));
        }
    }

    return true;
}

// ----------------------------------------------------------------------------
bool SettingsModel::save()
{
    QJsonArray enabledStats;
    for (int i = 0; i != numStatsEnabled(); ++i)
    {
        StatType type = statAtIndex(i);
        enabledStats.append(statTypeToString(type));
    }

    QJsonObject stats = {
        {"enabled", enabledStats}
    };

    QJsonObject obs = {
        {"enabled", exportToOBS()},
        {"extranewlines", additionalNewlinesOBS()},
        {"dir", destinationFolderOBS().absolutePath()}
    };

    QJsonObject data = {
        {"version", "1.0"},
        {"stats", stats},
        {"obs", obs}
    };

    QFile file(settingsFile_);
    if (file.open(QIODevice::WriteOnly) == false)
        return false;
    file.write(QJsonDocument(data).toJson());
    return true;
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

    save();
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

    save();
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

    save();
    dispatcher.dispatch(&SettingsListener::onSettingsOBSChanged);
}

// ----------------------------------------------------------------------------
void SettingsModel::setAdditionalNewlinesOBS(int lines)
{
    additionalNewlinesOBS_ = lines;

    save();
    dispatcher.dispatch(&SettingsListener::onSettingsOBSChanged);
}

// ----------------------------------------------------------------------------
void SettingsModel::setDestinationFolderOBS(const QDir& dir)
{
    destinationFolderOBS_ = dir;

    save();
    dispatcher.dispatch(&SettingsListener::onSettingsOBSChanged);
}
