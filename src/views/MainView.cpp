#include "stats/views/MainView.hpp"
#include "stats/views/StatsView.hpp"
#include "stats/views/SettingsView.hpp"

#include <QVBoxLayout>
#include <QTabWidget>

// ----------------------------------------------------------------------------
MainView::MainView(PlayerMeta* playerMeta, StatsCalculator* statsModel, SettingsModel* settingsModel, UserLabelsModel* labels, QWidget* parent)
{
    setLayout(new QVBoxLayout);

    QTabWidget* tabWidget = new QTabWidget;
    tabWidget->addTab(new StatsView(playerMeta, statsModel, settingsModel, labels), "Statistics");
    tabWidget->addTab(new SettingsView(settingsModel), "Settings");
    layout()->addWidget(tabWidget);
}
