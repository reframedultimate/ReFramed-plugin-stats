#pragma once

#include "stats/listeners/SettingsListener.hpp"
#include "rfcommon/FrameDataListener.hpp"
#include "rfcommon/MetaDataListener.hpp"
#include "rfcommon/ListenerDispatcher.hpp"
#include "rfcommon/RealtimePlugin.hpp"
#include "rfcommon/Reference.hpp"
#include <memory>

namespace rfcommon {
    class FrameData;
    class MappingInfo;
    class MetaData;
}

class StatsCalculator;
class SettingsModel;
class UserLabelsModel;

class StatsPlugin 
    : public rfcommon::RealtimePlugin
    , public rfcommon::MetaDataListener
    , public rfcommon::FrameDataListener
    , public SettingsListener
{
public:
    StatsPlugin(RFPluginFactory* factory);
    ~StatsPlugin();

    /*!
     * This is called by ReFramed to create an instance of your view.
     * It is possible that this gets called more than once, for example if
     * ReFramed wants to add your view to different parts of the program.
     * Your view should be designed in a way such that multiple views can
     * share the same underlying model.
     */
    QWidget* createView() override;

    /*!
     * The counter-part to createView(). When ReFramed removes your view
     * it will give it back to you to destroy.
     */
    void destroyView(QWidget* view) override;

private:
    void exportEmptyStats() const;
    void exportStats(const rfcommon::MappingInfo* map, const rfcommon::MetaData* mdata) const;

private:
    // These get called by the main application when connecting/disconnecting
    // to the Nintendo Switch. Typically you don't really need these,
    // but it might be interesting to show the status of the connection somewhere.
    void onProtocolAttemptConnectToServer(const char* ipAddress, uint16_t port) override;
    void onProtocolFailedToConnectToServer(const char* errormsg, const char* ipAddress, uint16_t port) override;
    void onProtocolConnectedToServer(const char* ipAddress, uint16_t port) override;
    void onProtocolDisconnectedFromServer() override;

    // These get called when a new game starts/ends, or if a new training mode session starts/ends.
    void onProtocolTrainingStarted(rfcommon::Session* training) override;
    void onProtocolTrainingResumed(rfcommon::Session* training) override;
    void onProtocolTrainingReset(rfcommon::Session* oldTraining, rfcommon::Session* newTraining) override;
    void onProtocolTrainingEnded(rfcommon::Session* training) override;
    void onProtocolGameStarted(rfcommon::Session* game) override;
    void onProtocolGameResumed(rfcommon::Session* game) override;
    void onProtocolGameEnded(rfcommon::Session* game) override;

private:
    // These get called when ReFramed loads/unloads a replay file
    void onGameSessionLoaded(rfcommon::Session* game) override;
    void onGameSessionUnloaded(rfcommon::Session* game) override;
    void onTrainingSessionLoaded(rfcommon::Session* training) override;
    void onTrainingSessionUnloaded(rfcommon::Session* training) override;

    // These get called when the user selects multiple replay files in ReFramed and 
    // loads them
    void onGameSessionSetLoaded(rfcommon::Session** games, int numGames) override;
    void onGameSessionSetUnloaded(rfcommon::Session** games, int numGames) override;

private:
    void onMetaDataTimeStartedChanged(rfcommon::TimeStamp timeStarted) override;
    void onMetaDataTimeEndedChanged(rfcommon::TimeStamp timeEnded) override;

    // Game related events
    void onMetaDataPlayerNameChanged(int fighterIdx, const rfcommon::SmallString<15>& name) override;
    void onMetaDataSetNumberChanged(rfcommon::SetNumber number) override;
    void onMetaDataGameNumberChanged(rfcommon::GameNumber number) override;
    void onMetaDataSetFormatChanged(const rfcommon::SetFormat& format) override;
    void onMetaDataWinnerChanged(int winnerPlayerIdx) override;

    // In training mode this increments every time a new training room is loaded
    void onMetaDataTrainingSessionNumberChanged(rfcommon::GameNumber number) override;

private:
    // These get called whenever a new frame is received (during an active game)
    void onFrameDataNewUniqueFrame(int frameIdx, const rfcommon::Frame<4>& frame) override;
    void onFrameDataNewFrame(int frameIdx, const rfcommon::Frame<4>& frame) override;

private:
    // The export code is implemented in these callbacks
    void onSettingsStatTypesChanged() override;
    void onSettingsOBSChanged() override;

private:
    rfcommon::Reference<rfcommon::MappingInfo> mappingInfo_;
    rfcommon::Reference<rfcommon::MetaData> metaData_;
    rfcommon::Reference<rfcommon::FrameData> frameData_;
    std::unique_ptr<StatsCalculator> statsCalculator_;
    std::unique_ptr<SettingsModel> settingsModel_;
    std::unique_ptr<UserLabelsModel> motionLabels_;
};
