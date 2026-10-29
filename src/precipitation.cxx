#include "precipitation.h"
#include "parse_csv.h"
#include <TH1D.h>
#include <TCanvas.h>
#include <csv/csv.h>
#include <vector>
#include <string>
#include <array>
#include <cmath>

std::vector<PrecipitationMeasurement> extract_precipitation_data(const std::string& filename, bool only_good_quality)
{
    // Use external csv reader, <3> specifies number of columns
    io::CSVReader<3> input(filename);

    // Handles header
    input.read_header(io::ignore_extra_column, "Date", "Precipitation_mm", "Quality");

    // Declare variables to store row data
    std::string date;
    double precipitation;
    std::string quality;

    // Vector of measurements to retunr
    std::vector<PrecipitationMeasurement> observations;

    while (input.read_row(date, precipitation, quality))
    {
        if (only_good_quality && quality != "G") // Keep only good if enabled
        { 
            continue; 
        }
        if (date.substr(5, 5) == "02-29") // This date is February 29, substr() starts at i=5, length 5
        {
            continue;
        }

        // Get day number from parse_csv
        int day_num = dayOfYear(date);

        // Get year from string to int
        int year = std::stoi(date.substr(0, 4));

        // Check if leap year with leap year rules
        bool isLeapYear = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

        // Reduce all day numbers after feb 29 by one
        if (isLeapYear && day_num > 60) {
            day_num--;
        }

        // Create one observation object for each row
        PrecipitationMeasurement observation{year, day_num, precipitation};

        // Append observation to vector of measurements
        observations.push_back(observation);
    }
    return observations;
}


DailyStats calculate_day_stats(const std::vector<PrecipitationMeasurement>& data) 
{
    // Create array of 365 vectors to contain precipitation values for each day
    std::array<std::vector<double>, 365> daily_values;

    // Add the precipitation in vectors corresponding to the day
    for (const auto& observation : data) {
        daily_values[observation.day_number - 1].push_back(observation.precipitation_mm);
    }

    DailyStats results;

    // Calculate the stats, mean and standard deviation
    for (int day = 1; day <= 365; day++) {

        const auto& mm_values = daily_values[day - 1];

        // Get size of vector and cast to int
        const int n = static_cast<int>(mm_values.size());

        // Calculate mean
        double mm_sum = 0.0;

        for (double mm_value : mm_values) {
            mm_sum += mm_value; 
        }

        const double mean = mm_sum / n;

        // Calculate std deviation
        double sum_squared_deviations = 0.0;

        for (double mm_value : mm_values) {
            sum_squared_deviations += (mm_value - mean) * (mm_value - mean);
        }

        const double standard_deviation = std::sqrt(sum_squared_deviations / (n - 1));

        // Append results to vectors
        results.mean_precipitations.push_back(mean);
        results.standard_deviations.push_back(standard_deviation);
        results.day_numbers.push_back(day);
    }

    return results;
}


void plot_precipitation_data(const DailyStats& stats_data, const std::string& city_name, const std::string& output_filename)
{   
    TH1D* histogram = new TH1D{"precipitation", ("Seasonal precipitation variation in " + city_name +";Day of year;Mean precipitation (mm)").c_str(), 365, 1, 366};

    for (int bin = 1; bin <= histogram->GetNbinsX(); bin++) {
        histogram->SetBinContent(bin, stats_data.mean_precipitations[bin - 1]);
        histogram->SetBinError(bin, stats_data.standard_deviations[bin - 1]);
    }

    TCanvas* c1 = new TCanvas("c1", "Precipitation", 800, 600);

    histogram->Draw("E1");
    c1->SaveAs(("results/precipitation/" + output_filename).c_str());

    delete histogram;
    delete c1;
}