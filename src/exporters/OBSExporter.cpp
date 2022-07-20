#include "stats/exporters/OBSExporter.hpp"
#include "stats/models/SettingsModel.hpp"
#include "stats/models/SettingsModel.hpp"


// ----------------------------------------------------------------------------
OBSExporter::OBSExporter(const StatsCalculator* stats, const SettingsModel* settings)
    : stats_(stats)
    , settings_(settings)
{
}

// ----------------------------------------------------------------------------
bool OBSExporter::exportTo(const QDir& path)
{
    return false;
}
