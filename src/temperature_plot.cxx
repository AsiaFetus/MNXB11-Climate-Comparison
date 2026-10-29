#include "temperature_plot.h"

#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TAxis.h>

#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <sstream>
#include <iomanip>
#include <ctime>

// Draw the seasonal mean temperature profiles of Lund and Umeå
void plotSeasonalTemperature(
    const TemperatureProfile& lundProfile,
    const TemperatureProfile& umeaProfile
) {
    // Configure the general appearance of the plots
    gStyle->SetOptStat(0);
    gStyle->SetTitleFontSize(0.045);
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);

    // Create the drawing canvas
    TCanvas canvas(
        "temperature_canvas",
        "Seasonal temperature",
        1000,
        700
    );

    // Create one histogram with 365 bins for each city
    TH1D lundHistogram(
        "lund_temperature",
        "Daily mean temperature;Day of year;Temperature (#circC)",
        365,
        0.5,
        365.5
    );

    TH1D umeaHistogram(
        "umea_temperature",
        "Daily mean temperature;Day of year;Temperature (#circC)",
        365,
        0.5,
        365.5
    );

    // Keep the histograms independent of ROOT's current directory
    lundHistogram.SetDirectory(nullptr);
    umeaHistogram.SetDirectory(nullptr);

    // Set a different colour for each city
    lundHistogram.SetLineColor(kBlue);
    lundHistogram.SetLineWidth(3);
    umeaHistogram.SetLineColor(kRed);
    umeaHistogram.SetLineWidth(3);

    // Use the same temperature range for both cities
    lundHistogram.SetMinimum(-30);
    lundHistogram.SetMaximum(30);

    // Fill the histograms with the pre-calculated mean temperatures
    for (std::size_t i = 0; i < lundProfile.days.size(); ++i) {
        lundHistogram.SetBinContent(
            lundProfile.days[i],
            lundProfile.meanValues[i]
        );
    }

    for (std::size_t i = 0; i < umeaProfile.days.size(); ++i) {
        umeaHistogram.SetBinContent(
            umeaProfile.days[i],
            umeaProfile.meanValues[i]
        );
    }

    // Draw Lund and overlay Umeå on the same axes
    lundHistogram.Draw("HIST");
    umeaHistogram.Draw("HIST SAME");

    // Identify the two curves in a legend
    TLegend legend(0.70, 0.75, 0.90, 0.90);
    legend.AddEntry(&lundHistogram, "Lund", "l");
    legend.AddEntry(&umeaHistogram, "Ume#aa", "l");
    legend.Draw();

    // Save the figure as a PDF file
    canvas.SaveAs(
        "results/temperature/seasonal_temperature_comparison.pdf"
    );
}

// Draw each year's daily temperatures and the overall average profile
void plotTemperatureAllYears(
    const std::vector<DailyTemperature>& dailyData,
    const TemperatureProfile& averageProfile,
    const std::string& city
) {
    gStyle->SetOptStat(0);
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);

    // Store each year's points as (day of year, daily mean temperature)
    std::map<int, std::vector<std::pair<int, double>>> yearlyData;

    for (const auto& day : dailyData) {
        // Parse the date in YYYY-MM-DD format
        std::tm date = {};
        std::istringstream dateStream(day.date);
        dateStream >> std::get_time(&date, "%Y-%m-%d");

        if (dateStream.fail()) {
            continue;
        }

        //Set the time fields before converting the date
        date.tm_hour = 12;
        date.tm_isdst = -1;

        if (std::mktime(&date) == -1) {
            continue;
        }

        int year = date.tm_year + 1900;

        // Convert the date to a common 365-day calendar
        // Exclude February 29 and shift later dates in leap years
        bool leapYear =
            (year % 4 == 0 && year % 100 != 0) || 
            (year % 400 == 0);

        if (date.tm_mon == 1 && date.tm_mday == 29) {
            continue;
        }

        int dayOfYear = date.tm_yday + 1;

        if (leapYear && date.tm_mon > 1) {
            --dayOfYear;
        }

        // Store the point under its corresponding year
        yearlyData[year].push_back({
            dayOfYear,
            day.meanTemperature
        });
    }

    // Create a canvas for this city
    TCanvas canvas(
        ("temperature_years_" + city).c_str(),
        (city + " daily mean temperatures by year").c_str(),
        1000,
        700
    );

    // A multigraph combines the yearly curves and the average curve
    TMultiGraph multiGraph;
    multiGraph.SetTitle(
    ("Daily mean temperature in " + city +
     ";Day of year;Daily mean temperature (#circC)").c_str()
    );

    // Draw each year as a separate grey line
    for (auto& [year, points] : yearlyData) {
        std::sort(
            points.begin(),
            points.end(),
            [](const auto& a, const auto& b) {
                return a.first < b.first;
            }
        );

        auto* graph = new TGraph(static_cast<int>(points.size()));
        graph->SetName(
            (city + "_temperature_" + std::to_string(year)).c_str()
        );
        graph->SetLineColor(kGray + 1);
        graph->SetLineWidth(1);

        for (std::size_t i = 0; i < points.size(); ++i) {
            graph->SetPoint(
                static_cast<int>(i),
                points[i].first,
                points[i].second
            );
        }

        multiGraph.Add(graph, "L");
    }

    // Create the blue graph for the average seasonal profile
    auto* averageGraph =
        new TGraph(static_cast<int>(averageProfile.days.size()));

    averageGraph->SetName((city + "_temperature_average").c_str());
    averageGraph->SetLineColor(kBlue);
    averageGraph->SetLineWidth(3);

    for (std::size_t i = 0; i < averageProfile.days.size(); ++i) {
        averageGraph->SetPoint(
            static_cast<int>(i),
            averageProfile.days[i],
            averageProfile.meanValues[i]
        );
    }

    // Draw all yearly curves and overlay the average
    multiGraph.Add(averageGraph, "L");
    multiGraph.Draw("A");

    // Set the axis ranges and add a legend
    multiGraph.GetXaxis()->SetLimits(1, 365);
    multiGraph.SetMinimum(-30);
    multiGraph.SetMaximum(30);

    TLegend legend(0.62, 0.76, 0.90, 0.90);
    legend.AddEntry((TObject*)nullptr, "Individual years", "");
    legend.AddEntry(averageGraph, "2011-2025 average", "l");
    legend.Draw();

    // Save the figure for the current city
    canvas.SaveAs(
        ("results/temperature/" + city + "_temperature_all_years.pdf").c_str()
    );

}
