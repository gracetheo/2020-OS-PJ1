#include <sys/types.h>

#define TIME_QUANTUM 500

static int curr_time; 	// current time
static int run_id;	// id of run_id process, -1 if none is run_id
static int finished; 	// total of finished process
static int last_switch;	// time of last switch

int sched_FIFO (process *proc, int total_proc);
int sched_RR (process *proc, int total_proc);
int sched_SJF (process *proc, int total_proc);
int sched_PSJF (process *proc, int total_proc);
