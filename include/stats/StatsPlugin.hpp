#pragma once

#include "stats/listeners/SettingsListener.hpp"
#include "rfcommon/RealtimePlugin.hpp"
#include "rfcommon/SessionListener.hpp"
#include "rfcommon/ListenerDispatcher.hpp"
#include "rfcommon/Reference.hpp"
#include <memory>

namespace rfcommon {
    class Session;
}

class StatsCalculator;
class SettingsModel;
class UserLabelsModel;

class StatsPlugin : public rfcommon::RealtimePlugin
                  , public rfcommon::SessionListener
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
    void exportStats(const rfcommon::Session* session) const;

private:
    // These get called by the main application when connecting/disconnecting
    // to the Nintendo Switch. Typically you don't really need these,
    // but it might be interesting to show the status of the connection somewhere.
    void onProtocolAttemptConnectToServer(const char* ipAddress, uint16_t port) override {}
    void onProtocolFailedToConnectToServer(const char* errormsg, const char* ipAddress, uint16_t port) override {}
    void onProtocolConnectedToServer(const char* ipAddress, uint16_t port) override {}
    void onProtocolDisconnectedFromServer() override {}

    // These get called when a new game starts/ends, or if a new training mode session starts/ends.
    void onProtocolTrainingStarted(rfcommon::RunningTrainingSession* session) override {}
    void onProtocolTrainingResumed(rfcommon::RunningTrainingSession* session) override {}
    void onProtocolTrainingReset(rfcommon::RunningTrainingSession* oldSession, rfcommon::RunningTrainingSession* newSession) override {}
    void onProtocolTrainingEnded(rfcommon::RunningTrainingSession* session) override {}
    void onProtocolMatchStarted(rfcommon::RunningGameSession* session) override;
    void onProtocolMatchResumed(rfcommon::RunningGameSession* session) override;
    void onProtocolMatchEnded(rfcommon::RunningGameSession* session) override;

private:
    // These get called when ReFramed loads/unloads a replay file
    void setSavedGameSession(rfcommon::SavedGameSession* session) override;
    void clearSavedGameSession(rfcommon::SavedGameSession* session) override {}

private:
    // RunningGameSession events
    void onRunningGameSessionPlayerNameChanged(int playerIdx, const rfcommon::SmallString<15> & name) override {}
    void onRunningGameSessionSetNumberChanged(rfcommon::SetNumber number) override {}
    void onRunningGameSessionGameNumberChanged(rfcommon::GameNumber number) override {}
    void onRunningGameSessionFormatChanged(const rfcommon::SetFormat& format) override {}
    void onRunningGameSessionWinnerChanged(int winnerPlayerIdx) override {}

    // RunningSession events
    void onRunningSessionNewUniquePlayerState(int playerIdx, const rfcommon::PlayerState& state) override {}
    void onRunningSessionNewPlayerState(int playerIdx, const rfcommon::PlayerState& state) override {}
    void onRunningSessionNewUniqueFrame(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states) override {}
    void onRunningSessionNewFrame(const rfcommon::SmallVector<rfcommon::PlayerState, 8>& states) override;

private:
    // The export code is implemented in these callbacks
    void onSettingsStatTypesChanged() override;
    void onSettingsOBSChanged() override;

private:
    rfcommon::Reference<rfcommon::Session> session_;
    std::unique_ptr<StatsCalculator> statsCalculator_;
    std::unique_ptr<SettingsModel> settingsModel_;
    std::unique_ptr<UserLabelsModel> motionLabels_;
};
