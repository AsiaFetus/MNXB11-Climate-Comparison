#include <iostream>
#include <vector>
#include <string>
#include "parse_csv.h"
#include "annualtemp.h"
#include <algorithm>
#include <TH1D.h>
#include <TCanvas.h>


annualtemp::annualtemp(const std::string& file) {
    // At the START of annualtemp::annualtemp:
std::cout << "[DEBUG] Constructor STARTED." << std::endl;


    std::vector<std::vector<std::string>> rawdata = parseCSV(file, 4);

    std::cout << file << "rows" << rawdata.size() << std::endl;

    std::vector<int> years_temp;
    std::vector<double> year_sum;
    std::vector<int> count;

    for (const auto& rows : rawdata) {
        int year = std::stoi(rows[0]);
        double temp = std::stod(rows[3]);

        bool check = false;
        for (size_t i = 0; i < years_temp.size(); ++i) {
            if (years_temp[i] == year) {
                year_sum[i] += temp;
                count[i] += 1; 
                check = true;
                break;
            }
        }

        if (!check) {
            years_temp.push_back(year);
            year_sum.push_back(temp);
            count.push_back(1);
        }
    }
    
    for (size_t i = 0; i < years_temp.size(); ++i) {
        years.push_back(years_temp[i]);
        meanTemp.push_back(year_sum[i] / count[i]);
    }

    size_t limit = 100;
    for (size_t i = 87; i < limit; ++i) {
        std::cout << "Year: " << years[i] << " | Mean Temp: " << meanTemp[i] << " °C" << std::endl;
}

// At the END of annualtemp::annualtemp:
std::cout << "[DEBUG] Constructor FINISHED. Stored years size: " << years.size() << std::endl;
}

void annualtemp::plotTemp() const {

    // At the START of annualtemp::plotTemp:
std::cout << "[DEBUG] plotTemp() CALLED. Years size is: " << years.size() << std::endl;
    if (years.empty() || meanTemp.empty()) {
        std::cerr << "Error: 'years' or 'meanTemps' vector is empty! Aggregation failed." << std::endl;
        return;
    }
    TCanvas *canvas = new TCanvas("canvas", "Temperature Histogram", 1000, 600);
    canvas->SetGrid();

    int minYear = years.front() - 1;
    int maxYear = years.back() + 1;
    int nBins = maxYear - minYear;

    TH1D *hist = new TH1D("hist", "Mean Annual Temperature (°C)", nBins, minYear, maxYear);

    for(size_t i = 0; i < years.size(); ++i) {
        int bin = hist->FindBin(years[i]);
        hist->SetBinContent(bin, meanTemp[i]);
    }
    hist->Draw("HIST");

    canvas->SaveAs("annual_temp_graph.pdf");
}