#include "mem.h"
#include "maps.h"
#include "pid.h"
#include "lxmem.h"




static inline struct mm_struct *get_task_mm_by_vpid(pid_t nr)
{
	struct task_struct *task;

	task = pid_task(find_vpid(nr), PIDTYPE_PID);
	if (!task)
		return NULL;
	
	return get_task_mm(task);
}
/**
 * mem_tool_rw_core - 内存读写核心函数
 * @buf: 用户空间传入的缓冲区指针
 * @mem_fn: 内存读写函数指针，可以是read_pte2phy_addr或者write_pte2phy_addr
 *
 * 该函数用于在用户空间和内核空间之间进行内存读写操作。
 * 它根据用户空间传入的缓冲区中的数据结构来确定读写的目标进程、虚拟地址、长度等信息，
 * 并调用相应的内存读写函数(mem_fn)来执行实际的读写操作。
 *
 * @return	成功读写的字节数，如果出现错误则返回负值。
 */
static inline ssize_t mem_tool_rw_core(const char __user *buf, size_t (*mem_fn)(size_t , char *, size_t, pte_t *))
{
	//要访问的进程任务结构体
	struct task_struct *task;
	struct mm_struct *mm;
	size_t max_page;
	size_t page_addr;
	size_t copied = 0;
	
	char *buffer;
	//要操作的虚拟地址起始指针
	size_t virt_addr;
	size_t len;
	//要操作的虚拟地址对应的页表项
	pte_t *pte = NULL;

	// 从用户空间拷贝通信数据到内核空间
	if (copy_from_user(process_rw_data, buf, sizeof(struct mem_process_rw)) != 0)
		return -EFAULT;

	// 根据进程ID获取对应的任务结构体
	task = pid_task(find_vpid(process_rw_data->pid), PIDTYPE_PID);
	if (!task)
		return -EFAULT;	

	// 获取目标任务的内存描述符
	mm = get_task_mm(task);
	if (!mm)
		return -EFAULT;
	
	virt_addr = process_rw_data->virt_addr;
	buffer = process_rw_data->base;
	len = process_rw_data->len;

	// 逐页进行内存读写操作
	while (len > 0) {
		// 获取虚拟地址对应的物理页地址及页表项
		page_addr = get_process_page_addr(mm, virt_addr, &pte);
		max_page = get_min_step_inside_page(virt_addr, min(len, PAGE_SIZE));

		if (page_addr == 0)
			goto err_phy_addr_none;
	
		// 调用指定的内存读写函数进行读写操作
		copied += mem_fn(virt_addr, buffer, max_page, pte);
		
	err_phy_addr_none:
		len -= max_page;
		buffer += max_page;
		virt_addr += max_page;
	
	}
	// 释放内存描述符
	mmput(mm);
	return copied;
}
/**
 * 打开设备文件事件
*/
static int mem_tool_open(struct inode *inode, struct file *filp)
{
	filp->private_data = memdev;
	printk("success open file\n");
	return 0;
}
/**
 * 关闭设备文件事件
*/
static int mem_tool_release(struct inode *inode, struct file *filp)
{
	printk("success close file\n");
	return 0;
}
/**
 * 读取数据事件
*/
static ssize_t mem_tool_read(struct file *file, char __user *buf, size_t count, loff_t *pos)
{
	return mem_tool_rw_core(buf, read_pte2phy_addr);
}
/**
 * 写入数据事件
*/
static ssize_t mem_tool_write(struct file *file, const char __user *buf, size_t count, loff_t *pos)
{	
	return mem_tool_rw_core(buf, write_pte2phy_addr);
}
/**
 * 设备控制操作
*/
static long mem_tool_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
	switch (cmd) {
	
	case MEMIOCTMAPC:
		return get_process_maps_count(arg);
	//读取内存映射区域信息
	case MEMIOCTMAP:
		if (copy_from_user(process_rw_data, (void *)arg, sizeof(struct mem_process_rw)) != 0)
			return -EFAULT;

		return get_process_maps(process_rw_data->pid, process_rw_data->base, process_rw_data->len);		
	case MEMIOCTGETPIDC:
		return get_process_count();
		
	case MEMIOCTGETPID: 
		if (copy_from_user(name, (void *)arg, sizeof(name)) != 0)
			return -EFAULT;
		//printk("【lxmemPID ：%s  ptr:%p \n",name,arg);
		return get_process_pid(name);
	default:
		break;
	
	}
	return 0;
}

static int user_custom_init(void)
{
	return 1;
}

static void user_custom_exit(void)
{
}
/**
 * 加载函数
*/
static int __init dev_mem_tool_init(void)
{
	int ret;
	//动态分配设备号
	ret = alloc_chrdev_region(&mem_tool_dev_t, 0, 1, DEV_FILE_NAME);
	if (ret < 0) {
		//分配设备号失败
		printk("failed to allocate device numbers: %d\n", ret);
		return ret;
	}
	//GFP_KERNEL是kmalloc最常使用的标志，没有什么特殊的地方
	memdev = kmalloc(sizeof(struct mem_tool_device), GFP_KERNEL);
	if (!memdev) {
		//内存分配失败
		printk("failed to allocate memory: %d\n", ret);
		goto done;
	}		
	//创建设备类
	mem_tool_class = class_create(THIS_MODULE, DEV_FILE_NAME);
	if (IS_ERR(mem_tool_class)) {
		//创建设备类失败
		printk("failed to create class: %d\n", ret);
		goto done;
	}
	memset(memdev, 0, sizeof(struct mem_tool_device));

	//初始化设备结构体，将相应操作注册给系统
	cdev_init(&memdev->cdev, &mem_tool_fops);
	memdev->cdev.owner = THIS_MODULE;
	memdev->cdev.ops = &mem_tool_fops;
	//添加字符设备
	ret = cdev_add(&memdev->cdev, mem_tool_dev_t, 1);
	if (ret) {
		//添加失败
		printk("failed to create device: %d\n", ret);
		goto done;
	}
	//创建设备节点，，执行后将在/dev下生成设备文件
	memdev->dev = device_create(mem_tool_class, NULL, mem_tool_dev_t, NULL, "%s", DEV_FILE_NAME);
	if (IS_ERR(memdev->dev)) {
		//创建失败
		printk("failed to create device: %d\n", ret);
		goto done;
	}
	//return 1;我也不知道为什么要写这玩意
	if (!user_custom_init()) {
		printk("failed to allocate page memory: %d\n", ret);
		goto done;
	}	
	
	printk("device create success %s\n", DEV_FILE_NAME);
	return 0;

done:
	return ret;
}

module_init(dev_mem_tool_init);

/**
 * 卸载函数
*/
static void __exit dev_mem_tool_exit(void)
{
	device_destroy(mem_tool_class, mem_tool_dev_t);
	class_destroy(mem_tool_class);
	
	cdev_del(&memdev->cdev);
	kfree(memdev);
	unregister_chrdev_region(mem_tool_dev_t, 1);
	
	user_custom_exit();

	printk("device destory success %s\n", DEV_FILE_NAME);
}


module_exit(dev_mem_tool_exit);

MODULE_LICENSE("GPL");
