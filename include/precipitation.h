#ifndef PRECIPITATION_H
#define PRECIPITATION_H

#include <vector>
#include <string>

// Data for each measurement
struct PrecipitationMeasurement {
    int year;
    int day_number;
    double precipitation_mm;
};

// Returns vector of measurements
std::vector<PrecipitationMeasurement> extract_precipitation_data(const std::string& filename, bool only_good_quality);

// Struct of stats for plotting
struct DailyStats {
    std::vector<double> day_numbers;
    std::vector<double> mean_precipitations;
    std::vector<double> standard_deviations;
};

DailyStats calculate_day_stats(const std::vector<PrecipitationMeasurement>& data);

#endif