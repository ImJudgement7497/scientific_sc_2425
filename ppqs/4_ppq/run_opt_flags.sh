#!/bin/bash

MAKEFILE="Makefile"            
OUTPUT_FILE="./results/opt_flags_log.txt" 
LOG_FILE="simple_md.log"      

OPT_FLAGS=$(sed -n 's/^OPT_FLAGS\s*=\s*//p' "$MAKEFILE")

echo "Current OPT_FLAGS: $OPT_FLAGS" >> "$OUTPUT_FILE"
echo "---------------------------------" >> "$OUTPUT_FILE"

make simple_md_C_opt


{ /usr/bin/time -f "Execution Time: %E" ./simple_md_C_opt > "$LOG_FILE"; } 2>&1 | tee -a "$LOG_FILE"

LAST_LINE=$(tail -n 1 "$LOG_FILE")

echo "$LAST_LINE" >> "$OUTPUT_FILE"
echo "---------------------------------" >> "$OUTPUT_FILE"

echo -e "\n" >> "$OUTPUT_FILE"

echo "OPT_FLAGS and the last line of program output saved to $OUTPUT_FILE"

diff simple_md.log ./results/simple_md.log.ref

make clean