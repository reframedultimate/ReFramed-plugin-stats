#include "stats/StatsPlugin.hpp"
#include "stats/models/StatsModel.hpp"
#include "stats/views/StatsView.hpp"
#include "rfcommon/RunningGameSession.hpp"
#include "rfcommon/SavedGameSession.hpp"

#include "stats/views/SettingsView.hpp"

// ----------------------------------------------------------------------------
StatsPlugin::StatsPlugin(RFPluginFactory* factory)
    : RealtimePlugin(factory)
    , model_(new StatsModel)
{
}

// ----------------------------------------------------------------------------
StatsPlugin::~StatsPlugin()
{
}

// ----------------------------------------------------------------------------
QWidget* StatsPlugin::createView()
{
    // Create new instance of view. The view registers as a listener to this model
    //return new StatsView(model_.get());
    return new SettingsView(nullptr);
}

// ----------------------------------------------------------------------------
void StatsPlugin::destroyView(QWidget* view)
{
    // ReFramed no longer needs the view, delete it
    delete view;
}

// ----------------------------------------------------------------------------
void StatsPlugin::onProtocolMatchEnded(rfcommon::RunningGameSession* session)
{
    // A match just ended. We tell the model to release any references to the
    // session
    model_->calculateStatistics(session);
}

// ----------------------------------------------------------------------------
void StatsPlugin::setSavedGameSession(rfcommon::SavedGameSession* session)
{
    model_->calculateStatistics(session);
}
