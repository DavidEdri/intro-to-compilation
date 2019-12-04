#!/bin/bash

while inotifywait -qqre modify ./src ./test.txt;
do
    clear
    make run
done