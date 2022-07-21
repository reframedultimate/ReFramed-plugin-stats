#include "stats/views/MainView.hpp"
#include "stats/views/StatsView.hpp"
#include "stats/views/SettingsView.hpp"

#include <QVBoxLayout>
#include <QTabWidget>

// ----------------------------------------------------------------------------
MainView::MainView(StatsCalculator* statsModel, SettingsModel* settingsModel, UserLabelsModel* labels, QWidget* parent)
{
    setLayout(new QVBoxLayout);

    QTabWidget* tabWidget = new QTabWidget;
    tabWidget->addTab(new StatsView(statsModel, settingsModel, labels), "Statistics");
    tabWidget->addTab(new SettingsView(settingsModel), "Settings");
    layout()->addWidget(tabWidget);
}
