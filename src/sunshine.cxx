#include "sunshine.h"

#include <ctime>
#include <iostream>

SunshineQuality analyzeSunshineData(const std::vector<std::vector<std::string>>& data) {
    SunshineQuality quality; // Initialize a SunshineQuality struct to hold the counts of different quality categories

    for (const auto& row : data) {
        quality.total++; // Increment the total count of observations

        int year = std::stoi(row[0]); // Convert the year string to an integer
        int day = std::stoi(row[1]); // Convert the day to an integer 
        double sunshine_value = std::stod(row[3]); // Convert the sunshine value string to a double
        std::string quality_str = row[4]; // Get the quality string

        if (quality_str == "G") {
            quality.good++; // Increment the count of good quality observations
        } else if (quality_str == "Y") {
            quality.yellow++; // Increment the count of yellow quality observations
            quality.yellowYears.push_back(year); // Store the year of the yellow observation
            quality.yellowDays.push_back(day); // Store the day of the yellow observation
        } else {
            quality.other++; // Increment the count of other quality observations
        }

        if (sunshine_value < 0) {
            quality.negative++; // Increment the count of negative sunshine values
        }
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

    if (!quality.yellowYears.empty()) { // Check if there are any yellow quality observations

        int firstYear = quality.yellowYears.front(); // Get the first year with yellow quality observations
        int lastYear = quality.yellowYears.back(); // Get the last year with yellow quality observations

        for (int year = firstYear; year <= lastYear; year++) {

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