#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include "process.h"
#include "scheduler.h"

int cmp (const void *a, const void *b) {
	return ((process *) a)->ready_time - ((process *) b)->ready_time;
}

int RR_process (process *proc, int total_proc) {
	int ret = -1;

	// no running process, check for next available process
	if(run_id == -1) {
		for(int i = 0; i < total_proc; i++) {
			// proc[i] is ready and not finished.
			if(proc[i].pid != -1 && proc[i].exec_time > 0) {
				ret = i;
				break;
			}
		}
	}

	// 1 time quantum is up
	else if ((curr_time - last_switch) % TIME_QUANTUM == 0) {
		// preempt the running process
		proc[run_id].ready_time = curr_time;
		qsort(proc, total_proc, sizeof(process), cmp);

		// check for the next ready pprocess in queue
		ret = (run_id + 1) % total_proc;

		while(proc[ret].pid == -1 || proc[ret].exec_time == 0)
			ret = (ret + 1) % total_proc;
	}

	else ret = run_id;

  return ret;
}

int sched_RR (process *proc, int total_proc) {
	proc_assign_cpu(getpid(), PARENT_CPU);
	proc_high_prio(getpid());

	curr_time = 0;
	run_id = -1;
	finished = 0;

	while(1) {
		// run_id process is done
		if(run_id != -1 && proc[run_id].exec_time == 0) {
			waitpid(proc[run_id].pid, NULL, 0);
			printf("%s %d\n", proc[run_id].name, proc[run_id].pid);
			fflush(stdout);
			run_id = -1;
			finished++;

			// all processes is finished
			if(finished == total_proc) break;
		}

		// check for ready process
		for(int i = 0; i < total_proc; i++) {
			if(proc[i].ready_time == curr_time && proc[i].pid == -1) {
				proc[i].pid = proc_exec(proc[i]);
				proc_low_prio(proc[i].pid);
			}
		}

		// check for next process
		int next_id = RR_process(proc, total_proc);

		if(next_id != -1) {
			last_switch = curr_time;
			// context switch
			if(run_id != next_id) {
				proc_high_prio(proc[next_id].pid);
				proc_low_prio(proc[run_id].pid);
				run_id = next_id;
			}
		}

		unit_time();

		if(run_id != -1)
			proc[run_id].exec_time--;

		curr_time++;
	}

	return 0;
}
