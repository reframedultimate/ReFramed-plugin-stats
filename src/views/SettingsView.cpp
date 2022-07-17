#include "stats/views/SettingsView.hpp"

// ----------------------------------------------------------------------------
SettingsView::SettingsView(SettingsModel* model, QWidget* parent)
    : QWidget(parent)
    , settings_(model)
{
}

// ----------------------------------------------------------------------------
SettingsView::~SettingsView()
{
}
