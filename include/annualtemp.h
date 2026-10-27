#include <iostream>
#include <vector>
#include <string>
#include<csv/csv.h>

void annualmean(const std::string& file);

class annualtemp {
    public:
        explicit annualtemp(const std::string& areaName, const std::string& file);

        void plotTemp(const std::string& filename) const;
        void plotTempComp(const annualtemp& other, const std::string& filename) const; 
    private:
        std::vector<int> years;
        std::vector<double> meanTemp;
        std::string name;
};