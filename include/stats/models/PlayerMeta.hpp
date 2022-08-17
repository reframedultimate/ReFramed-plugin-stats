#pragma once

#include "rfcommon/ListenerDispatcher.hpp"
#include "rfcommon/MetaDataListener.hpp"
#include "rfcommon/Reference.hpp"
#include "rfcommon/FighterMotion.hpp"
#include <QString>

class PlayerMetaListener;

namespace rfcommon {
    class Hash40Strings;
    class MappingInfo;
    class MetaData;
    class UserMotionLabels;
}

class PlayerMeta : public rfcommon::MetaDataListener
{
public:
    PlayerMeta(rfcommon::UserMotionLabels* userLabels, rfcommon::Hash40Strings* hash40Strings);
    ~PlayerMeta();

    void setMetaData(rfcommon::MappingInfo* map, rfcommon::MetaData* mdata);
    void clearMetaData();

    QString name(int fighterIdx) const;
    QString character(int fighterIdx) const;
    QString moveName(int fighterIdx, rfcommon::FighterMotion motion) const;

    rfcommon::ListenerDispatcher<PlayerMetaListener> dispatcher;

private:
    void onMetaDataTimeStartedChanged(rfcommon::TimeStamp timeStarted) override;
    void onMetaDataTimeEndedChanged(rfcommon::TimeStamp timeEnded) override;

    // Game related events
    void onMetaDataPlayerNameChanged(int fighterIdx, const rfcommon::String& name) override;
    void onMetaDataSetNumberChanged(rfcommon::SetNumber number) override;
    void onMetaDataGameNumberChanged(rfcommon::GameNumber number) override;
    void onMetaDataSetFormatChanged(const rfcommon::SetFormat& format) override;
    void onMetaDataWinnerChanged(int winnerPlayerIdx) override;

    // In training mode this increments every time a new training room is loaded
    void onMetaDataTrainingSessionNumberChanged(rfcommon::GameNumber number) override;

private:
    struct Player
    {
        QString name;
        QString character;
    };

    rfcommon::Reference<rfcommon::MetaData> metaData_;
    rfcommon::Reference<rfcommon::UserMotionLabels> userLabels_;
    rfcommon::Reference<rfcommon::Hash40Strings> hash40Strings_;
    rfcommon::SmallVector<Player, 2> players_;
};
