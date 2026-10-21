#!/bin/bash

# Check arguments
if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <raw_data_file> <output_file> <lines_to_skip>"
    exit 1
fi

RAW_DATA_FILE="$1"
OUTPUT_FILE="$2"
LINES_TO_SKIP="$3"

# Skip metadata and keep date and precipitation columns
tail -n +"$((LINES_TO_SKIP + 1))" "$RAW_DATA_FILE" |
    cut -d';' -f3,4,5 > "$OUTPUT_FILE"

# Add column headers
sed -i '1i Date,Precipitation_mm,Quality' "$OUTPUT_FILE"

# Replace semicolons with commas
sed -i 's/;/,/g' "$OUTPUT_FILE"