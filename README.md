# MNXB11 – Lund vs Umeå Climate Comparison

## About

This project is the final project for the MNXB11 course at Lund University.

The objective is to compare the climate of Lund and Umeå using historical meteorological data from the Swedish Meteorological and Hydrological Institute (SMHI).

The data is processed using Bash scripts and C++, while ROOT is used for statistical analysis and visualization.

## Project objectives

We investigate four aspects of the climate:

1. **Seasonal temperature variation (Elena):** How does the mean temperature vary throughout the year in Lund and Umeå?
2. **Long-term temperature trends (Georges):** How has the mean annual temperature changed over time in both cities?
3. **Precipitation (Gabriel):** How does precipitation vary throughout the year, and which city receives more precipitation?
4. **Global solar irradiance (Manuel):** How does incoming solar radiation vary throughout the year, and how does the difference in latitude affect the measured irradiance?

## Project structure

```text
MNXB11-Climate-Comparison/
├── datasets/
│   ├── raw_data/           # Original SMHI data
│   ├── derived_data/       # Cleaned CSV files
│   └── README.md           # Dataset documentation
├── include/                # C++ header files
├── src/                    # C++ source files
├── scripts/
│   └── data/               # Data preprocessing scripts
├── slurm/                  # SLURM submission scripts
├── main.cxx                # Main program
├── Makefile                # Compilation instructions
├── rootlogon.C             # ROOT configuration
├── Workplan.md             # Project tasks and timeline
└── ChangeLog               # Record of project changes
```

## Data

The project uses three meteorological parameters obtained from SMHI:

| Parameter | Unit |
|---|---|
| Temperature | °C |
| Precipitation | mm |
| Global solar irradiance | W/m² |

The original datasets are stored in `datasets/raw_data/`. Cleaned datasets are stored in `datasets/derived_data/`.

The data preparation scripts remove unnecessary metadata and format the measurements for C++ import.

Further information about the measurement stations, CSV formats, and preprocessing can be found in [datasets/README.md](datasets/README.md).

## Building and running

The project uses C++17.

### Requirements

- g++ (C++17 compatible)
- GNU Make
- [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)
- ROOT (for statistical analysis and plotting)

The CSV library header must be available at `external/include/csv/csv.h`. The Makefile must include `-I external/include` in its compiler flags.

To compile the project from the repository root:

```bash
make
```

Run the executable:

```bash
./main
```

To remove compiled files:

```bash
make clean
```

## Data processing and analysis

The project follows these steps:

1. **Data preparation:** Bash scripts extract the relevant measurements from the original SMHI datasets.
2. **Data import and conversion:** C++ functions import the cleaned CSV files into memory.Dates are separated into year and day-of-year values (1–366), while hours are converted into integers (0–23).
3. **Statistical analysis:** Measurements are grouped and analysed to identify seasonal patterns, long-term changes, and differences between the cities.
4. **Visualization:** ROOT is used to produce plots for the climate comparisons.

The CSV importer is implemented in `src/parse_csv.cxx` and declared in `include/parse_csv.h`. It supports three-column and four-column cleaned datasets.

During analysis, measurements must be compared over common time periods, accounting for missing observations and quality indicators.

## Team

- Elena Sanchez Nunez
- Gabriel Corrigan Fornvik
- Georges Nguyen
- Manuel Lorente Rodriguez

For individual responsibilities and deadlines, see [Workplan.md](Workplan.md).

## References

- [SMHI – Swedish Meteorological and Hydrological Institute](https://www.smhi.se/)
- [ROOT Data Analysis Framework](https://root.cern/)
- [fast-cpp-csv-parser](https://github.com/ben-strasser/fast-cpp-csv-parser)