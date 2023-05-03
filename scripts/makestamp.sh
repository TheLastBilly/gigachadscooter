#!/bin/sh

filename=$(basename -- "$1")
filename="${filename%.*}"
convert $1 -compress none "$filename"_0.ppm
ppmquant 224 "$filename"_0.ppm > "$filename"_1.ppm
if [ -z "$2" ]; then
    pnmnoraw "$filename"_1.ppm > "$filename"_stamp.ppm
else
    pnmnoraw "$filename"_1.ppm > $2
fi
rm "$filename"_0.ppm "$filename"_1.ppm 
