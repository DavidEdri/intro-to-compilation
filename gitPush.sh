#!/usr/bin/env sh


if [ -z "$1" ]
then
      echo "must have a message to commit"
else
    git add .
    git commit -am "$1"
    git push origin master
fi