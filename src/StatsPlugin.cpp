#include "stats/StatsPlugin.hpp"
#include "stats/models/StatsCalculator.hpp"
#include "stats/models/SettingsModel.hpp"
#include "stats/views/MainView.hpp"
#include "rfcommon/RunningGameSession.hpp"
#include "rfcommon/SavedGameSession.hpp"

#include "stats/views/SettingsView.hpp"

// ----------------------------------------------------------------------------
StatsPlugin::StatsPlugin(RFPluginFactory* factory)
    : RealtimePlugin(factory)
    , statsModel_(new StatsCalculator)
    , settingsModel_(new SettingsModel)
{
}

// ----------------------------------------------------------------------------
StatsPlugin::~StatsPlugin()
{
    if (session_)
        session_->dispatcher.removeListener(this);
}

// ----------------------------------------------------------------------------
QWidget* StatsPlugin::createView()
{
    // Create new instance of view. The view registers as a listener to this model
    //return new StatsView(model_.get());
    return new MainView(statsModel_.get(), settingsModel_.get());
}

// ----------------------------------------------------------------------------
void StatsPlugin::destroyView(QWidget* view)
{
    // ReFramed no longer needs the view, delete it
    delete view;
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
    statsModel_->resetStatistics();
    session_->replayUniqueFrameEvents(this);
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
    statsModel_->resetStatistics();
    session_->replayUniqueFrameEvents(this);
}

// ----------------------------------------------------------------------------
void StatsPlugin::onProtocolMatchEnded(rfcommon::RunningGameSession* session)
{
    if (session_)
        session_->dispatcher.removeListener(this);
    session_.drop();

    // TODO export stats data
}

// ----------------------------------------------------------------------------
void StatsPlugin::setSavedGameSession(rfcommon::SavedGameSession* session)
{
    statsModel_->resetStatistics();
    session->replayUniqueFrameEvents(this);

    // TODO export stats data
}

// ----------------------------------------------------------------------------
void StatsPlugin::onRunningSessionNewUniqueFrame(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states)
{
    statsModel_->updateStatistics(states);
}
