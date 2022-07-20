#pragma once

#include <QWidget>

class StatsCalculator;
class SettingsModel;

class MainView : public QWidget
{
public:
    explicit MainView(StatsCalculator* statsModel, SettingsModel* settingsModel, QWidget* parent=nullptr);
};
