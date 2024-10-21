#!/bin/bash

# Define file paths
FILES=(
    "./bin/get_times.sh"
    "./src/Q3_bmjm501_main.cpp"
    "./analysis/graph.py"
)

# Use 'VIKING' as the default if no argument is provided
user_input=${1:-VIKING}  # Use the first command-line argument, or default to 'VIKING'

# Function to replace 'XT' with user input
replace_xt_with_input() {
    for file in "${FILES[@]}"; do
        if [[ -f "$file" ]]; then
            # Use sed to replace 'XT' with the user input string
            sed -i "s/XT/$user_input/g" "$file"
            echo "Replaced 'XT' with '$user_input' in $file"
        else
            echo "Warning: $file does not exist."
        fi
    done
}

# Function to restore 'XT'
restore_xt() {
    for file in "${FILES[@]}"; do
        if [[ -f "$file" ]]; then
            # Use sed to replace the user input string back with 'XT'
            sed -i "s/$user_input/XT/g" "$file"
            echo "Restored '$user_input' back to 'XT' in $file"
        else
            echo "Warning: $file does not exist."
        fi
    done
}

# First: Replace 'XT' with user input string
replace_xt_with_input

# ---- Perform some operation here ---- #
# Example operation: Just a placeholder echo
echo "Performing operations with modified files..."
# Add your custom operations here, like running some commands, scripts, etc.

# ------------------------------------- #

bash ./bin/get_times.sh

# Second: Replace the user input string back to 'XT'
restore_xt

# Confirm restoration
echo "Restored 'XT' in all files."
