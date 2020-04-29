#include <sys/types.h>

#define GET_TIME 333
#define PRINTK 334

#define PARENT_CPU 0
#define CHILD_CPU 1

void unit_time();

typedef struct {
  char name[32];  // process name
  int ready_time; // process ready time
  int exec_time;  // process left execution time
  pid_t pid;      // process id
} process;

int proc_assign_cpu (int pid, int core_id);
int proc_exec (process proc);
int proc_low_prio (int pid);
int proc_high_prio (int pid);
