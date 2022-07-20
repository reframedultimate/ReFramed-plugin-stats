#pragma once

class SettingsListener
{
public:
    virtual void onSettingsStatTypesChanged() = 0;
    virtual void onSettingsOBSChanged() = 0;
};
