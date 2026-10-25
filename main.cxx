#include "parse_csv.h"
#include "sunshine.h"

#include <TCanvas.h>
#include <TH2D.h>
#include <TStyle.h>

#include <iostream>
#include <string>

int main() {

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

    return 0;
}