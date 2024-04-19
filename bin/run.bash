#!/bin/bash

FILES=('01' '02' '11' '15' '27' '29' '32' '38' '42')
OUTPUT=$1

if [ -z "$OUTPUT" ]; then
    echo "Error: argument not found"
    echo "ex: /bash/xyz.bash <operation>"
    echo
else
    for file in "${FILES[@]}"; do
        if [ ! -d "samples/$OUTPUT" ]; then
            echo "creating directory: $OUTPUT"
            mkdir -p "samples/$OUTPUT" && 
                echo "directory created: $OUTPUT" || echo "directory not created: $OUTPUT"
            echo
        fi
        ./build/Main --file=data/drive_00"$file"_sync/image_02/data --output="$OUTPUT"/drive_00"$file"_sync_"$OUTPUT"
    done
fi

