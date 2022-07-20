#include "ui_SettingsView.h"
#include "stats/views/SettingsView.hpp"
#include "stats/views/SettingsDragWidget.hpp"
#include "stats/models/SettingsModel.hpp"

#include <QVBoxLayout>
#include <QFileDialog>

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

    connect(ui_->checkBox_obsExport, &QCheckBox::toggled, this, &SettingsView::onOBSCheckBoxToggled);
    connect(ui_->checkBox_obsInsertNewlines, &QCheckBox::toggled, this, &SettingsView::onOBSInsertNewLinesCheckBoxToggled);
    connect(ui_->toolButton_obsBrowseFolder, &QToolButton::released, this, &SettingsView::onOBSBrowseFolderButtonReleased);
    connect(enabledStats_, &SettingsDragWidget::statAdded, this, &SettingsView::onStatEnabled);
    connect(disabledStats_, &SettingsDragWidget::statAdded, this, &SettingsView::onStatDisabled);
}

// ----------------------------------------------------------------------------
SettingsView::~SettingsView()
{
    delete ui_;
}

// ----------------------------------------------------------------------------
void SettingsView::onOBSCheckBoxToggled(bool enable)
{
    ui_->label_obsDestFolder->setEnabled(enable);
    ui_->lineEdit_obsDestFolder->setEnabled(enable);
    ui_->toolButton_obsBrowseFolder->setEnabled(enable);
    ui_->checkBox_obsInsertNewlines->setEnabled(enable);
    ui_->spinBox_obsNewlines->setEnabled(enable && ui_->checkBox_obsInsertNewlines->isChecked());

    settings_->setExportToOBS(enable);
    settings_->setAdditionalNewlinesOBS(ui_->checkBox_obsInsertNewlines->isChecked() ? ui_->spinBox_obsNewlines->value() : 0);
}

// ----------------------------------------------------------------------------
void SettingsView::onOBSInsertNewLinesCheckBoxToggled(bool enable)
{
    ui_->spinBox_obsNewlines->setEnabled(enable);

    settings_->setAdditionalNewlinesOBS(enable ? ui_->spinBox_obsNewlines->value() : 0);
}

// ----------------------------------------------------------------------------
void SettingsView::onOBSBrowseFolderButtonReleased()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Destination Folder");
    ui_->lineEdit_obsDestFolder->setText(dir);

    settings_->setDestinationFolderOBS(dir);
}

// ----------------------------------------------------------------------------
void SettingsView::onStatEnabled(int insertIndex, StatisticType type)
{
    settings_->setStatEnabled(type, true);
    settings_->setStatAtIndex(insertIndex, type);
}

// ----------------------------------------------------------------------------
void SettingsView::onStatDisabled(int insertIndex, StatisticType type)
{
    settings_->setStatEnabled(type, false);
}
