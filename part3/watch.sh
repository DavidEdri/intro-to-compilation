#!/bin/bash
dir=./src
file=./input.code

while inotifywait -qqre modify "$dir" "$file";
do
    clear
    make test
    printf "\n\n\n"
    make run
done