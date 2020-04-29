#include <linux/linkage.h>
#include <linux/kernel.h>

asmlinkage void sys_pj1_print(int pid, long start_time, long finish_time) {
    static const long BASE = 1e9;
    printk(KERN_INFO "[Project1] %d %ld.%09ld %ld.%09ld\n", pid, start_time / BASE, start_time % BASE, finish_time / BASE, finish_time % BASE);
}

// syscall 334
