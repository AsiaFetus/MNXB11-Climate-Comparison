#ifndef TEMPERATURE_PLOT_H
#define TEMPERATURE_PLOT_H

#include "temperature.h"

void plotSeasonalTemperature(
    const TemperatureProfile& lundProfile,
    const TemperatureProfile& umeaProfile
);

#endif
