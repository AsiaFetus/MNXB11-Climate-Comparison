#include <iostream>
#include <vector>
#include <string>
#include "parse_csv.h"
#include <algorithm>

void annualmean(const std::string& file) {
    std::vector<std::vector<std::string>> rawdata = parseCSV(file, 4);

    std::cout << file << "rows" << rawdata.size() << std::endl;

    std::vector<int> years;
    std::vector<double> year_sum;
    std::vector<int> count;

    for (const auto& rows : rawdata) {
        int year = std::stoi(rows[0]);
        double temp = std::stod(rows[3]);

        bool check = false;
        for (size_t i = 0; i < years.size(); i++) {
            if (years[i] == year) {
                year_sum[i] += temp;
                count[i] += 1; 
                check = true;
                break;
            }
        }

        if (!check) {
            years.push_back(year);
            year_sum.push_back(temp);
            count.push_back(1);
        }
    }
    
    std::vector<double> meanTemp;
    for (size_t i = 0; i < years.size(); i++) {
        meanTemp.push_back(year_sum[i] / count[i]);
    }
/*
// Safely check up to the first 5 years (or fewer if the dataset is tiny)
    size_t limit = std::min(size_t(5), years.size());
    
    for (size_t i = 0; i < limit; ++i) {
        double meanTemp = year_sum[i] / count[i];
        std::cout << "Year: " << years[i] 
                  << " | Readings count: " << count[i] 
                  << " | Mean Temp: " << meanTemp << " °C" << std::endl;
    }
*/
    
}