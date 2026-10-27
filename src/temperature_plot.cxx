#include "temperature_plot.h"

#include <TH1D.h>
#include <TCanvas.h>
#include <TLegend.h>
#include <TStyle.h>

#include <string>

void plotSeasonalTemperature(
    const TemperatureProfile& lundProfile,
    const TemperatureProfile& umeaProfile
) {
    gStyle->SetOptStat(0);
    gStyle->SetTitleFontSize(0.045);
    gStyle->SetPadGridX(true);
    gStyle->SetPadGridY(true);

    TCanvas canvas(
        "temperature_canvas",
        "Seasonal temperature",
        1000,
        700
    );

    TH1D lundHistogram(
        "lund_temperature",
        "Seasonal mean temperature;Day of year;Temperature (#circC)",
        365,
        0.5,
        365.5
    );

    TH1D umeaHistogram(
        "umea_temperature",
        "Seasonal mean temperature;Day of year;Temperature (#circC)",
        365,
        0.5,
        365.5
    );

    lundHistogram.SetDirectory(nullptr);
    umeaHistogram.SetDirectory(nullptr);

    lundHistogram.SetLineColor(kBlue);
    lundHistogram.SetLineWidth(2);

    umeaHistogram.SetLineColor(kRed);
    umeaHistogram.SetLineWidth(2);

    lundHistogram.SetMinimum(-10);
    lundHistogram.SetMaximum(30);

    for (std::size_t i = 0; i < lundProfile.days.size(); ++i) {
        lundHistogram.SetBinContent(
            i + 1,
            lundProfile.meanValues[i]
        );

        lundHistogram.SetBinError(
            i + 1,
            lundProfile.standardDeviations[i]
        );
    }

    for (std::size_t i = 0; i < umeaProfile.days.size(); ++i) {
        umeaHistogram.SetBinContent(
            i + 1,
            umeaProfile.meanValues[i]
        );

        umeaHistogram.SetBinError(
            i + 1,
            umeaProfile.standardDeviations[i]
        );
    }

    lundHistogram.Draw("E1");
    umeaHistogram.Draw("E1 SAME");

    TLegend legend(0.70, 0.75, 0.90, 0.90);
    legend.AddEntry(&lundHistogram, "Lund", "l");
    legend.AddEntry(&umeaHistogram, "Ume#aa", "l");
    legend.Draw();

    canvas.SaveAs(
        "results/temperature/seasonal_temperature_comparison.pdf"
    );
}
