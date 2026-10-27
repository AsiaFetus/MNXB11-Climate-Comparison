#include <iostream>
#include <vector>
#include <string>
#include "parse_csv.h"
#include "annualtemp.h"
#include <algorithm>
#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TF1.h>
#include <TStyle.h>
#include <TGraph.h>



annualtemp::annualtemp(const std::string& areaName,const std::string& file) : name(areaName) {

    /*
    // At the START of annualtemp::annualtemp:
std::cout << "[DEBUG] Constructor STARTED." << std::endl;
*/

    std::vector<std::vector<std::string>> rawdata = parseCSV(file, 4);
    std::cout << file << "rows" << rawdata.size() << std::endl;
    if (!rawdata.empty()) {
        std::cout << "--- UMEA COLUMN INSPECT ---" << std::endl;
        for (size_t c = 0; c < rawdata[1].size(); ++c) {
            std::cout << "  col[" << c << "] = " << rawdata[1][c] << std::endl;
        }
    }
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

    size_t limit = 66;
    for (size_t i = 50; i < limit; ++i) {
        std::cout << "Year: " << years[i] << " | Mean Temp: " << meanTemp[i] << " °C" << std::endl;
}
/*
// At the END of annualtemp::annualtemp:
std::cout << "[DEBUG] Constructor FINISHED. Stored years size: " << years.size() << std::endl;
*/
}

void annualtemp::plotTemp(const std::string& filename) const {
/*
    // At the START of annualtemp::plotTemp:
std::cout << "[DEBUG] plotTemp() CALLED. Years size is: " << years.size() << std::endl;

*/
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

    hist->SetTitle("Mean annual temperature;" "Year; Temperature(°C)");
    hist->SetStats(0);
    hist->SetFillColor(kAzure + 4);
    hist->Draw("HIST");

    //hist->Draw("AXIS");

/*
    TGraph *graph = new TGraph();
    for(int bin = 1; bin <= hist->GetNbinsX(); ++bin) {
        graph->Expand(graph->GetN() + 1, 100);
        graph->SetPoint(graph->GetN(), hist->GetBinCenter(bin),
        hist->GetBinContent(bin));
    }
    
    graph->SetLineColor(kAzure + 4);
    graph->SetLineWidth(2);
    graph->Draw("SAME C");
    */
    
    TF1 *f1 = new TF1("f1","pol1", minYear, maxYear);
    f1->SetLineColor(kRed - 4);
    f1->SetLineWidth(2);
    hist->Fit(f1, "R");
    f1->Draw("SAME");
    canvas->SaveAs(filename.c_str());



}

void annualtemp::plotTempComp(const annualtemp& other, const std::string& filename) const {
    int startYear = std::max(years.front(), other.years.front());
    int endYear = 2025;
    int nBins =  endYear - startYear;

    // 2. Calculate unified Y-axis bounds from BOTH datasets
    double min_val = std::min(
        *std::min_element(meanTemp.begin(), meanTemp.end()),
        *std::min_element(other.meanTemp.begin(), other.meanTemp.end())
    );
    double max_val = std::max(
        *std::max_element(meanTemp.begin(), meanTemp.end()),
        *std::max_element(other.meanTemp.begin(), other.meanTemp.end()));

    TCanvas *canvas = new TCanvas("canvas_comp", "Temperature comparison", 1000, 600);
    canvas->SetGrid();
    TH1D *hist1 = new TH1D("hist1", "Mean Annual Temperature (°C)", nBins, startYear, endYear);
    TH1D *hist2 = new TH1D("hist2", "Mean Annual Temperature (°C)", nBins, startYear, endYear);

    for (size_t i = 0; i< years.size(); ++i) {
        if (years[i] >= startYear && years[i] <=endYear){
            int bin = hist1->FindBin(years[i]);
            hist1->SetBinContent(bin, meanTemp[i]);
        }
    }

    for (size_t i = 0; i< other.years.size(); ++i) {
        if (other.years[i] >= startYear && other.years[i] <=endYear){
            int bin = hist2->FindBin(other.years[i]);
            hist2->SetBinContent(bin, other.meanTemp[i]);
        }
    }

    hist1->SetStats(0);
    hist1->SetLineColor(kAzure);
    hist1->SetLineWidth(2);
    hist1->SetTitle("Mean annual temperature comparison;""Year; Temperature(°C)");

    hist2->SetStats(0);
    hist2->SetLineColor(kRed);
    hist2->SetLineWidth(2);
    hist1->SetMinimum(min_val - 1.0);
    hist1->SetMaximum(max_val + 1.0);

    hist1->Draw("HIST");
    hist2->Draw("HIST SAME");

    TLegend *legend = new TLegend(0.75, 0.78, 0.90, 0.90);
    legend->AddEntry(hist1, this->name.c_str(),"l");
    legend->AddEntry(hist2, other.name.c_str(), "l");
    legend->Draw();
    canvas->SaveAs(filename.c_str());
}