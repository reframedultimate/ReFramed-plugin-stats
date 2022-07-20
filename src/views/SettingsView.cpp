#include "ui_SettingsView.h"
#include "stats/models/SettingsModel.hpp"
#include "stats/views/SettingsView.hpp"
#include "stats/widgets/SettingsDragWidget.hpp"

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

    for (int i = 0; i != settings_->numStatsEnabled(); ++i)
    {
        StatType type = settings_->statAtIndex(i);
        enabledStats_->addStat(type);
    }

    for (int i = 0; i != STAT_COUNT; ++i)
    {
        StatType type = static_cast<StatType>(i);
        if (settings_->statEnabled(type) == false)
            disabledStats_->addStat(type);
    }

    ui_->checkBox_obsExport->setChecked(settings_->exportToOBS());
    ui_->label_obsDestFolder->setEnabled(settings_->exportToOBS());
    ui_->toolButton_obsBrowseFolder->setEnabled(settings_->exportToOBS());
    ui_->checkBox_obsInsertNewlines->setEnabled(settings_->exportToOBS());
    ui_->spinBox_obsNewlines->setEnabled(settings_->exportToOBS() && settings_->additionalNewlinesOBS() > 0);

    ui_->lineEdit_obsDestFolder->setText(settings_->destinationFolderOBS().absolutePath());
    ui_->checkBox_obsInsertNewlines->setChecked(settings_->additionalNewlinesOBS() > 0);
    ui_->spinBox_obsNewlines->setValue(settings_->additionalNewlinesOBS());

    ui_->groupBox_enabledStats->setLayout(new QVBoxLayout);
    ui_->groupBox_disabledStats->setLayout(new QVBoxLayout);
    ui_->groupBox_enabledStats->layout()->addWidget(enabledStats_);
    ui_->groupBox_disabledStats->layout()->addWidget(disabledStats_);

    connect(ui_->checkBox_obsExport, &QCheckBox::toggled, this, &SettingsView::onOBSCheckBoxToggled);
    connect(ui_->checkBox_obsInsertNewlines, &QCheckBox::toggled, this, &SettingsView::onOBSInsertNewLinesCheckBoxToggled);
    connect(ui_->spinBox_obsNewlines, qOverload<int>(&QSpinBox::valueChanged), this, &SettingsView::onOBSSpinBoxNewLinesChanged);
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
void SettingsView::onOBSSpinBoxNewLinesChanged(int value)
{
    settings_->setAdditionalNewlinesOBS(value);
}

// ----------------------------------------------------------------------------
void SettingsView::onOBSBrowseFolderButtonReleased()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Destination Folder");
    ui_->lineEdit_obsDestFolder->setText(dir);

    settings_->setDestinationFolderOBS(dir);
}

// ----------------------------------------------------------------------------
void SettingsView::onStatEnabled(int insertIndex, StatType type)
{
    settings_->setStatEnabled(type, true);
    settings_->setStatAtIndex(insertIndex, type);
}

// ----------------------------------------------------------------------------
void SettingsView::onStatDisabled(int insertIndex, StatType type)
{
    settings_->setStatEnabled(type, false);
}
