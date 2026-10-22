#include "parse_csv.h"
#include <iostream>

int main() {

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

    return 0;
}
