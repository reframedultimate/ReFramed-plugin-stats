#include "stats/StatsPlugin.hpp"
#include "stats/exporters/OBSExporter.hpp"
#include "stats/models/StatsCalculator.hpp"
#include "stats/models/SettingsModel.hpp"
#include "stats/models/UserLabelsModel.hpp"
#include "stats/util/Paths.hpp"
#include "stats/views/MainView.hpp"
#include "rfcommon/RunningGameSession.hpp"
#include "rfcommon/SavedGameSession.hpp"

#include "stats/views/SettingsView.hpp"

// ----------------------------------------------------------------------------
StatsPlugin::StatsPlugin(RFPluginFactory* factory)
    : RealtimePlugin(factory)
    , statsCalculator_(new StatsCalculator)
    , settingsModel_(new SettingsModel(dataDir().absoluteFilePath("settings.json")))
    , motionLabels_(new UserLabelsModel)
{
#if defined(_WIN32)
    motionLabels_->loadMotionLabels("share\\reframed\\data\\plugin-stats\\ParamLabels.csv");
#else
    motionLabels_->loadMotionLabels("share/reframed/data/plugin-stats/ParamLabels.csv");
#endif

    settingsModel_->dispatcher.addListener(this);
}

// ----------------------------------------------------------------------------
StatsPlugin::~StatsPlugin()
{
    if (session_)
        session_->dispatcher.removeListener(this);

    settingsModel_->dispatcher.removeListener(this);
}

// ----------------------------------------------------------------------------
QWidget* StatsPlugin::createView()
{
    // Create new instance of view. The view registers as a listener to this model
    //return new StatsView(model_.get());
    return new MainView(statsCalculator_.get(), settingsModel_.get(), motionLabels_.get());
}

// ----------------------------------------------------------------------------
void StatsPlugin::destroyView(QWidget* view)
{
    // ReFramed no longer needs the view, delete it
    delete view;
}

// ----------------------------------------------------------------------------
void StatsPlugin::exportEmptyStats() const
{
    if (settingsModel_->exportToOBS())
    {
        OBSExporter exporter(statsCalculator_.get(), settingsModel_.get(), motionLabels_.get());
        if (session_)
        {
            const rfcommon::FighterIDMapping& fighterIDs = session_->mappingInfo().fighterID;
            rfcommon::FighterID p1FighterID = session_->playerFighterID(0);
            rfcommon::FighterID p2FighterID = session_->playerFighterID(1);
            const rfcommon::String* p1Fighter = fighterIDs.map(p1FighterID);
            const rfcommon::String* p2Fighter = fighterIDs.map(p2FighterID);

            exporter.setPlayerTag(0, session_->playerTag(0).cStr());
            exporter.setPlayerTag(1, session_->playerTag(1).cStr());
            exporter.setPlayerCharacter(0, p1Fighter ? p1Fighter->cStr() : "--");
            exporter.setPlayerCharacter(1, p2Fighter ? p2Fighter->cStr() : "--");
        }

        exporter.exportEmptyValues();
    }
}

// ----------------------------------------------------------------------------
void StatsPlugin::exportStats(const rfcommon::Session* session) const
{
    if (settingsModel_->exportToOBS())
    {
        const rfcommon::FighterIDMapping& fighterIDs = session->mappingInfo().fighterID;
        rfcommon::FighterID p1FighterID = session->playerFighterID(0);
        rfcommon::FighterID p2FighterID = session->playerFighterID(1);
        const rfcommon::String* p1Fighter = fighterIDs.map(p1FighterID);
        const rfcommon::String* p2Fighter = fighterIDs.map(p2FighterID);

        OBSExporter exporter(statsCalculator_.get(), settingsModel_.get(), motionLabels_.get());
        exporter.setPlayerTag(0, session->playerTag(0).cStr());
        exporter.setPlayerTag(1, session->playerTag(1).cStr());
        exporter.setPlayerCharacter(0, p1Fighter ? p1Fighter->cStr() : "--");
        exporter.setPlayerCharacter(1, p2Fighter ? p2Fighter->cStr() : "--");
        exporter.exportStatistics();
    }
}

// ----------------------------------------------------------------------------
void StatsPlugin::onProtocolMatchStarted(rfcommon::RunningGameSession* session)
{
    // Unregister from current session, register to new session
    if (session_)
        session_->dispatcher.removeListener(this);
    session_ = session;
    session_->dispatcher.addListener(this);

    // If the session already has frames, process them so we are caught up
    statsCalculator_->resetStatistics();
    session_->replayUniqueFrameEvents(this);

    exportEmptyStats();
}

// ----------------------------------------------------------------------------
void StatsPlugin::onProtocolMatchResumed(rfcommon::RunningGameSession* session)
{
    // Unregister from current session, register to new session
    if (session_)
        session_->dispatcher.removeListener(this);
    session_ = session;
    session_->dispatcher.addListener(this);

    // If the session already has frames, process them so we are caught up
    statsCalculator_->resetStatistics();
    session_->replayUniqueFrameEvents(this);

    exportEmptyStats();
}

// ----------------------------------------------------------------------------
void StatsPlugin::onProtocolMatchEnded(rfcommon::RunningGameSession* session)
{
    exportStats(session);

    // We hold on to our reference to the session object until a new session
    // is started, so that if settings change, the exporters still have
    // data to export
}

// ----------------------------------------------------------------------------
void StatsPlugin::setSavedGameSession(rfcommon::SavedGameSession* session)
{
    statsCalculator_->resetStatistics();
    session->replayUniqueFrameEvents(this);

    exportStats(session);
}

// ----------------------------------------------------------------------------
void StatsPlugin::onRunningSessionNewUniqueFrame(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states)
{
    statsCalculator_->updateStatistics(states);
}

// ----------------------------------------------------------------------------
void StatsPlugin::onSettingsStatTypesChanged()
{
    if (session_)
        exportStats(session_);
    else
        exportEmptyStats();
}

// ----------------------------------------------------------------------------
void StatsPlugin::onSettingsOBSChanged()
{
    if (session_)
        exportStats(session_);
    else
        exportEmptyStats();
}
