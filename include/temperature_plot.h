#ifndef TEMPERATURE_PLOT_H
#define TEMPERATURE_PLOT_H

#include "temperature.h"

#include <vector>

void plotSeasonalTemperature(
    const TemperatureProfile& lundProfile,
    const TemperatureProfile& umeaProfile
);

void plotTemperatureAllYears(
    const std::vector<DailyTemperature>& dailyData,
    const TemperatureProfile& averageProfile,
    const std::string& city
);

#endif
