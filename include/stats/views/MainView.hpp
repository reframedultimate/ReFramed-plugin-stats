#pragma once

#include <QWidget>

class StatsModel;
class SettingsModel;

class MainView : public QWidget
{
public:
    explicit MainView(StatsModel* statsModel, SettingsModel* settingsModel, QWidget* parent=nullptr);
};
