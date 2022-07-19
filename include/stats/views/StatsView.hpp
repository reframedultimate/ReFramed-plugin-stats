#pragma once

#include "stats/listeners/StatsListener.hpp"
#include "stats/listeners/SettingsListener.hpp"
#include <QWidget>

// Forward declare the class created by Qt designer
namespace Ui {
    class StatsView;
}

class QGridLayout;
class StatsModel;
class SettingsModel;

class StatsView : public QWidget
                , public StatsListener
                , public SettingsListener
{
    Q_OBJECT

public:
    explicit StatsView(StatsModel* stats, SettingsModel* settings, QWidget* parent=nullptr);
    ~StatsView();

private:
    void onStatsUpdated() override;
    void onSettingsChanged() override;

private:
    // We hold a weak reference to the model (StatsModel) so we can listen
    // to it
    StatsModel* stats_;

    // We also listen to changes to the settings
    SettingsModel* settings_;

    // Holds all of the statistic lables
    QGridLayout* layout_;
};
