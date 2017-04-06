#!/bin/bash

rm -f $1.o
rm -f $1

gcc -Wall $1.c -o $1

if ["$?" != "0"]; then
  echo "COMPILATION ERROR"
fi

./$1
