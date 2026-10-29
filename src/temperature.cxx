#include "temperature.h"

#include <fstream>
#include <map>
#include <sstream>
#include <stdexcept>
#include <string>
#include <vector>

// Read the temperature CSV file

std::vector<std::vector<std::string>> parseTemperatureCSV(
    const std::string& filename
) {
    std::ifstream file(filename);

    // Stop if the file cannot be opened
    if (!file.is_open()) {
        throw std::runtime_error(
            "Could not open temperature file: " + filename
        );
    }

    std::vector<std::vector<std::string>> rows;
    std::string line;

    // Skip the first line because it contains the column names
    std::getline(file, line);

    // Read the file one line at a time
    while (std::getline(file, line)) {
        if (line.empty()) {
            continue;
        }

        std::stringstream ss(line);
        std::string date, time, temperature, quality;

        // Separate the four CSV columns
        std::getline(ss, date, ',');
        std::getline(ss, time, ',');
        std::getline(ss, temperature, ',');
        std::getline(ss, quality, ',');

        rows.push_back({date, time, temperature, quality});
    }

    return rows;
}

//Calculate one mean temperature per day

std::vector<DailyTemperature> calculateDailyMean(
    const std::vector<std::vector<std::string>>& data,
    int firstYear,
    int lastYear
) {
    // Temporary information collected for each date
    struct DayData {
        double sum = 0.0;
        int nObservations = 0;
        int nGood = 0;
        int nYellow = 0;
    };

    //Observations from the same date collected together
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

        if (year < firstYear || year > lastYear) {
            continue;
        }

        double temperature;
        try {
            temperature = std::stod(row[2]);
        } catch (...) {
            continue;
        }

        const std::string& quality = row[3];
        
        DayData& day = dailyData[date];
        day.sum += temperature;
        day.nObservations++;

        // Count observations by quality
        if (quality == "G") {
            day.nGood++;
        } else if (quality == "Y") {
            day.nYellow++;
        }
    }

    std::vector<DailyTemperature> result;

    // Calculate the mean for each date    
    for (const auto& entry : dailyData) {
        const std::string& date = entry.first;
        const DayData& day = entry.second;

        // Ignore dates with fewer than two observations
        if (day.nObservations < 2) {
            continue;
        }
        
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

//Calculate the seasonal temperature profile

TemperatureProfile calculateMeanTemperatureByDayOfYear(
    const std::vector<DailyTemperature>& dailyData
) {
    // Each position stores the daily means for one calendar day
    std::vector<std::vector<double>> values(365);

    // Number of days before each month in a non-leap year.
    const int daysBeforeMonth[] = {
        0, 31, 59, 90, 120, 151,
        181, 212, 243, 273, 304, 334
    };
    
    for (const auto& day : dailyData) {
        int year, month, dayOfMonth;

        // Extract the year, month and day from YYYY-MM-DD
        try {
            year = std::stoi(day.date.substr(0, 4));
            month = std::stoi(day.date.substr(5, 2));
            dayOfMonth = std::stoi(day.date.substr(8, 2));
        } catch (...) {
            continue;
        }

        // Avoid invalid month values before accessing the array.
        if (month < 1 || month > 12 ||
            dayOfMonth < 1 || dayOfMonth > 31) {
            continue;
        }

        // Convert the date into a day number (1-365)
        int dayOfYear =
            daysBeforeMonth[month - 1] + dayOfMonth;

        // Check whether the year contains February 29
        bool leapYear =
            (year % 4 == 0 && year % 100 != 0) ||
            (year % 400 == 0);

        // Exclude February 29 to use a common 365-day calendar
        if (leapYear && month == 2 && dayOfMonth == 29) {
            continue;
        }

        // Shift dates after February 29 back by one day
        if (leapYear && month > 2) {
            dayOfYear--;
        }

        int index = dayOfYear - 1;

        if (index < 0 || index >= 365) {
            continue;
        }

        // Collect the daily mean for this calendar day across all years
        values[index].push_back(day.meanTemperature);
    }

    TemperatureProfile result;

    // Calculate the mean temperature for each day of the year
    for (int i = 0; i < 365; ++i) {
        // Skip days without observations
        if (values[i].empty()) {
        continue;
        }

        result.days.push_back(i + 1);

        // Calculate the average temperature
        double sum = 0.0;
        
        for (double temperature : values[i]) {
            sum += temperature;
        }

        double mean = sum / values[i].size();
        result.meanValues.push_back(mean);
    }

    return result;
}

