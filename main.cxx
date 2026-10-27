#include "parse_csv.h"
#include "sunshine.h"
#include "sunshine_plot.h"
#include "temperature.h"
#include "temperature_plot.h"
#include "precipitation.h"

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

    auto lundSeasonal =
        calculateMeanTemperatureByDayOfYear(lund_daily);

    auto umeaSeasonal =
        calculateMeanTemperatureByDayOfYear(umea_daily);

    plotSeasonalTemperature(
        lundSeasonal,
        umeaSeasonal
    );

    std::cout << "Lund daily means: "
              << lund_daily.size() << std::endl;

    std::cout << "Umeå daily means: "
              << umea_daily.size() << std::endl;
    
    std::cout << "Lund seasonal points: "
              << lundSeasonal.days.size() << std::endl;

    std::cout << "Umeå seasonal points: "
              << umeaSeasonal.days.size() << std::endl;

    // PRECIPITATION ANALYSIS
    bool onlyGoodQuality{false};
    auto lundPrecipitationData{extract_precipitation_data("datasets/derived_data/precipitation/precipitation_lund_derived.csv", onlyGoodQuality)};
    auto umeaPrecipitationData{extract_precipitation_data("datasets/derived_data/precipitation/precipitation_umea_derived.csv", onlyGoodQuality)};

    // Print the first 10 Lund observations
    std::cout << "\nLund precipitation data:\n";

    for (std::size_t i = 0; i < lundPrecipitationData.size() && i < 10; i++)
    {
    const auto& obs = lundPrecipitationData[i];

    std::cout << "Year: " << obs.year
              << ", Day: " << obs.day_number
              << ", Precipitation: " << obs.precipitation_mm
              << " mm\n";
    }

    // Good vs bad check in Lund
    auto lundGood{extract_precipitation_data("datasets/derived_data/precipitation/precipitation_lund_derived.csv", true)};
    std::cout << "Lund precipitation quality statistics:\n";
    std::cout << "Good-quality observations (G): " << lundGood.size() << '\n';
    std::cout << "Total observations: " << lundPrecipitationData.size() << '\n';

    return 0;
}
