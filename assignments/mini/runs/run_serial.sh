#!/bin/bash

# Define the files to search and modify
MAIN_FILE="./src/main.cpp"
MAKEFILE="./Makefile"

# Define the placeholder for original content
ORIGINAL_MAIN_FILE="$MAIN_FILE.bak"
ORIGINAL_MAKEFILE="$MAKEFILE.bak"

# Create backups of the original files
cp "$MAIN_FILE" "$ORIGINAL_MAIN_FILE"
cp "$MAKEFILE" "$ORIGINAL_MAKEFILE"

# Search and replace in main.cpp
if grep -q "XT" "$MAIN_FILE"; then
    echo "Replacing 'XT' with 'execute_serial()' in $MAIN_FILE..."
    sed -i 's/XT/execute_serial()/g' "$MAIN_FILE"
else
    echo "'XT' not found in $MAIN_FILE."
fi

# Search and replace in Makefile
if grep -q "XT" "$MAKEFILE"; then
    echo "Replacing 'XT' with 'g++' in $MAKEFILE..."
    sed -i 's/XT/g++/g' "$MAKEFILE"
else
    echo "'XT' not found in $MAKEFILE."
fi

make clean
make
./bin/main

# Restore original files
mv "$ORIGINAL_MAIN_FILE" "$MAIN_FILE"
mv "$ORIGINAL_MAKEFILE" "$MAKEFILE"
