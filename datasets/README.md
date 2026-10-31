# Climate Datasets

## About

This directory contains meteorological data from the Swedish Meteorological and Hydrological Institute (SMHI), used to compare climate conditions in Lund and Umeå. We use measurements of temperature, precipitation, and sunshine duration.

The data is divided into original measurements (`raw_data/`) and cleaned datasets (`derived_data/`).

## Directory structure

```text
datasets/
├── datasets.tgz
├── raw_data/
│   ├── precipitation_lund_53430.csv
│   ├── precipitation_umea_robacksdalen_140490.csv
│   ├── sunshine_lund_53445.csv
│   ├── sunshine_umea_140615.csv
│   ├── smhi-opendata_1_53430_20261007_164857_Lund.csv
│   └── smhi-opendata_1_140480_20261007_164814_Umeå_Flygplats.csv
│
├── derived_data/
│   ├── sunshine/
│   │   ├── lund_sunshine_derived.csv
│   │   └── umea_sunshine_derived.csv
│   ├── temperature/
│   │   ├── lund_temp_derived.csv
│   │   └── Umeå_temp_derived.csv
│   └── precipitation/
│       ├── precipitation_lund_derived.csv
│       └── precipitation_umea_derived.csv
│
└── README.md
```

## Raw data

The original SMHI files contain meteorological observations together with metadata, measurement units, station information, and quality indicators.

The raw files are preserved separately from the processed data.

The course-provided temperature datasets are also available in the `datasets.tgz` archive.

To extract the archive, if needed:

```bash
tar -xzvf datasets/datasets.tgz -C datasets/
```

## Derived data

The derived datasets are prepared for easier import into C++.

Processing may include:

- Removing metadata and unnecessary header lines.
- Selecting the relevant measurement columns.
- Converting column separators into a consistent format.
- Removing unnecessary columns.


### Generating derived datasets

Run the following commands from the repository root.

The scripts accept three arguments:

```bash
bash scripts/data/<script>.sh <raw_file> <output_file> <lines_to_skip>
```

For example, to generate the Lund solar irradiance dataset:

```bash
bash scripts/data/sunshine_derived.sh \
  datasets/raw_data/sunshine_lund_53445.csv \
  datasets/derived_data/sunshine/lund_sunshine_derived.csv \
  N
```

Replace `N` with the first data line number in the raw file.


### Sunshine duration

Location: `derived_data/sunshine/`

The cleaned sunshine datasets contain four fields, without headers:

```text
Date,Time,Irradiance,Quality
```

The cleaned solar irradiance datasets contain four columns, without headers:

```text
Date,Time,Irradiance,Quality
```

- **Date:** Date of the measurement (YYYY-MM-DD).
- **Time:** Time of the measurement in UTC (HH:MM:SS).
- **Irradiance:** Mean incoming solar power per unit area, measured in W/m², averaged over one hour.
- **Quality:** Indicates the quality of the measurement:
  - `G`: Good-quality measured data.
  - `Y`: Estimated or interpolated values where direct measurements were unavailable.

To obtain this data the following can be executed:

```bash
bash scripts/data/sunshine_derived.sh \
  datasets/raw_data/sunshine_lund_53445.csv \
  datasets/derived_data/sunshine/lund_sunshine_derived.csv \
  12
```

### Precipitation

Location: `derived_data/precipitation/`

The cleaned precipitation datasets contain three fields:

```text
Date,Precipitation,Quality
```

### Temperature

Location: `derived_data/tmperature/`

The cleaned temperature datasets contain four fields:

```text
Date,Time,Temperature,Quality
```


## Data processing scripts

The scripts are located in:

```text
scripts/data/
```

They create new CSV files from the original SMHI data to import into the C++ program.

Each script should be run with the arguments specified in its usage instructions.

## C++ data import

The processed datasets are imported using the `parseCSV()` function defined in:

```text
include/parse_csv.h
src/parse_csv.cxx
```

The importer handles datasets with three or four columns, depending on the type of measurement.

During import, dates are separated into two numerical values:
- **Year:** Extracted from the original date (YYYY).
- **Day of year:** Converted into an integer between 1–365 (or 366 for leap years).

For hourly measurements, the time is also converted into an integer between 0–23.

## Data sources

The solar irradiance and precipitation datasets were downloaded
from the following SMHI measurement stations:

| Parameter | City | Station | Source |
|---|---|---|---|
| Solar irradiance | Lund | 53445 | [SMHI](https://www.smhi.se/data/hitta-data-for-en-plats/ladda-ner-vaderobservationer/irradiance/53445) |
| Solar irradiance | Umeå | 140615 | [SMHI](https://www.smhi.se/data/hitta-data-for-en-plats/ladda-ner-vaderobservationer/irradiance/140615) |
| Precipitation | Lund | 53430 | [SMHI](https://www.smhi.se/data/hitta-data-for-en-plats/ladda-ner-vaderobservationer/precipitation24HourSum/53430) |
| Precipitation | Umeå | 140490 | [SMHI](https://www.smhi.se/data/hitta-data-for-en-plats/ladda-ner-vaderobservationer/precipitation24HourSum/140490) |

Temperature datasets were provided as part of the MNXB11 course material.
Original CSV files are stored in `datasets/raw_data/`.

Data usage conditions: [SMHI Terms of Use](https://www.smhi.se/data/om-smhis-data/villkor-for-anvandning).
