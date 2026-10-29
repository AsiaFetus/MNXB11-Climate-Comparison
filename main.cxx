#include "parse_csv.h"
#include "sunshine.h"
#include "sunshine_plot.h"
#include "temperature.h"
#include "temperature_plot.h"
#include "precipitation.h"
#include "annualtemp.h"

#include <iostream>

int main() {
    annualtemp lund("Lund", "datasets/derived_data/temperature/lund_temp_derived.csv");
    annualtemp umea("Ume#aa", "datasets/derived_data/temperature/Umeå_temp_derived.csv");
    lund.plotTemp("lund_annual_temp.pdf");
    umea.plotTemp("umea_annual_temp.pdf");
    lund.plotTempComp(umea,"umea_vs_lund_temp_comp.pdf");
    return 0;
}
