#!/bin/bash

# Specify file paths
MAKEFILE="Makefile"            # Path to the Makefile
OUTPUT_FILE="opt_flags_log.txt" # File to track OPT_FLAGS and results
LOG_FILE="simple_md.log"      # File where the program logs its output

# Step 1: Read OPT_FLAGS from Makefile
OPT_FLAGS=$(sed -n 's/^OPT_FLAGS\s*=\s*//p' "$MAKEFILE")

# Step 2: Write OPT_FLAGS to the output file
echo "Current OPT_FLAGS: $OPT_FLAGS" >> "$OUTPUT_FILE"
echo "---------------------------------" >> "$OUTPUT_FILE"

make clean
make simple_md_C_opt

# Step 3: Run the program and time it, capturing both output and timing
{ /usr/bin/time -f "Execution Time: %E" ./simple_md_C_opt > "$LOG_FILE"; } 2>&1 | tee -a "$LOG_FILE"
# Step 4: Read the last line of the log file
LAST_LINE=$(tail -n 1 "$LOG_FILE")

# Step 5: Append the last line to OUTPUT_FILE
echo "$LAST_LINE" >> "$OUTPUT_FILE"
echo "---------------------------------" >> "$OUTPUT_FILE"

# Step 6: Add a separator for clarity between runs
echo -e "\n" >> "$OUTPUT_FILE"

echo "OPT_FLAGS and the last line of program output saved to $OUTPUT_FILE"

diff simple_md.log simple_md.log.ref