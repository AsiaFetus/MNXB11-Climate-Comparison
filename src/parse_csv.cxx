#include "parse_csv.h"
// Details of how this library works is not the important part here
std::vector<Geometry::Rectangle> parse_rectangles(const std::string& filename) {
    io::CSVReader<2> input{filename};
    input.read_header(io::ignore_extra_column, "Width", "Height");
    double width{};
    double height{};
    std::vector<Geometry::Rectangle> rows{};
    while(input.read_row(width, height)) {
        rows.push_back(Geometry::Rectangle{width, height});
    }
    return rows;
}