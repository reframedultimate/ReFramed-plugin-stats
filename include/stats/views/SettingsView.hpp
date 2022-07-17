#pragma once

#include <QWidget>

class SettingsModel;

class SettingsView : public QWidget
{
    Q_OBJECT
public:
    explicit SettingsView(SettingsModel* model, QWidget* parent=nullptr);
    ~SettingsView();

private:
    SettingsModel* settings_;
};
