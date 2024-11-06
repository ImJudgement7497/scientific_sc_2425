#!/bin/bash

# Define the files to search and modify
MAKEFILE="./Makefile"
# Define the placeholder for original content
ORIGINAL_MAKEFILE="$MAKEFILE.bak"

cp "$MAKEFILE" "$ORIGINAL_MAKEFILE"


# Search and replace in Makefile
if grep -q "XT" "$MAKEFILE"; then
    echo "Replacing 'XT' with 'mpic++' in $MAKEFILE..."
    sed -i 's/XT/mpic++/g' "$MAKEFILE"
else
    echo "'XT' not found in $MAKEFILE."
fi

if grep -q "YT" "$MAKEFILE"; then
    echo "Replacing 'YT' with 'src_parallel' in $MAKEFILE..."
    sed -i 's/YT/src_parallel/g' "$MAKEFILE"
else
    echo "'YT' not found in $MAKEFILE."
fi

make clean
make

rm ./logs/parallel/**
mpiexec -n 2 ./bin/main

# Restore original files
mv "$ORIGINAL_MAKEFILE" "$MAKEFILE"
