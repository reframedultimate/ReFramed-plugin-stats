#pragma once

#include <QWidget>

class StatsCalculator;
class SettingsModel;
class UserLabelsModel;

class MainView : public QWidget
{
public:
    explicit MainView(StatsCalculator* statsModel, SettingsModel* settingsModel, UserLabelsModel* labels, QWidget* parent=nullptr);
};
