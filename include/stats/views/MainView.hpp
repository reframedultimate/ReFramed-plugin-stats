#pragma once

#include <QWidget>

class PlayerMeta;
class StatsCalculator;
class SettingsModel;
class UserLabelsModel;

class MainView : public QWidget
{
public:
    explicit MainView(PlayerMeta* playerMeta, StatsCalculator* statsModel, SettingsModel* settingsModel, UserLabelsModel* labels, QWidget* parent=nullptr);
};
