#pragma once

#include "stats/StatType.hpp"
#include <QString>

class StatsCalculator;

class StatsFormatter
{
public:
    StatsFormatter(const StatsCalculator* stats);

    QString playerStatAsString(int fighterIdx, StatType type) const;

private:
    const StatsCalculator* stats_;
};
