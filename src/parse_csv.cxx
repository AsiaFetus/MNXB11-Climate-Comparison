
#include <ctime>
#include <iomanip>
#include <sstream>
#include <string>

#include "parse_csv.h"
#include <csv/csv.h>
#include <string>
#include <vector>

int yearOfDate(const std::string& date) {
    std::tm time{}; // Create a time object 
    std::istringstream ss(date); // Convert string to stream (year-month-day)

    ss >> std::get_time(&time, "%Y-%m-%d"); // Parse the date string in to the time object

    return time.tm_year + 1900; // Return the year (years since 1900)
}

int dayOfYear(const std::string& date) {
    std::tm time{}; // Create a time object 
    std::istringstream ss(date); // Convert string to stream (year-month-day)

    ss >> std::get_time(&time, "%Y-%m-%d"); // Parse the date string in to the time object

    return time.tm_yday + 1; // Return the day of the year (0-365) + 1
}

int hourOfDay(const std::string& hour) {
    std::tm time{}; // Create a time object 
    std::istringstream ss(hour); // Convert string to stream (hour:minute)

    ss >> std::get_time(&time, "%H:%M:%S"); // Parse the hour string in to the time object

    return time.tm_hour; // Return the hour of the day (0-23)
}

// Create a 2D vector containing the CSV observations
std::vector<std::vector<std::string>> parseCSV(const std::string& filename, const int expected_columns) {
    
    std::string date{};
    double data{};
    std::string quality{};

    if (expected_columns == 4) {

        io::CSVReader<4> input{filename};
        std::string hour{};
        std::vector<std::vector<std::string>> rows{};

        while (input.read_row(date, hour, data, quality)) {
            rows.push_back({
                std::to_string(yearOfDate(date)), 
                std::to_string(dayOfYear(date)), 
                std::to_string(hourOfDay(hour)), 
                std::to_string(data), 
                quality});
        }

        return rows;

    } else if (expected_columns == 3) {

        io::CSVReader<3> input{filename};
        std::vector<std::vector<std::string>> rows{};

        while (input.read_row(date, data, quality)) {
            rows.push_back({
                std::to_string(yearOfDate(date)), 
                std::to_string(dayOfYear(date)), 
                std::to_string(data), 
                quality});
        }

        return rows;
    } else {
        throw std::invalid_argument("Expected columns must be either 3 or 4.");
    }
}