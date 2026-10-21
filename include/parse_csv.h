#ifndef PARSE_CSV_H
#define PARSE_CSV_H
// Our external library file
#include<csv/csv.h>
#include<vector>
#include<string>
#include "Rectangle.h"
std::vector<Geometry::Rectangle> parse_rectangles(const std::string& filename);
#endif // PARSE_CSV_H