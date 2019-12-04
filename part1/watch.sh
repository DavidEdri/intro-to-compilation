#!/bin/bash
dir=./src
file=./test.txt

while inotifywait -qqre modify "$dir" "$file" ;
do
    make run
done