#include <iostream>
#include <vector>
#include <string>
#include<csv/csv.h>

void annualmean(const std::string& file);

class annualtemp {
    public:
        explicit annualtemp(const std::string& file);

        void plotTemp() const;
    private:
        std::vector<int> years;
        std::vector<double> meanTemp;
};