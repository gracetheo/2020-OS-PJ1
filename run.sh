#!/bin/bash

sudo dmesg --clear
sudo ./main < "test/TIME_MEASUREMENT.txt" > "output/TIME_MEASUREMENT_stdout.txt"
sudo dmesg --notime | grep Project1 > "output/TIME_MEASUREMENT_dmesg.txt"

for filename in FIFO PSJF RR SJF
do
  for i in 1 2 3 4 5
  do
    sudo dmesg --clear
    sudo ./main < "test/${filename}_${i}.txt" > "output/${filename}_${i}_stdout.txt"
    sudo dmesg --notime | grep Project1 > "output/${filename}_${i}_dmesg.txt"
  done
done
