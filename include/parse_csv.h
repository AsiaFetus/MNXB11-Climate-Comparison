#ifndef PARSE_CSV_H
#define PARSE_CSV_H
// Our external library file
#include<csv/csv.h>
#include<vector>
#include<string>

std::vector<std::vector<std::string>> parseCSV(const std::string& filename, const int expected_columns);

int yearOfDate(const std::string& date);

int dayOfYear(const std::string& date);

int hourOfDay(const std::string& hour);
#endif