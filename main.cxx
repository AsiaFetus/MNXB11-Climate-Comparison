#include "parse_csv.h"
#include "sunshine.h"
#include "sunshine_plot.h"
#include "temperature.h"

#include <iostream>

int main() {
    // SUNSHINE DATA ANALYSIS AND PLOTTING

    // Import data
    auto lund = parseCSV(
        "datasets/derived_data/sunshine/lund_sunshine_derived.csv", 4
    );

    auto umea = parseCSV(
        "datasets/derived_data/sunshine/umea_sunshine_derived.csv", 4
    );

    // Calculate averages
    auto lundDaily = averageSunshineByDay(lund);
    auto umeaDaily = averageSunshineByDay(umea);

    auto lundYearly = averageSunshineByYear(lund);
    auto umeaYearly = averageSunshineByYear(umea);

    // Generate plots
    plotSunshineAllYears(lund, lundDaily, "Lund");
    plotSunshineAllYears(umea, umeaDaily, "Umea");

    plotSunshineComparison(lundDaily, umeaDaily, "Lund", "Umea");
    plotYearlySunshineComparison(lundYearly, umeaYearly, "Lund", "Umea");

    //ANALYSIS 1: SEASONAL VARIATION IN MEAN TEMPERATURE

    auto umea_temperature = parseTemperatureCSV(
        "datasets/derived_data/temperature/Umeå_temp_derived.csv"
    );

    auto lund_temperature = parseTemperatureCSV(
        "datasets/derived_data/temperature/lund_temp_derived.csv"
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
