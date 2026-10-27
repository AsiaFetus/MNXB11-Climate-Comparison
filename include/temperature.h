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

std::vector<std::vector<std::string>> parseTemperatureCSV(
    const std::string& filename
);

struct TemperatureProfile {
    std::vector<int> days;
    std::vector<double> meanValues;
    std::vector<double> standardDeviations;
};

TemperatureProfile calculateMeanTemperatureByDayOfYear(
    const std::vector<DailyTemperature>& dailyData
);

#endif
