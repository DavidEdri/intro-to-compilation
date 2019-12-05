#!/bin/bash
dir=./src
file=./input.c

while inotifywait -qqre modify "$dir" "$file";
do
    clear
    make run
done