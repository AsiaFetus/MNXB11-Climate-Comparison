#include "parse_csv.h"
#include "sunshine.h"
#include "sunshine_plot.h"

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

    return 0;
}
