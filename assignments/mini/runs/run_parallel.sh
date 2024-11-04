#!/bin/bash

# Define the files to search and modify
MAKEFILE="./Makefile"
MAIN_FILE="./src_parallel/main.cpp"
# Define the placeholder for original content
ORIGINAL_MAKEFILE="$MAKEFILE.bak"
ORIGINAL_MAIN_FILE="$MAIN_FILE.bak"

cp "$MAKEFILE" "$ORIGINAL_MAKEFILE"
cp "$MAIN_FILE" "$ORIGINAL_MAIN_FILE"

# if grep -q "XT" "$MAIN_FILE"; then
#     echo "Replacing 'XT' with 'execute_parallel()' in $MAIN_FILE..."
#     sed -i 's/XT/execute_parallel()/g' "$MAIN_FILE"
# else
#     echo "'XT' not found in $MAIN_FILE."
# fi

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
mpiexec -n 2 ./bin/main

# Restore original files
mv "$ORIGINAL_MAKEFILE" "$MAKEFILE"
mv "$ORIGINAL_MAIN_FILE" "$MAIN_FILE"
