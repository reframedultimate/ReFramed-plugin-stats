#pragma once

#include "stats/StatType.hpp"
#include <QString>

class StatsCalculator;
class UserLabelsModel;

class StatsFormatter
{
public:
    StatsFormatter(const StatsCalculator* stats, const UserLabelsModel* userLabels);

    QString playerStatAsString(int fighterIdx, StatType type) const;

private:
    const StatsCalculator* stats_;
    const UserLabelsModel* labels_;
};
