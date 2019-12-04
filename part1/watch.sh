#!/bin/bash
dir=./src
file=./test.txt

while inotifywait -qqre modify "$dir" "$file" ;
do
    clear
    make run
done