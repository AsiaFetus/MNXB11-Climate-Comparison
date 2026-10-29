#include "parse_csv.h"
#include <iostream>
#include "annualtemp.h"

int main() {

    /*
    // Import sunshine data from Lund
    auto lund_sunshine = parseCSV("datasets/derived_data/sunshine/lund_sunshine_derived.csv", 4);

    std::cout << "\nImported " << lund_sunshine.size() << " rows\n";

    // Print first 5 observations
    for (int i = 0; i < 5 && i < static_cast<int>(lund_sunshine.size()); i++) {
        for (const auto& value : lund_sunshine[i]) {
            std::cout << value << " ";
        }
        std::cout << '\n';
    }
*/
    

annualtemp lund("Lund","datasets/derived_data/temperature/lund_temp_derived.csv");

lund.plotTemp("lund_annual_temp.pdf");

annualtemp umea("Ume#aa", "datasets/derived_data/temperature/Umeå_temp_derived.csv");

umea.plotTemp("umea_annual_temp.pdf");

umea.plotTempComp(lund, "umea_vs_lund_temp_comp.pdf");

return 0;
}
