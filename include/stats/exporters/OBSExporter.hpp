#pragma once

#include <QString>
#include <QVector>

class StatsCalculator;
class SettingsModel;

class QDir;

class OBSExporter
{
public:
    OBSExporter(const StatsCalculator* stats, const SettingsModel* settings);
    bool exportEmptyValues() const;
    bool exportStatistics() const;

    void setPlayerTag(int idx, const QString& tag);
    void setPlayerCharacter(int idx, const QString& character);

private:
    bool writeNames() const;
    bool writeScene() const;
    bool writePlayerCharacters() const;
    bool writePlayerTags() const;
    bool writePlayerStats() const;
    bool writePlayerStatsEmpty() const;

private:
    const StatsCalculator* stats_;
    const SettingsModel* settings_;
    QVector<QString> tags_;
    QVector<QString> chars_;
};
