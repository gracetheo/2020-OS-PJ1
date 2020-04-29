# 2020 Operating System Project 1 - Process Scheduling

To compile program :
```
$ make
$ sudo ./main
$ dmesg -c | grep Project1
```

Input :
```
S // the scheduling policy, one of the following strings: FIFO, RR, SJF, PSJF.
N // the number of processes
N1 R1 T1
N2 R2 T2
…
Nn Rn Tn
//Ni - a character string with a length less than 32 bytes, specifying the name of the i-th process.
//Ri - a non-negative integer, specifying the ready time of the i-th process.
//Ti - a non-negative integer, specifying the execution time of the i-th process.
```

To run all test cases provided :
```
$ make
$ bash run.sh
```
