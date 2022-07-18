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
    void onStatEnabled(StatisticType type);
    void onStatDisabled(StatisticType type);

private:
    Ui::SettingsView* ui_;
    SettingsModel* settings_;
    SettingsDragWidget* enabledStats_;
    SettingsDragWidget* disabledStats_;
};
