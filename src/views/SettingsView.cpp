#include "stats/views/SettingsView.hpp"
#include "stats/views/SettingsDragWidget.hpp"

#include <QVBoxLayout>

// ----------------------------------------------------------------------------
SettingsView::SettingsView(SettingsModel* model, QWidget* parent)
    : QWidget(parent)
    , settings_(model)
{
    setLayout(new QVBoxLayout);
    layout()->addWidget(new SettingsDragWidget);
    layout()->addWidget(new SettingsDragWidget);
}

// ----------------------------------------------------------------------------
SettingsView::~SettingsView()
{
}
