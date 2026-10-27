#include "precipitation.h"
#include "parse_csv.h"
#include <vector>
#include <string>
#include <csv/csv.h>

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