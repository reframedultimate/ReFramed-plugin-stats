#pragma once

class StatsCalculator;
class SettingsModel;

class QDir;

class OBSExporter
{
public:
    OBSExporter(const StatsCalculator* stats, const SettingsModel* settings);
    bool exportTo(const QDir& path);

private:
    const StatsCalculator* stats_;
    const SettingsModel* settings_;
};
