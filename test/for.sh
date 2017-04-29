#!/bin/bash

for (( i=1; i <= 5; i++ ))
do
 date >> test.txt
 echo "Random number $i: $RANDOM"
done


i=1
for file in /etc/[abcd]*.conf
do
  python t.py 130 40 450 >> test.txt
  cat test.txt
done