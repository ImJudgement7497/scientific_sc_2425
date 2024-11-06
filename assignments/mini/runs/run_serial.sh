#!/bin/bash

# Define the files to search and modify
MAKEFILE="./Makefile"

# Define the placeholder for original content
ORIGINAL_MAKEFILE="$MAKEFILE.bak"

# Create backups of the original files
cp "$MAKEFILE" "$ORIGINAL_MAKEFILE"


# Search and replace in Makefile
if grep -q "XT" "$MAKEFILE"; then
    echo "Replacing 'XT' with 'g++' in $MAKEFILE..."
    sed -i 's/XT/g++/g' "$MAKEFILE"
else
    echo "'XT' not found in $MAKEFILE."
fi

if grep -q "YT" "$MAKEFILE"; then
    echo "Replacing 'YT' with 'src_serial' in $MAKEFILE..."
    sed -i 's/YT/src_serial/g' "$MAKEFILE"
else
    echo "'YT' not found in $MAKEFILE."
fi

make clean
make
rm ./logs/serial/**
./bin/main

# Restore original files
mv "$ORIGINAL_MAKEFILE" "$MAKEFILE"
