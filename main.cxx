#include "parse_csv.h"
#include "sunshine.h"
#include "temperature.h"

#include <TCanvas.h>
#include <TH2D.h>
#include <TStyle.h>

#include <iostream>
#include <string>

int main() {

    //Analysis 4
    auto umea = parseCSV(
        "datasets/derived_data/sunshine/umea_sunshine_derived.csv", 4
    );
    auto lund = parseCSV(
        "datasets/derived_data/sunshine/lund_sunshine_derived.csv", 4
    );

    SunshineQuality umea_quality = analyzeSunshineData(umea);
    SunshineQuality lund_quality = analyzeSunshineData(lund);
    printSunshineQuality(umea_quality);
    printSunshineQuality(lund_quality);

    //Analysis 1
    auto umea_temperature = parseCSV(
        "datasets/derived_data/temperature/Umeå_temp_derived.csv", 4
    );

    auto lund_temperature = parseCSV(
        "datasets/derived_data/temperature/lund_temp_derived.csv", 4
    );

    auto lund_daily = calculateDailyMean(
        lund_temperature, 2011, 2025
    );

    auto umea_daily = calculateDailyMean(
        umea_temperature, 2011, 2025
    );

    std::cout << "Lund daily means: "
              << lund_daily.size() << std::endl;

    std::cout << "Umeå daily means: "
              << umea_daily.size() << std::endl;

    return 0;
}