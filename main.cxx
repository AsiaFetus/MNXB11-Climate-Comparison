#include "parse_csv.h"
#include "sunshine.h"
#include "sunshine_plot.h"
#include "temperature.h"
#include "temperature_plot.h"
#include "precipitation.h"
#include "annualtemp.h"

#include <iostream>

int main() {
    // SUNSHINE DATA ANALYSIS AND PLOTTING

    // Import data
    auto lund_sunshine = parseCSV(
        "datasets/derived_data/sunshine/lund_sunshine_derived.csv", 4
    );

    auto umea_sunshine = parseCSV(
        "datasets/derived_data/sunshine/umea_sunshine_derived.csv", 4
    );

    // Analyze sunshine quality
    auto lundQuality = analyzeSunshineData(lund_sunshine);
    auto umeaQuality = analyzeSunshineData(umea_sunshine);

    std::cout << "Lund sunshine quality statistics:\n";
    printSunshineQuality(lundQuality);
    std::cout << "\nUmeå sunshine quality statistics:\n";
    printSunshineQuality(umeaQuality);

    // Calculate averages
    auto lundDaily = averageSunshineByDay(lund_sunshine);
    auto umeaDaily = averageSunshineByDay(umea_sunshine);

    auto lundYearly = averageSunshineByYear(lund_sunshine);
    auto umeaYearly = averageSunshineByYear(umea_sunshine);

    // Generate plots
    plotSunshineAllYears(lund_sunshine, lundDaily, "Lund");
    plotSunshineAllYears(umea_sunshine, umeaDaily, "Umea");

    plotSunshineComparison(lundDaily, umeaDaily, "Lund", "Umea");
    plotYearlySunshineComparison(lundYearly, umeaYearly, "Lund", "Umea");

    //ANALYSIS 1: SEASONAL VARIATION IN MEAN TEMPERATURE

    // Import the derived temperature data for both cities
    auto umea_temperature = parseTemperatureCSV(
        "datasets/derived_data/temperature/Umeå_temp_derived.csv"
    );

    auto lund_temperature = parseTemperatureCSV(
        "datasets/derived_data/temperature/lund_temp_derived.csv"
    );

    //Calculate mean temperature for each valid day between 2011 and 2025
    auto lund_daily = calculateDailyMean(
        lund_temperature, 2011, 2025
    );

    auto umea_daily = calculateDailyMean(
        umea_temperature, 2011, 2025
    );

    //Calculate the average temperature for each day of a common 365-day calendar
    auto lundSeasonal =
        calculateMeanTemperatureByDayOfYear(lund_daily);

    auto umeaSeasonal =
        calculateMeanTemperatureByDayOfYear(umea_daily);

    //Plot the individual years together with the average seasonal profile for each city
    plotTemperatureAllYears(
        lund_daily,
        lundSeasonal,
        "Lund"
    );

    plotTemperatureAllYears(
        umea_daily,
        umeaSeasonal,
        "Umea"
    );

    // Generate the average seasonal comparison plot
    plotSeasonalTemperature(
        lundSeasonal,
        umeaSeasonal
    );

    //Print the number of calculated daily means and seasonal data points to check the results
    std::cout << "Lund daily means: "
              << lund_daily.size() << std::endl;

    std::cout << "Umeå daily means: "
              << umea_daily.size() << std::endl;
    
    std::cout << "Lund seasonal points: "
              << lundSeasonal.days.size() << std::endl;

    std::cout << "Umeå seasonal points: "
              << umeaSeasonal.days.size() << std::endl;

    // PRECIPITATION ANALYSIS
    bool onlyGoodQuality{true};
    auto lundPrecipitationData{extract_precipitation_data("datasets/derived_data/precipitation/precipitation_lund_derived.csv", onlyGoodQuality)};
    auto umeaPrecipitationData{extract_precipitation_data("datasets/derived_data/precipitation/precipitation_umea_derived.csv", onlyGoodQuality)};

    auto lund_stats{calculate_day_stats(lundPrecipitationData)};
    auto umea_stats{calculate_day_stats(umeaPrecipitationData)};

    plot_precipitation_data(lund_stats, "Lund", "lund_precipitation.pdf");
    plot_precipitation_data(umea_stats, "Umea", "umea_precipitation.pdf");

    // Temperature anaylsis
    annualtemp lund("Lund", "datasets/derived_data/temperature/lund_temp_derived.csv");
    annualtemp umea("Umeå", "datasets/derived_data/temperature/Umeå_temp_derived.csv");
    lund.plotTemp("./results/temperature/lund_annual_temp.pdf");
    umea.plotTemp("./results/temperature/umea_annual_temp.pdf");
    lund.plotTempComp(umea,"./results/temperature/umea_vs_lund_temp_comp.pdf");
    return 0;
}
