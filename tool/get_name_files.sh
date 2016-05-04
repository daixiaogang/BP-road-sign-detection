#!/usr/bin/env bash
#
# Print filename files
#


$input = "/home/maiikeru/Downloads/GTSRB/Final_Training/Images/"
$output = "/tmp/negatives.dat"

find $input -type f  -name '*.ppm' > $output
