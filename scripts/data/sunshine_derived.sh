#!/bin/bash

# This script takes 3 arguments: the path to the raw data file, the path to the output file, and the number of lines to skip at the beginning of the raw data file.

if [ "$#" -ne 3 ]; then
    echo "Usage: $0 <raw_data_file> <output_file> <lines_to_skip>"
    exit 1
fi

RAW_DATA_FILE="$1"
OUTPUT_FILE="$2"
LINES_TO_SKIP="$3"

# -n +$3: start reading at line $3, -d ';': use semicolon as delimiter, -f1,2,3,4,5,6: keep only the first 6 columns
tail -n +$3 "$RAW_DATA_FILE" | cut -d';' -f1,2,3,4,5,6 > "$OUTPUT_FILE"
