#pragma once

#include "stats/models/StatisticType.hpp"
#include <QWidget>

class SettingsModel;
class SettingsDragWidget;

namespace Ui {
    class SettingsView;
}

class SettingsView : public QWidget
{
    Q_OBJECT

public:
    explicit SettingsView(SettingsModel* model, QWidget* parent=nullptr);
    ~SettingsView();

private slots:
    void onOBSCheckBoxToggled(bool enable);
    void onOBSInsertNewLinesCheckBoxToggled(bool enable);
    void onOBSBrowseFolderButtonReleased();
    void onStatEnabled(int insertIndex, StatisticType type);
    void onStatDisabled(int insertIndex, StatisticType type);

private:
    Ui::SettingsView* ui_;
    SettingsModel* settings_;
    SettingsDragWidget* enabledStats_;
    SettingsDragWidget* disabledStats_;
};
