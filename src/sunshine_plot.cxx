
#include "sunshine_plot.h"

#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TF1.h>

#include <vector>
#include <string>

// Apply consistent styling to all plots
void setupPlot() {
    gStyle->SetOptStat(0); // Disable statistics box
    gStyle->SetTitleFontSize(0.045);
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);
}

// Create a histogram from a SunshineProfile
TH1D makeHistogram(
    const SunshineProfile& profile,
    const std::string& name,
    int color,
    int bins,
    double min,
    double max
) {
    TH1D hist(name.c_str(), "", bins, min, max);
    hist.SetDirectory(nullptr);
    hist.SetLineColor(color);
    hist.SetLineWidth(3);

    // Populate the histogram with the mean values from the profile
    for (size_t i = 0; i < profile.days.size(); i++) {
        int bin = hist.FindBin(profile.days[i]);
        hist.SetBinContent(bin, profile.meanValues[i]);
    }

    return hist;
}

// Show all individual years and their average
void plotSunshineAllYears(
    const std::vector<std::vector<std::string>>& data,
    const SunshineProfile& averageProfile,
    const std::string& city
) {
    setupPlot();

    auto irradiation = sunshineByYearAndDay(data);
    TCanvas canvas("allYears", "Solar irradiation", 1100, 650); // Create a canvas for the plot (widtha and height in pixels) 

    std::vector<TH1D> yearlyHists; // Vector to hold histograms for each year
    yearlyHists.reserve(irradiation.size()); // Reserve space for all years

    TH1D average = makeHistogram(
        averageProfile, "average", kBlue + 1, 365, 0.5, 365.5
    ); // Create a histogram for the average profile

    average.SetTitle(("Daily solar irradiation in " + city).c_str());

    average.GetXaxis()->SetTitle("Day of year");
    average.GetYaxis()->SetTitle("Daily irradiation (Wh/m^{2})");

    average.SetMinimum(0);
    average.Draw("HIST");

    // Draw the average first to establish the axes
    average.SetMinimum(0); // Set the minimum value of the y-axis to 0
    average.SetMaximum(1.4 * average.GetMaximum()); // Set the maximum value of the y-axis to 40% more than the maximum value of the average histogram

    // Add each year as a transparent histogram
    for (size_t y = 0; y < irradiation.size(); y++) {
        TH1D hist(
            ("year_" + std::to_string(y)).c_str(),
            "", 365, 0.5, 365.5
        ); // Create a histogram for the current year
        hist.SetDirectory(nullptr);

        // Populate the histogram with the daily irradiation values for the current year
        for (int day = 1; day <= 365; day++)
            hist.SetBinContent(day, irradiation[y][day]);

        hist.SetLineColorAlpha(kGray + 1, 0.15);
        hist.SetLineWidth(1);

        yearlyHists.push_back(hist);
        yearlyHists.back().Draw("HIST SAME"); // Draw the histogram without errasing the previous histograms
    }

    // Average histogram on top
    average.Draw("HIST SAME"); 

    // Add a legend to the plot
    TLegend legend(0.72, 0.78, 0.90, 0.90);
    legend.AddEntry(&yearlyHists[0], "Individual years", "l"); // line
    legend.AddEntry(&average, "Average", "l");
    legend.Draw();

    canvas.SaveAs(("./results/sunshine/" + city + "_sunshine_all_years.pdf").c_str());
    canvas.Clear();
}

// Compare average daily irradiation between cities
void plotSunshineComparison(
    const SunshineProfile& profile1,
    const SunshineProfile& profile2,
    const std::string& city1,
    const std::string& city2
) {
    setupPlot();

    TCanvas canvas("daily", "Daily comparison", 1100, 650);

    TH1D hist1 = makeHistogram(profile1, "daily1", kBlue + 1, 365, 0.5, 365.5);
    TH1D hist2 = makeHistogram(profile2, "daily2", kRed + 1, 365, 0.5, 365.5);

    hist1.SetTitle(
        "Average daily solar irradiation;"
        "Day of year;Daily irradiation (Wh/m^{2})"
    ); // Set the title and axis labels for the histogram

    hist1.SetMinimum(0);
    hist1.SetMaximum(1.1 * std::max(hist1.GetMaximum(), hist2.GetMaximum()));

    hist1.Draw("HIST");
    hist2.Draw("HIST SAME");

    TLegend legend(0.75, 0.78, 0.90, 0.90);
    legend.AddEntry(&hist1, city1.c_str(), "l");
    legend.AddEntry(&hist2, city2.c_str(), "l");
    legend.Draw();

    canvas.SaveAs("./results/sunshine/daily_sunshine_comparison.pdf");
}

// Compare annual mean irradiance between cities

void plotYearlySunshineComparison(
    const SunshineProfile& profile1,
    const SunshineProfile& profile2,
    const std::string& city1,
    const std::string& city2
) {
    setupPlot();

    TCanvas canvas("yearly", "Annual solar irradiance", 1100, 650);

    // Create graphs using our existing data
    TGraph graph1(
        profile1.days.size(),
        profile1.days.data(),
        profile1.meanValues.data()
    );

    TGraph graph2(
        profile2.days.size(),
        profile2.days.data(),
        profile2.meanValues.data()
    );

    // Style the measured data
    graph1.SetLineColor(kBlue + 1);
    graph2.SetLineColor(kRed + 1);

    graph1.SetLineWidth(2);
    graph2.SetLineWidth(2);

    // Fit a straight line to each city's data
    TF1 fit1("fit1", "pol1", profile1.days.front(), profile1.days.back());
    TF1 fit2("fit2", "pol1", profile2.days.front(), profile2.days.back());

    graph1.Fit(&fit1, "Q0");
    graph2.Fit(&fit2, "Q0");

    // Style the fitted lines
    fit1.SetLineColor(kBlue + 1);
    fit2.SetLineColor(kRed + 1);

    fit1.SetLineStyle(2);
    fit2.SetLineStyle(2);

    fit1.SetLineWidth(2);
    fit2.SetLineWidth(2);

    // Draw the measured data
    TMultiGraph graphs;
    graphs.Add(&graph1);
    graphs.Add(&graph2);

    graphs.Draw("AL");

    // Set titles after drawing the axes
    graphs.SetTitle("Annual mean solar irradiance");
    graphs.GetXaxis()->SetTitle("Year");
    graphs.GetYaxis()->SetTitle("Mean irradiance (W/m^{2})");

    // Add the linear fits
    fit1.Draw("SAME");
    fit2.Draw("SAME");

    // Legend in the upper-left corner
    TLegend legend(0.14, 0.70, 0.38, 0.89);

    legend.AddEntry(&graph1, city1.c_str(), "l");
    legend.AddEntry(&graph2, city2.c_str(), "l");
    legend.AddEntry(
        &fit1,
        Form("%s fit: y = %.3fx %+.1f",
            city1.c_str(), fit1.GetParameter(1), fit1.GetParameter(0)),
        "l"
    );

    legend.AddEntry(
        &fit2,
        Form("%s fit: y = %.3fx %+.1f",
            city2.c_str(), fit2.GetParameter(1), fit2.GetParameter(0)),
        "l"
    );
    legend.Draw();

    canvas.SaveAs("results/sunshine/yearly_sunshine_comparison.pdf");
}
