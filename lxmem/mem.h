#include <asm/page.h>
#include <asm/pgtable.h>
#include <linux/mm.h>
#include <linux/uaccess.h>
#include <linux/highmem.h>

#define pte_to_phys(virt_addr, pte_value) ((virt_addr & ~PAGE_MASK) | (pte_value & PAGE_MASK))

#define __phys_to_page(paddr)	(pfn_to_page(PFN_DOWN(paddr)))




static size_t get_min_step_inside_page(size_t addr, size_t size);

static size_t get_process_page_addr(struct mm_struct *mm, size_t virt_addr, pte_t **pte);

static size_t read_pte2phy_addr(size_t virt_addr, char *buf, size_t len, pte_t *pte);

static size_t write_pte2phy_addr(size_t virt_addr, char *buf, size_t len, pte_t *pte);

static inline size_t get_min_step_inside_page(size_t addr, size_t size)
{
	return min(PAGE_SIZE - (addr & (PAGE_SIZE - 1)), size);
}

static size_t get_process_page_addr(struct mm_struct *mm, size_t virt_addr, pte_t **pte)
{
	pgd_t *pgd;
	p4d_t *p4d;
	pud_t *pud;
	pmd_t *pmd;
	
	pgd = pgd_offset(mm, virt_addr);
	if (pgd_none(*pgd))
		return 0;
	
	p4d = p4d_offset(pgd, virt_addr);
	if (p4d_none(*p4d))
		return 0;
		
	pud = pud_offset(p4d, virt_addr);
	if (pud_none(*pud))
		return 0;
		
	pmd = pmd_offset(pud, virt_addr);
	if (pmd_none(*pmd))
		return 0;
	
	*pte = pte_offset_kernel(pmd, virt_addr);
	if (pte_none(**pte))
		return 0;
	
	return pte_val(**pte);
}
/**
 * 将一个虚拟地址对应的物理地址，从内核空间复制到用户空间
 * @param virt_addr 虚拟地址，表示要复制的的起始地址
 * @param buf		用户空间缓冲区
 * @param len		要复制的字节数
 * @param pte		页表项，保存虚拟地址到物理地址的映射，用于将virt_addr转换为物理地址，然后拷贝给用户空间
*/
static size_t read_pte2phy_addr(size_t virt_addr, char *buf, size_t len, pte_t *pte)
{	
	size_t ret;
	void *kaddr;
	/**
	 * 检查给定物理页是否有效，避免对无效的物理页进行访问
	 * @param pte_pfn()		获取页表项中存储的物理页框号
	 * @param pfn_valid()	检查给定的物理页框号是否在系统的有效物理内存范围内
	*/
	if (!pfn_valid(pte_pfn(*pte)))
		return 0;
	/**
	 * 将虚拟地址转换为内核线性地址
	 * 内核线性地址是内核在虚拟地址空间中的地址，用于访问内核的代码和数据。
	 * 在x86架构下，内核线性地址通常是通过将虚拟地址的最高位设置为1来表示，以与用户空间地址进行区分。
	 * @param __va()			用于将物理地址转换为内核线性地址。它受一个物理地址作为参数，并返回相应的内核线性地址。
	 * @param pte_to_phys()		用于从页表项（pte）中提取物理地址。它接受虚拟地址和页表项的值作为参数，并返回相应的物理地址。
	 * @param pte_val()			用于从页表项中提取其值，返回值只使用了其中的51位，通过按位与操作符和掩码 0x7fffffffffffffULL，可以将高于 51 位的位清零，从而确保获得的物理地址是有效的。
	*/
	kaddr = __va(pte_to_phys(virt_addr, pte_val(*pte)) & 0x7fffffffffffffULL);

	/**
	 * 将数据从内核空间拷贝到用户空间，返回未能成功复制的字节数，返回0表示全部复制完毕
	*/
	ret = copy_to_user(buf, kaddr, len);

	return len;
}
/**
 * 将用户空间缓冲区中的数据拷贝到内核空间中
 * @param virt_addr		要写入数据的虚拟地址，表示内核线性地址空间中的一个位置。
 * @param buf			用户空间缓冲区，存放要写入的数据。
 * @param len			要拷贝的数据长度
 * @param pte			是一个物理页的页表项，描述了 virt_addr 对应的物理页的映射信息。
*/
static size_t write_pte2phy_addr(size_t virt_addr, char *buf, size_t len, pte_t *pte)
{
	size_t ret;
	void *baseaddr;

	if (!pfn_valid(pte_pfn(*pte)))
		return 0;
	
	baseaddr = __va(pte_to_phys(virt_addr, pte_val(*pte)) & 0x7fffffffffffffULL);

	ret = copy_from_user(baseaddr, buf, len);

	return len;
}
