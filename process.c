#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sched.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <sys/resource.h>
#include "process.h"

void unit_time() {
  volatile unsigned long i;
  for(i = 0; i < 1000000UL; i++);
}

int proc_assign_cpu (int pid, int core_id) {
  cpu_set_t mask;
  CPU_ZERO (&mask);
  CPU_SET(core_id, &mask);

  if(sched_setaffinity(pid, sizeof(cpu_set_t), &mask) != 0) {
    perror("sched_setaffinity");
    exit(1);
  }

  return 0;
}

int proc_exec (process proc) {
  int pid = fork();

  if(pid == -1) {
    perror("fork");
    return -1;
  }

  else if(pid == 0) {
    static const long BASE = 1e9;
    long start_time, finish_time;

    start_time = syscall(GET_TIME);

    for(int i = 0; i < proc.exec_time; i++)
      unit_time();

    finish_time = syscall(GET_TIME);

    #ifdef DEBUG
      fprintf(stderr, "[Project1] %d %ld.%09ld %ld.%09ld\n", getpid(), start_time / BASE, start_time % BASE, finish_time / BASE, finish_time % BASE);
    #endif

    syscall(PRINTK, getpid(), start_time, finish_time);

    exit(0);
  }

  proc_assign_cpu(pid, CHILD_CPU);
  return pid;
}

int proc_low_prio(int pid) {
	struct sched_param sp = { .sched_priority = 0 };

	int ret = sched_setscheduler(pid, SCHED_IDLE, &sp);

	if (ret < 0) {
		perror("sched_setscheduler");
		return -1;
	}

	return ret;
}

int proc_high_prio(int pid) {
  struct sched_param sp = { .sched_priority = 0 };

	int ret = sched_setscheduler(pid, SCHED_OTHER, &sp);

	if (ret < 0) {
		perror("sched_setscheduler");
		return -1;
	}

	return ret;
}
