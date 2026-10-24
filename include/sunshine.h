#ifndef SUNSHINE_H
#define SUNSHINE_H

#include <string>
#include <vector>

struct SunshineQuality {
    int total = 0;
    int good = 0;
    int yellow = 0;
    int other = 0;
    int negative = 0;

    std::vector<int> yellowYears;
    std::vector<int> yellowDays;
};

SunshineQuality analyzeSunshineData(const std::vector<std::vector<std::string>>& data);

void printSunshineQuality(const SunshineQuality& quality);

#endif 