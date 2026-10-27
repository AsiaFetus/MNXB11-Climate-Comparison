#include "temperature.h"

#include <cmath>
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

TemperatureProfile calculateMeanTemperatureByDayOfYear(
    const std::vector<DailyTemperature>& dailyData
) {
    std::vector<std::vector<double>> values(365);

    for (const auto& day : dailyData) {
        int year;
        int month;
        int dayOfMonth;

        try {
            year = std::stoi(day.date.substr(0, 4));
            month = std::stoi(day.date.substr(5, 2));
            dayOfMonth = std::stoi(day.date.substr(8, 2));
        } catch (...) {
            continue;
        }

        static const int daysBeforeMonth[] = {
            0, 31, 59, 90, 120, 151,
            181, 212, 243, 273, 304, 334
        };

        int dayOfYear =
            daysBeforeMonth[month - 1] + dayOfMonth;

        bool leapYear =
            (year % 4 == 0 && year % 100 != 0) ||
            (year % 400 == 0);

        // Ignore February 29.
        if (leapYear && month == 2 && dayOfMonth == 29) {
            continue;
        }

        // Shift dates after February 29 back by one day.
        if (leapYear && month > 2) {
            dayOfYear--;
        }

        int index = dayOfYear - 1;

        if (index < 0 || index >= 365) {
            continue;
        }

        values[index].push_back(day.meanTemperature);
    }

    TemperatureProfile result;

    for (int i = 0; i < 365; ++i) {
        result.days.push_back(i + 1);

        if (values[i].empty()) {
            result.meanValues.push_back(0.0);
            result.standardDeviations.push_back(0.0);
            continue;
        }

        double sum = 0.0;

        for (double temperature : values[i]) {
            sum += temperature;
        }

        double mean = sum / values[i].size();

        double squaredDifferenceSum = 0.0;

        for (double temperature : values[i]) {
            double difference = temperature - mean;
            squaredDifferenceSum += difference * difference;
        }

        double standardDeviation = 0.0;

        if (values[i].size() > 1) {
            standardDeviation =
                std::sqrt(
                    squaredDifferenceSum /
                    (values[i].size() - 1)
                );
        }

        result.meanValues.push_back(mean);
        result.standardDeviations.push_back(standardDeviation);
    }

    return result;
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
