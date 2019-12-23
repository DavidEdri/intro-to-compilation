#!/bin/bash
dir=./src
file=./input.code

while inotifywait -qqre modify "$dir" "$file";
do
    clean
    make run
done