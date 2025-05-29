#!/bin/bash

if [ $# -eq 0 ]; then
  echo "Please provide a case name, e.g., 'my_case.txt'"
  exit 1
fi
case="$1"
temp_case="temp_case.txt"
# echo $case

cp "$case" "$temp_case"
sleep 0.1
# Path to your executable
executable_path="./lineseg.exe"

# Execute the program
# $executable_path "$@"
$executable_path

rm "$temp_case"