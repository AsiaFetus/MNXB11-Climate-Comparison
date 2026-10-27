#ifndef TEMPERATURE_H
#define TEMPERATURE_H

#include <string>
#include <vector>

struct DailyTemperature {
    std::string date;
    double meanTemperature;
    int nObservations;
    int nGood;
    int nYellow;
};

std::vector<DailyTemperature> calculateDailyMean(
    const std::vector<std::vector<std::string>>& data,
    int firstYear,
    int lastYear
);

#endif