#include <linux/sched.h>
#include <linux/kernel.h>

#include "lxmem.h"

static int get_process_count(void)
{
	struct task_struct *task;
	int count = 0;
    
	printk(KERN_INFO "PID\tName\n");
	for_each_process(task) {
		printk(KERN_INFO "[lxmem]%d   %s\n", task->pid, task->comm);
	count++;
	}
	return count;
}

static int get_process_pid(char* name)
{
	struct task_struct *task;
	int pid = 0;

	for_each_process(task) {
		printk(KERN_INFO "[lxmem]%d   %s  c%s[lxgetpid]\n", task->pid, task->comm,name);
		if (strstr(name,task->comm)) {
			pid = task->pid;
			printk(KERN_INFO "[lxmem]%d   %s [pid;%d]\n", task->pid, task->comm,pid);
			return pid;
		}
    	}
    return 0;
}

