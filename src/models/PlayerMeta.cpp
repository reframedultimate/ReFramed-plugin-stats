#include "stats/listeners/PlayerMetaListener.hpp"
#include "stats/models/PlayerMeta.hpp"
#include "rfcommon/MappingInfo.hpp"
#include "rfcommon/MetaData.hpp"

// ----------------------------------------------------------------------------
PlayerMeta::PlayerMeta()
{}

// ----------------------------------------------------------------------------
PlayerMeta::~PlayerMeta()
{
    if (metaData_)
        metaData_->dispatcher.removeListener(this);
}

// ----------------------------------------------------------------------------
void PlayerMeta::setMetaData(rfcommon::MappingInfo* map, rfcommon::MetaData* mdata)
{
    clearMetaData();

    players_.resize(mdata->fighterCount());
    for (int i = 0; i != mdata->fighterCount(); ++i)
    {
        players_[i].name = mdata->name(i).cStr();
        players_[i].character = map->fighter.toName(mdata->fighterID(i));
    }

    metaData_ = mdata;
    metaData_->dispatcher.addListener(this);

    dispatcher.dispatch(&PlayerMetaListener::onPlayerMetaChanged);
}

// ----------------------------------------------------------------------------
void PlayerMeta::clearMetaData()
{
    if (metaData_)
        metaData_->dispatcher.removeListener(this);

    players_.clear();
    dispatcher.dispatch(&PlayerMetaListener::onPlayerMetaChanged);
}

// ----------------------------------------------------------------------------
QString PlayerMeta::name(int fighterIdx) const
{
    return players_.count() ?
        players_[fighterIdx].name :
        QString("Player ") + QString::number(fighterIdx + 1);
}

// ----------------------------------------------------------------------------
QString PlayerMeta::character(int fighterIdx) const
{
    return players_.count() ?
        players_[fighterIdx].character :
        "";
}

// ----------------------------------------------------------------------------
void PlayerMeta::onMetaDataPlayerNameChanged(int fighterIdx, const rfcommon::String& name)
{
    players_[fighterIdx].name = name.cStr();
    dispatcher.dispatch(&PlayerMetaListener::onPlayerMetaChanged);
}

// ----------------------------------------------------------------------------
void PlayerMeta::onMetaDataTimeStartedChanged(rfcommon::TimeStamp timeStarted) 
{
}

// ----------------------------------------------------------------------------
void PlayerMeta::onMetaDataTimeEndedChanged(rfcommon::TimeStamp timeEnded) 
{
}

// ----------------------------------------------------------------------------
void PlayerMeta::onMetaDataSetNumberChanged(rfcommon::SetNumber number) 
{
}

// ----------------------------------------------------------------------------
void PlayerMeta::onMetaDataGameNumberChanged(rfcommon::GameNumber number) 
{
}

// ----------------------------------------------------------------------------
void PlayerMeta::onMetaDataSetFormatChanged(const rfcommon::SetFormat& format) 
{
}

// ----------------------------------------------------------------------------
void PlayerMeta::onMetaDataWinnerChanged(int winnerPlayerIdx) 
{
}

// ----------------------------------------------------------------------------
void PlayerMeta::onMetaDataTrainingSessionNumberChanged(rfcommon::GameNumber number)
{
}
