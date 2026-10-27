#ifndef PRECIPITATION_H
#define PRECIPITATION_H

#include <vector>
#include <string>

struct PrecipitationMeasurement {
    int year;
    int day_number;
    double precipitation_mm;
};

std::vector<PrecipitationMeasurement> extract_precipitation_data(const std::string& filename, bool only_good_quality);



#endif