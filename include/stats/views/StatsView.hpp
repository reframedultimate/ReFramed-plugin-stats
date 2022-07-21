#pragma once

#include "stats/listeners/StatsCalculatorListener.hpp"
#include "stats/listeners/SettingsListener.hpp"
#include <QWidget>

// Forward declare the class created by Qt designer
namespace Ui {
    class StatsView;
}

class QGridLayout;
class StatsCalculator;
class SettingsModel;
class UserLabelsModel;

class StatsView : public QWidget
                , public StatsCalculatorListener
                , public SettingsListener
{
    Q_OBJECT

public:
    explicit StatsView(StatsCalculator* stats, SettingsModel* settings, UserLabelsModel* labels, QWidget* parent=nullptr);
    ~StatsView();

private:
    void updateStatsLabels();
    void recreateLayout();

private:
    void onStatsUpdated() override;
    void onSettingsStatTypesChanged() override;
    void onSettingsOBSChanged() override {}

private:
    // We hold a weak reference to the model (StatsModel) so we can listen
    // to it
    StatsCalculator* stats_;

    // We also listen to changes to the settings
    SettingsModel* settings_;

    UserLabelsModel* labels_;

    // Holds all of the statistic lables
    QGridLayout* layout_;
};
