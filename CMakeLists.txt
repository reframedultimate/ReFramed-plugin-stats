cmake_minimum_required (VERSION 3.2)

set (PLUGIN_NAME "stats")

project ("plugin-${PLUGIN_NAME}"
    VERSION 0.0.1
    LANGUAGES CXX)

find_package (ReFramed REQUIRED)
include (ReFramedPlugin)

reframed_add_plugin (plugin-${PLUGIN_NAME}
    FORMS
        "forms/StatsView.ui"
        "forms/SettingsView.ui"
    SOURCES
        "src/models/SettingsModel.cpp"
        "src/models/StatsModel.cpp"
        "src/models/StatisticType.cpp"
        "src/views/MainView.cpp"
        "src/views/SettingsView.cpp"
        "src/views/SettingsDragWidget.cpp"
        "src/views/SettingsStatsItem.cpp"
        "src/views/StatsView.cpp"
        "src/StatsPlugin.cpp"
        "src/Plugin.cpp"
    HEADERS
        "include/${PLUGIN_NAME}/listeners/StatsListener.hpp"
        "include/${PLUGIN_NAME}/listeners/SettingsListener.hpp"
        "include/${PLUGIN_NAME}/models/SettingsModel.hpp"
        "include/${PLUGIN_NAME}/models/StatsModel.hpp"
        "include/${PLUGIN_NAME}/models/StatisticType.hpp"
        "include/${PLUGIN_NAME}/views/MainView.hpp"
        "include/${PLUGIN_NAME}/views/SettingsDragWidget.hpp"
        "include/${PLUGIN_NAME}/views/SettingsStatsItem.hpp"
        "include/${PLUGIN_NAME}/views/SettingsView.hpp"
        "include/${PLUGIN_NAME}/views/StatsView.hpp"
        "include/${PLUGIN_NAME}/StatsPlugin.hpp"
    INCLUDE_DIRECTORIES
        "include"
    MOC_HEADERS
        "include/${PLUGIN_NAME}/views/MainView.hpp"
        "include/${PLUGIN_NAME}/views/SettingsView.hpp"
        "include/${PLUGIN_NAME}/views/StatsView.hpp"
        "include/${PLUGIN_NAME}/views/SettingsDragWidget.hpp"
        "include/${PLUGIN_NAME}/views/SettingsStatsItem.hpp")

if (${REFRAMED_plugin-${PLUGIN_NAME}})
endif ()

