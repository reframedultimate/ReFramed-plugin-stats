#include "stats/config.hpp"
#include "stats/PluginConfig.hpp"
#include "stats/StatsPlugin.hpp"
#include "stats/util/Paths.hpp"
#include "rfcommon/PluginInterface.hpp"

#if defined(STATS_ENABLE_LICENSING)
#   include "stats/views/LicenseDialog.hpp"
#endif

// Gets called when the main application wants to create your plugin
static rfcommon::Plugin* createStatsPlugin(RFPluginFactory* factory)
{
    return new StatsPlugin(factory);
}

// Gets called when the main application removes your plugin from its
// list. You have to delete the object here.
static void destroyStatsPlugin(rfcommon::Plugin* model)
{
    delete model;
}

// This is a list of create/destroy functions which the main application uses
// to instantiate your plugins. You can have multiple plugins in a single
// shared libary, but in this case we only have one.
static RFPluginFactory factories[] = {
    {createStatsPlugin, destroyStatsPlugin, RFPluginType::REALTIME,
    {"Statistics",
     "misc > misc",  // category > sub-category
     "Vye, TheComet",  // your name
     "Vye#0547, @leeevye, TheComet#5387, @TheComet93",  // various contact details
     "Generates statistics after each game such as stage control, neutral wins, average kill%, etc.\n"
     "\n"
     "Data can be exported to OBS to create a real-time stream overlay"}},

    {0}  // List must be terminated with a NULL or bad things happen!
};

static int start(uint32_t version, const char** error)
{
    // Gets called right after shared library is loaded. Initialize global state
    // here
    //
    // Version of ReFramed gets passed in here. Return 0 if you're compatible,
    // any other number (-1) if otherwise. If you return non-zero, write an
    // error message to *error so the main application can tell the user what
    // went wrong.

    if (ensureDataDirExists() == false)
    {
        *error = "Failed to create data directory";
        return -1;
    }

#if defined(STATS_ENABLE_LICENSING)
    QString licenseFileName = dataDir().absoluteFilePath("license.txt");
    QFile licenseFile(licenseFileName);
    if (licenseFile.exists())
    {
        licenseFile.open(QIODevice::ReadOnly);
        QDataStream stream(&licenseFile);
        QString result;
        stream >> result;
        if (result == STATS_LICENSED_TO)
            return 0;
        licenseFile.close();
    }
    
    LicenseDialog dialog;
    if (dialog.exec() == QDialog::Accepted)
    {
        licenseFile.open(QIODevice::WriteOnly);
        QDataStream stream(&licenseFile);
        stream << QString(STATS_LICENSED_TO);
    }
    else
    {
        *error = "License was rejected";
        return -1;
    }
#endif

    return 0;
}

static void stop()
{
    // Gets called before the shared libary is unloaded. Clean up global state here.
}

DEFINE_PLUGIN(factories, start, stop)
