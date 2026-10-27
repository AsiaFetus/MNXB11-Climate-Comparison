#include "temperature.h"

#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

std::vector<std::vector<std::string>> parseTemperatureCSV(
    const std::string& filename
) {
    std::ifstream file(filename);

    if (!file.is_open()) {
        throw std::runtime_error(
            "Could not open temperature file: " + filename
        );
    }

    std::vector<std::vector<std::string>> rows;
    std::string line;

    // Skip the header
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string date;
        std::string time;
        std::string temperature;
        std::string quality;

        std::getline(ss, date, ',');
        std::getline(ss, time, ',');
        std::getline(ss, temperature, ',');
        std::getline(ss, quality, ',');

        rows.push_back({date, time, temperature, quality});
    }

    return rows;
}

std::vector<DailyTemperature> calculateDailyMean(
    const std::vector<std::vector<std::string>>& data,
    int firstYear,
    int lastYear
) {
    struct DayData {
        double sum = 0.0;
        int nObservations = 0;
        int nGood = 0;
        int nYellow = 0;
    };

    std::map<std::string, DayData> dailyData;

    for (const auto& row : data) {

        if (row.size() < 4)
            continue;

        const std::string& date = row[0];

        int year;

        try {
            year = std::stoi(date.substr(0, 4));
        } catch (...) {
            continue;
        }

        if (year < firstYear || year > lastYear)
            continue;

        const std::string& temperatureString = row[2];
        const std::string& quality = row[3];

        double temperature;

        try {
            temperature = std::stod(temperatureString);
        } catch (...) {
            continue;
        }

        DayData& day = dailyData[date];

        day.sum += temperature;
        day.nObservations++;

        if (quality == "G")
            day.nGood++;
        else if (quality == "Y")
            day.nYellow++;
    }

    std::vector<DailyTemperature> result;

    for (const auto& entry : dailyData) {

        const std::string& date = entry.first;
        const DayData& day = entry.second;

        if (day.nObservations < 2)
            continue;

        DailyTemperature dailyTemperature;

        dailyTemperature.date = date;
        dailyTemperature.meanTemperature =
            day.sum / day.nObservations;
        dailyTemperature.nObservations = day.nObservations;
        dailyTemperature.nGood = day.nGood;
        dailyTemperature.nYellow = day.nYellow;

        result.push_back(dailyTemperature);
    }

    return result;
}