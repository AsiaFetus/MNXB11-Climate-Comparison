#ifndef SUNSHINE_PLOT_H
#define SUNSHINE_PLOT_H

#include "sunshine.h"

void plotSunshineAllYears(
    const std::vector<std::vector<std::string>>& data,
    const SunshineProfile& averageProfile,
    const std::string& city
);

void plotSunshineComparison(
    const SunshineProfile& profile1,
    const SunshineProfile& profile2,
    const std::string& city1,
    const std::string& city2
);

void plotYearlySunshineComparison(
    const SunshineProfile& profile1,
    const SunshineProfile& profile2,
    const std::string& city1,
    const std::string& city2
);
#endif 