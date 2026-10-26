#include "sunshine.h"

#include <ctime>
#include <iostream>

#include <vector>
#include <string>

SunshineQuality analyzeSunshineData(
    const std::vector<std::vector<std::string>>& data
) {
    SunshineQuality quality; // Initialize constructor

    for (const auto& row : data) { // Loop through each row of the data
        quality.total++;

        if (row[4] == "G") {
            quality.good++;
        } else if (row[4] == "Y") {
            quality.yellow++;
            // Store the year of the yellow observation
            quality.yellowYears.push_back(std::stoi(row[0]));
            // Store the day of the yellow observation
            quality.yellowDays.push_back(std::stoi(row[1]));
        } else {
            quality.other++;
        }

        if (std::stod(row[3]) < 0) // Check for negative sunshine values
            quality.negative++;
    }

    return quality;
}

void printSunshineQuality(const SunshineQuality& quality) {
    std::cout << "\n" << "Total observations: " << quality.total << std::endl;
    std::cout << "Good quality observations: " << quality.good << std::endl;
    std::cout << "Yellow quality observations: " << quality.yellow << std::endl;
    std::cout << "Other quality observations: " << quality.other << std::endl;
    std::cout << "Negative sunshine values: " << quality.negative << std::endl;
    std::cout << "\n" << "Yellow quality observations by year and day:" << std::endl;

    if (!quality.yellowYears.empty()) { // Check for any yellow observations

        int firstYear = quality.yellowYears.front(); // First year with yellow observations
        int lastYear = quality.yellowYears.back(); // Last year with yellow observations

        for (int year = firstYear; year <= lastYear; year++) { // Loop through each year in the range of yellow observations

            int one = 0;
            int twoToFive = 0;
            int moreThanFive = 0;

            // Check every possible day of the year
            for (int day = 1; day <= 366; day++) {

                int count = 0;

                // Count yellow observations on this day
                // Loop through all yellow observations years (one value per yellow observation, repeated years)
                for (size_t i = 0; i < quality.yellowYears.size(); i++) {  
                    if (quality.yellowYears[i] == year &&
                        quality.yellowDays[i] == day) {
                        count++;
                    }
                }

                // Classify the day
                if (count == 1) {
                    one++;
                } else if (count >= 2 && count <= 5) {
                    twoToFive++;
                } else if (count > 5) {
                    moreThanFive++;
                }
            }

            if (one + twoToFive + moreThanFive > 0) {
                std::cout << "Year " << year << ": "
                        << one << " independent days with 1 yellow, "
                        << twoToFive << " independent days with 2-5 yellow, "
                        << moreThanFive << " independent days with >5 yellow\n";
            }
        }
    }
}


std::vector<std::vector<double>> sunshineByYearAndDay(
    const std::vector<std::vector<std::string>>& data
) {
    int firstYear = std::stoi(data.front()[0]); 
    int numberOfYears = std::stoi(data.back()[0]) - firstYear + 1; // Last year - First year + 1 to include both endpoints

    std::vector<std::vector<double>> irradiation(
        numberOfYears, std::vector<double>(366, 0.0)
    ); // Initialize a 2D vector to store daily irradiation for each year

    for (const auto& row : data) { // Loop through each row of the data
        int year = std::stoi(row[0]);
        int day = std::stoi(row[1]);
        double value = std::stod(row[3]);

        // Check if the year is a leap year
        bool leap = (year % 4 == 0 &&
                     (year % 100 != 0 || year % 400 == 0));

        // Remove Feb 29 and shift later days
        if (leap && day == 60) continue;
        if (leap && day > 60) day--;

        irradiation[year - firstYear][day] += value; // Store the daily irradiation value in the corresponding year and day
    }

    return irradiation;
}

SunshineProfile averageSunshineByDay(
    const std::vector<std::vector<std::string>>& data
) {
    auto irradiation = sunshineByYearAndDay(data);
    int numberOfYears = irradiation.size();

    SunshineProfile result; // Initialize a SunshineProfile to store the average sunshine by day

    // Calculate the average sunshine for each day
    for (int day = 1; day <= 365; day++) {
        double total = 0;

        for (int year = 0; year < numberOfYears; year++)
            total += irradiation[year][day];

        // Last year only includes January to July
        int yearsToAverage = numberOfYears - (day > 182);

        result.days.push_back(day);
        result.meanValues.push_back(total / yearsToAverage);
    }

    return result;
}
SunshineProfile averageSunshineByYear(
    const std::vector<std::vector<std::string>>& data
) {
    auto irradiation = sunshineByYearAndDay(data);
    int firstYear = std::stoi(data.front()[0]);

    SunshineProfile result;

    // Loop thorough each year and calculate the total sunshine for that year
    for (size_t year = 0; year < irradiation.size(); year++) {
        double total = 0.0;

        // Calculate the total sunshine for the year
        for (int day = 1; day <= 365; day++)
            total += irradiation[year][day];

        int actualYear = firstYear + year;

        result.days.push_back(actualYear);
        result.meanValues.push_back(
            total / (actualYear == 2026 ? 4351.0 : 8760.0) // Use 4351 hours for 2026 (Jan-Jul) and 8760 hours for other years
        );
    }

    return result;
}