#!/bin/bash

cd ..

for filename in TIME_MEASUREMENT FIFO_1 PSJF_2 RR_3 SJF_4
do
  sudo dmesg --clear
  echo "Demo : ${filename}"
  sudo ./main < "test/${filename}.txt"
  sudo dmesg --notime | grep Project1 --color
  echo ""
done
