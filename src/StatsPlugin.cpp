#include "stats/StatsPlugin.hpp"
#include "stats/exporters/OBSExporter.hpp"
#include "stats/models/StatsCalculator.hpp"
#include "stats/models/SettingsModel.hpp"
#include "stats/models/UserLabelsModel.hpp"
#include "stats/util/Paths.hpp"
#include "stats/views/MainView.hpp"

#include "rfcommon/Frame.hpp"
#include "rfcommon/FrameData.hpp"
#include "rfcommon/MappingInfo.hpp"
#include "rfcommon/MetaData.hpp"
#include "rfcommon/Session.hpp"

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
    if (metaData_)
    {
        frameData_->dispatcher.removeListener(this);
        metaData_->dispatcher.removeListener(this);
    }

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
        const auto p1FighterID = metaData_ ? metaData_->fighterID(0) : rfcommon::FighterID::makeInvalid();
        const auto p2FighterID = metaData_ ? metaData_->fighterID(1) : rfcommon::FighterID::makeInvalid();
        const char* p1Fighter = mappingInfo_ ? mappingInfo_->fighter.toName(p1FighterID, "--") : "--";
        const char* p2Fighter = mappingInfo_ ? mappingInfo_->fighter.toName(p2FighterID, "--") : "--";
        const char* p1Tag = metaData_ ? metaData_->name(0).cStr() : "--";
        const char* p2Tag = metaData_ ? metaData_->name(1).cStr() : "--";

        OBSExporter exporter(statsCalculator_.get(), settingsModel_.get(), motionLabels_.get());
        exporter.setPlayerTag(0, p1Tag);
        exporter.setPlayerTag(1, p2Tag);
        exporter.setPlayerCharacter(0, p1Fighter);
        exporter.setPlayerCharacter(1, p2Fighter);
        exporter.exportEmptyValues();
    }
}

// ----------------------------------------------------------------------------
void StatsPlugin::exportStats(const rfcommon::MappingInfo* map, const rfcommon::MetaData* mdata) const
{
    if (settingsModel_->exportToOBS())
    {
        const auto p1FighterID = mdata->fighterID(0);
        const auto p2FighterID = mdata->fighterID(1);
        const char* p1Fighter = map->fighter.toName(p1FighterID, "--");
        const char* p2Fighter = map->fighter.toName(p2FighterID, "--");
        const char* p1Tag = mdata->name(0).cStr();
        const char* p2Tag = mdata->name(1).cStr();

        OBSExporter exporter(statsCalculator_.get(), settingsModel_.get(), motionLabels_.get());
        exporter.setPlayerTag(0, p1Tag);
        exporter.setPlayerTag(1, p2Tag);
        exporter.setPlayerCharacter(0, p1Fighter);
        exporter.setPlayerCharacter(1, p2Fighter);
        exporter.exportStatistics();
    }
}

// ----------------------------------------------------------------------------
void StatsPlugin::onProtocolGameStarted(rfcommon::Session* game)
{
    statsCalculator_->resetStatistics();

    // Unregister from current session
    if (metaData_)
    {
        metaData_->dispatcher.removeListener(this);
        frameData_->dispatcher.removeListener(this);
        mappingInfo_.drop();
        metaData_.drop();
        frameData_.drop();
    }

    // We need mapping info, metadata and frame data in order to process
    // statistics
    auto map = game->tryGetMappingInfo();
    auto mdata = game->tryGetMetaData();
    auto fdata = game->tryGetFrameData();
    if (map == nullptr || mdata == nullptr || fdata == nullptr)
        return;

    // Register as listeners so we are informed when data changes
    mappingInfo_ = map;
    metaData_ = mdata;
    frameData_ = fdata;
    metaData_->dispatcher.addListener(this);
    frameData_->dispatcher.addListener(this);

    // If the session already has frames, process them so we are caught up
    for (int frameIdx = 0; frameIdx != frameData_->frameCount(); ++frameIdx)
    {
        rfcommon::Frame<4> frame;
        for (int fighterIdx = 0; fighterIdx != frameData_->fighterCount(); ++fighterIdx)
            frame.push(frameData_->stateAt(fighterIdx, frameIdx));

        statsCalculator_->updateStatistics(frame);
    }

    // OBS should display empty statistics, since a new game has started now
    exportEmptyStats();
}

// ----------------------------------------------------------------------------
void StatsPlugin::onProtocolGameResumed(rfcommon::Session* game)
{
    // pretty much same thing
    onProtocolGameStarted(game);
}

// ----------------------------------------------------------------------------
void StatsPlugin::onProtocolGameEnded(rfcommon::Session* game)
{
    if (mappingInfo_)
        exportStats(mappingInfo_, metaData_);

    // We hold on to our reference to the data until a new session is started, 
    // so that if settings change, the exporters still have data to export
}

// ----------------------------------------------------------------------------
void StatsPlugin::onGameSessionLoaded(rfcommon::Session* game)
{
    statsCalculator_->resetStatistics();

    // Unregister from current session
    if (metaData_)
    {
        metaData_->dispatcher.removeListener(this);
        frameData_->dispatcher.removeListener(this);
        mappingInfo_.drop();
        metaData_.drop();
        frameData_.drop();
    }

    // We need mapping info, metadata and frame data in order to process
    // statistics
    rfcommon::MappingInfo* map = game->tryGetMappingInfo();
    rfcommon::MetaData* mdata = game->tryGetMetaData();
    rfcommon::FrameData* fdata = game->tryGetFrameData();
    if (map == nullptr || mdata == nullptr || fdata == nullptr)
        return;

    // Register as listeners so we are informed when data changes
    mappingInfo_ = map;
    metaData_ = mdata;
    frameData_ = fdata;
    metaData_->dispatcher.addListener(this);
    frameData_->dispatcher.addListener(this);

    // Process all frames
    for (int frameIdx = 0; frameIdx != fdata->frameCount(); ++frameIdx)
    {
        rfcommon::Frame<4> frame;
        for (int fighterIdx = 0; fighterIdx != fdata->fighterCount(); ++fighterIdx)
            frame.push(fdata->stateAt(fighterIdx, frameIdx));

        statsCalculator_->updateStatistics(frame);
    }

    exportStats(map, mdata);
}

// ----------------------------------------------------------------------------
void StatsPlugin::onGameSessionSetLoaded(rfcommon::Session** games, int numGames)
{
    statsCalculator_->resetStatistics();

    // Unregister from current session
    if (metaData_)
    {
        metaData_->dispatcher.removeListener(this);
        frameData_->dispatcher.removeListener(this);
        mappingInfo_.drop();
        metaData_.drop();
        frameData_.drop();
    }

    while (numGames--)
    {
        rfcommon::Session* game = *games++;

        // We need mapping info, metadata and frame data in order to process
        // statistics
        rfcommon::MappingInfo* map = game->tryGetMappingInfo();
        rfcommon::MetaData* mdata = game->tryGetMetaData();
        rfcommon::FrameData* fdata = game->tryGetFrameData();
        if (map == nullptr || mdata == nullptr || fdata == nullptr)
            continue;

        // Need these for the final export
        mappingInfo_ = map;
        metaData_ = mdata;

        // Process all frames
        for (int frameIdx = 0; frameIdx != fdata->frameCount(); ++frameIdx)
        {
            rfcommon::Frame<4> frame;
            for (int fighterIdx = 0; fighterIdx != fdata->fighterCount(); ++fighterIdx)
                frame.push(fdata->stateAt(fighterIdx, frameIdx));

            statsCalculator_->updateStatistics(frame);
        }
    }

    if (mappingInfo_)
    {
        exportStats(mappingInfo_, metaData_);
        mappingInfo_.drop();
        metaData_.drop();
    }
}

// ----------------------------------------------------------------------------
void StatsPlugin::onMetaDataPlayerNameChanged(int fighterIdx, const rfcommon::SmallString<15>& name)
{
    // TODO update player name
}

// ----------------------------------------------------------------------------
void StatsPlugin::onFrameDataNewFrame(int frameIdx, const rfcommon::Frame<4>& frame)
{
    statsCalculator_->updateStatistics(frame);
}

// ----------------------------------------------------------------------------
void StatsPlugin::onSettingsStatTypesChanged()
{
    if (metaData_)
        exportStats(mappingInfo_, metaData_);
    else
        exportEmptyStats();
}

// ----------------------------------------------------------------------------
void StatsPlugin::onSettingsOBSChanged()
{
    if (metaData_)
        exportStats(mappingInfo_, metaData_);
    else
        exportEmptyStats();
}

// ----------------------------------------------------------------------------
// Unused callbacks
void StatsPlugin::onProtocolAttemptConnectToServer(const char* ipAddress, uint16_t port) {}
void StatsPlugin::onProtocolFailedToConnectToServer(const char* errormsg, const char* ipAddress, uint16_t port) {}
void StatsPlugin::onProtocolConnectedToServer(const char* ipAddress, uint16_t port) {}
void StatsPlugin::onProtocolDisconnectedFromServer() {}

void StatsPlugin::onProtocolTrainingStarted(rfcommon::Session* training) {}
void StatsPlugin::onProtocolTrainingResumed(rfcommon::Session* training) {}
void StatsPlugin::onProtocolTrainingReset(rfcommon::Session* oldTraining, rfcommon::Session* newTraining) {}
void StatsPlugin::onProtocolTrainingEnded(rfcommon::Session* training) {}

void StatsPlugin::onGameSessionUnloaded(rfcommon::Session* game) {}
void StatsPlugin::onGameSessionSetUnloaded(rfcommon::Session** games, int numGames) {}
void StatsPlugin::onTrainingSessionLoaded(rfcommon::Session* training) {}
void StatsPlugin::onTrainingSessionUnloaded(rfcommon::Session* training) {}

void StatsPlugin::onMetaDataTimeStartedChanged(rfcommon::TimeStamp timeStarted) {}
void StatsPlugin::onMetaDataTimeEndedChanged(rfcommon::TimeStamp timeEnded) {}

void StatsPlugin::onMetaDataSetNumberChanged(rfcommon::SetNumber number) {}
void StatsPlugin::onMetaDataGameNumberChanged(rfcommon::GameNumber number) {}
void StatsPlugin::onMetaDataSetFormatChanged(const rfcommon::SetFormat& format) {}
void StatsPlugin::onMetaDataWinnerChanged(int winnerPlayerIdx) {}

void StatsPlugin::onMetaDataTrainingSessionNumberChanged(rfcommon::GameNumber number) {}
void StatsPlugin::onFrameDataNewUniqueFrame(int frameIdx, const rfcommon::Frame<4>& frame) {}
