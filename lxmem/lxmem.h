#ifndef DV_MEM_H_
#define DV_MEM_H_

#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/cdev.h>
#include <linux/fs.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/sched/mm.h>
#include <linux/kallsyms.h>
#include <linux/kprobes.h>


#define DEV_FILE_NAME "lxmem"

struct mem_tool_device {
	struct cdev cdev;		//设备结构体
	struct device *dev;
	int max;
};

static struct mem_tool_device *memdev;

/**
 * 用于和用户空间进程通信的结构体
*/
struct mem_process_rw {
	pid_t pid;
	size_t virt_addr;
	void *base;
	size_t len;
};

static struct mem_process_rw process_rw_data[1];

struct process_pid {
	pid_t pid;
	char comm[32];
};


struct vm_area_process {
    unsigned long start;
    unsigned long end;
    char perms[5];
    unsigned long offset;
    unsigned int major;
    unsigned int minor;
    unsigned long inode;
    char path[128];
};

#define MEMIOCTMAPC                                              _IOR('M', 0x2, int)
#define MEMIOCTMAP                                               _IOWR('M', 0x3, void *)
#define MEMIOCTGETPIDC						 _IOR('P', 0x4, int)
#define MEMIOCTGETPID						 _IOWR('P', 0x5, void *)

static char name[32];

//设备号
static dev_t mem_tool_dev_t;
//设备类
static struct class *mem_tool_class;

static struct process_pid process_pid_data[1];



static inline struct mm_struct *get_task_mm_by_vpid(pid_t nr);

static inline ssize_t mem_tool_rw_core(const char __user *buf, size_t (*mem_fn)(size_t , char *, size_t, pte_t *));



static int mem_tool_open(struct inode *inode, struct file *filp);

static int mem_tool_release(struct inode *inode, struct file *filp);

static ssize_t mem_tool_read(struct file *file, char __user *buf, size_t count, loff_t *pos);

static ssize_t mem_tool_write(struct file *file, const char __user *buf, size_t count, loff_t *pos);

static long mem_tool_ioctl(struct file *filp, unsigned int cmd, unsigned long arg);


/**
 * 设备操作函数，对设备文件执行相应操作时，将调用此处注册上的函数
*/
static const struct file_operations mem_tool_fops = {
	.owner		= THIS_MODULE,
	.open           = mem_tool_open,	//打开设备文件
	.release        = mem_tool_release,	//关闭设备文件
	.read           = mem_tool_read,	//读取数据
	.write          = mem_tool_write,	//写入数据
	.llseek         = no_llseek,		//不支持随机读写
	.compat_ioctl   = mem_tool_ioctl,	//设备控制操作	32位
	.unlocked_ioctl = mem_tool_ioctl,	//设备控制操作	64位，非锁定
};



static int user_custom_init(void);

static void user_custom_exit(void);

static int __init dev_mem_tool_init(void);

static void __exit dev_mem_tool_exit(void);

#endif // DV_MEM_H_
