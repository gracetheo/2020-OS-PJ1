main: main.o process.o FIFO.o RR.o SJF.o PSJF.o
	gcc main.o process.o FIFO.o RR.o SJF.o PSJF.o -o main
main.o: main.c Makefile
	gcc main.c -c
process.o: process.c process.h Makefile
	gcc process.c -c
FIFO.o: FIFO.c scheduler.h Makefile
	gcc FIFO.c -c
RR.o: RR.c scheduler.h Makefile
	gcc RR.c -c
SJF.o: SJF.c scheduler.h Makefile
	gcc SJF.c -c
PSJF.o: PSJF.c scheduler.h Makefile
	gcc PSJF.c -c
clean:
	rm -rf *o
run:
	sudo ./main
