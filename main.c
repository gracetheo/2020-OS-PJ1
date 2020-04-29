#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <string.h>
#include "process.h"
#include "scheduler.h"

int R (const void *a, const void *b) {
	return ((process *) a)->ready_time - ((process *) b)->ready_time;
}

int main (int argc, char *argv[]) {
  char sched_policy[5];
  int total_proc;
  process *proc;

  scanf("%s %d", sched_policy, &total_proc);

  proc = (process *) malloc(total_proc * sizeof(process));

  for(int i = 0; i < total_proc; i++) {
    scanf("%s %d %d", proc[i].name, &proc[i].ready_time, &proc[i].exec_time);
  }

	// sort processes based on ready time
  qsort(proc, total_proc, sizeof(process), R);

	for(int i = 0; i < total_proc; i++)
		proc[i].pid = -1; // all processes is not ready

  if(!strcmp(sched_policy, "FIFO")) sched_FIFO(proc, total_proc);
  else if(!strcmp(sched_policy, "RR")) sched_RR(proc, total_proc);
  else if(!strcmp(sched_policy, "SJF")) sched_SJF(proc, total_proc);
  else if(!strcmp(sched_policy, "PSJF")) sched_PSJF(proc, total_proc);
  else {
    fprintf(stderr, "Invalid schedule policy.");
    return 0;
  }

  return 0;
}
