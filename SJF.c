#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <sched.h>
#include <unistd.h>
#include <sys/wait.h>
#include "process.h"
#include "scheduler.h"

int SJF_process (process *proc, int total_proc) {
	// non-preemptive, let the running program to run until finish
	if(run_id != -1) return run_id;

  int ret = -1;
	for(int i = 0; i < total_proc; i++) {
		// proc[i] is not ready or finished
		if(proc[i].pid == -1 || proc[i].exec_time == 0)
			continue;

		// check for proc with lowest execution time left
		if(ret == -1 || proc[i].exec_time < proc[ret].exec_time)
			ret = i;
	}

  return ret;
}

int sched_SJF (process *proc, int total_proc) {
	proc_assign_cpu(getpid(), PARENT_CPU);
	proc_high_prio(getpid());

	curr_time = 0;
	run_id = -1;
	finished = 0;

	while(1) {
		// run_id process is finished
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
			if(proc[i].ready_time == curr_time) {
				proc[i].pid = proc_exec(proc[i]);
				proc_low_prio(proc[i].pid);
			}
		}

		// check for next process
		int next_id = SJF_process(proc, total_proc);

		if(next_id != -1) {
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
