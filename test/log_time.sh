#!/bin/bash

# Directory containing the map data files
MAP_DIR="../data"
# Output log file
LOG_FILE="run_log.txt"

# Function to get dimensions of the map (n x m)
get_map_dimensions() {
    local file="$1"
    # Read the first line to get 'n' (number of columns)
    local n=$(head -1 "$file" | wc -w)
    # Count the number of lines to get 'm' (number of rows)
    local m=$(wc -l < "$file")
    # Increment m by one, except when m is zero
    if [ "$m" -gt 0 ]; then
        m=$((m + 1))
    fi
    echo "${m}x${n}"
}

# Clear previous log file
echo "" > "$LOG_FILE"

# Function to process map files with a given algorithm
process_maps() {
    local algorithm="$1"
    local ALGORITHM_NAME=$(if [ "$algorithm" -eq 1 ]; then echo "DFS"; else echo "BFS"; fi)

    for map_file in $(seq 1 8); do
        # Construct the map file name
        FILE_NAME="${MAP_DIR}/map${map_file}.txt"
        # Get map dimensions
        DIMENSIONS=$(get_map_dimensions "$FILE_NAME")
        # Run the program and capture its output
        OUTPUT=$(../src/main "$algorithm" 10 "$FILE_NAME" 2>&1)
        # Extract the time taken from the output
        TIME_TAKEN=$(echo "$OUTPUT" | grep -oP 'Time taken: \K[0-9]+')
        # Log details
        LOG_ENTRY="Algorithm: $ALGORITHM_NAME - $FILE_NAME (${DIMENSIONS}) - Time: ${TIME_TAKEN} nanoseconds"
        echo "$LOG_ENTRY" >> "$LOG_FILE"
    done
}

# Process maps with DFS first, then BFS
process_maps 1
process_maps 0

# Display the log file content
cat "$LOG_FILE"
