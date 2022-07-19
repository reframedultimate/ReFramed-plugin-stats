#include "stats/views/MainView.hpp"
#include "stats/views/StatsView.hpp"
#include "stats/views/SettingsView.hpp"

#include <QVBoxLayout>
#include <QTabWidget>

// ----------------------------------------------------------------------------
MainView::MainView(StatsModel* statsModel, SettingsModel* settingsModel, QWidget* parent)
{
    setLayout(new QVBoxLayout);

    QTabWidget* tabWidget = new QTabWidget;
    tabWidget->addTab(new StatsView(statsModel, settingsModel), "Statistics");
    tabWidget->addTab(new SettingsView(settingsModel), "Settings");
    layout()->addWidget(tabWidget);
}
