#include "sunshine.h"

#include <ctime>
#include <iostream>

#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>

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


SunshineProfile averageSunshineByDay(
    const std::vector<std::vector<std::string>>& data
) {
    // Get the first and last year from the data
    int firstYear = std::stoi(data.front()[0]);
    int lastYear = std::stoi(data.back()[0]);
    int numberOfYears = lastYear - firstYear + 1;

    // Create a 2D vector to store daily irradiation for each year
    std::vector<std::vector<double>> irradiation(
        numberOfYears, std::vector<double>(366, 0.0)
    );

    // Go through every observation
    for (const auto& row : data) {
        int year = std::stoi(row[0]);
        int day = std::stoi(row[1]);
        double value = std::stod(row[3]);

        // Check if it is a leap year
        bool leap = (year % 4 == 0 &&
                     (year % 100 != 0 || year % 400 == 0));

        // Remove February 29 and adjust following days
        if (leap && day == 60) continue;
        if (leap && day > 60) day--;

        if (day < 1 || day > 365) continue;

        // Convert the year to an index starting from 0
        int yearIndex = year - firstYear;

        // Add the hourly irradiance to that year's daily total
        irradiation[yearIndex][day] += value;
    }

    SunshineProfile result;

    // Calculate the average irradiation for each day of the year
    for (int day = 1; day <= 365; day++) {
        double total = 0.0;

        for (int year = 0; year < numberOfYears; year++) {
            total += irradiation[year][day];
        }

        // July 1 is day 182 in a non-leap year
        int yearsToAverage = numberOfYears;

        if (day > 182) {
            yearsToAverage--;
        }

        result.days.push_back(day);
        result.meanValues.push_back(
            total / yearsToAverage
        );
    }
    return result;
}


void plotSunshineHistogram(
    const SunshineProfile& profile,
    const std::string& city
) {
    TCanvas canvas("canvas", "Sunshine histogram", 1000, 600);

    TH1D hist(
        "hist",
        ("Average daily irradiation in " + city +
         ";Day of year;Daily irradiation (Wh/m^{2})").c_str(),
        365, 0.5, 365.5
    );

    for (size_t i = 0; i < profile.days.size(); i++) {
        int day = static_cast<int>(profile.days[i]);
        hist.SetBinContent(day, profile.meanValues[i]);
    }

    hist.Draw("HIST");

    canvas.SaveAs((city + "_sunshine.pdf").c_str());
}

void plotSunshineComparison(
    const SunshineProfile& profile1,
    const SunshineProfile& profile2,
    const std::string& city1,
    const std::string& city2
) {
    TCanvas canvas("canvas", "Sunshine comparison", 1000, 600);

    TH1D hist1(
        "hist1",
        ("Average daily irradiation;Day of year;Daily irradiation (Wh/m^{2})"),
        365, 0.5, 365.5
    );

    TH1D hist2(
        "hist2",
        ("Average daily irradiation;Day of year;Daily irradiation (Wh/m^{2})"),
        365, 0.5, 365.5
    );

    for (size_t i = 0; i < profile1.days.size(); i++) {
        int day = static_cast<int>(profile1.days[i]);
        hist1.SetBinContent(day, profile1.meanValues[i]);
    }

    for (size_t i = 0; i < profile2.days.size(); i++) {
        int day = static_cast<int>(profile2.days[i]);
        hist2.SetBinContent(day, profile2.meanValues[i]);
    }

    hist1.SetLineColor(kBlue);
    hist1.SetLineWidth(2);

    hist2.SetLineColor(kRed);
    hist2.SetLineWidth(2);

    hist1.Draw("HIST");
    hist2.Draw("HIST SAME");

    TLegend legend(0.75, 0.75, 0.90, 0.90);
    legend.AddEntry(&hist1, city1.c_str(), "l");
    legend.AddEntry(&hist2, city2.c_str(), "l");
    legend.Draw();

    canvas.SaveAs((city1 + "_" + city2 + "_sunshine_comparison.pdf").c_str());
}