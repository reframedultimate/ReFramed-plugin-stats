#include "ui_SettingsView.h"
#include "stats/views/SettingsView.hpp"
#include "stats/views/SettingsDragWidget.hpp"

#include <QVBoxLayout>

// ----------------------------------------------------------------------------
SettingsView::SettingsView(SettingsModel* model, QWidget* parent)
    : QWidget(parent)
    , ui_(new Ui::SettingsView)
    , settings_(model)
    , enabledStats_(new SettingsDragWidget)
    , disabledStats_(new SettingsDragWidget)
{
    ui_->setupUi(this);

    for (int i = 0; i != STAT_COUNT; ++i)
        enabledStats_->addStat(static_cast<StatisticType>(i));

    ui_->groupBox_enabledStats->setLayout(new QVBoxLayout);
    ui_->groupBox_disabledStats->setLayout(new QVBoxLayout);
    ui_->groupBox_enabledStats->layout()->addWidget(enabledStats_);
    ui_->groupBox_disabledStats->layout()->addWidget(disabledStats_);

    connect(enabledStats_, &SettingsDragWidget::statAdded, this, &SettingsView::onStatEnabled);
    connect(disabledStats_, &SettingsDragWidget::statAdded, this, &SettingsView::onStatDisabled);
}

// ----------------------------------------------------------------------------
SettingsView::~SettingsView()
{
    delete ui_;
}

// ----------------------------------------------------------------------------
void SettingsView::onStatEnabled(StatisticType type)
{

}

// ----------------------------------------------------------------------------
void SettingsView::onStatDisabled(StatisticType type)
{

}
