#include "memory.h"
#include "stdint.h"
#include "print.h"
#include "global.h"
#include "debug.h"
#include "string.h"

#define PG_SIZE 4096
#define MEM_BITMAP 0xc009a000
#define KERNEL_VIR 

#define PF_KERNEL 1
#define PF_USER   2

#define PDE_IDX(addr) ((addr & 0xffc00000) >> 22)
#define PTE_IDX(addr) ((addr & 0x003ff000) >> 12)

struct pool {
	struct   bitmap bm;
	uint32_t size;
	uint32_t start;
};

struct pool kernel_pool, user_pool;
struct virtual_addr kernel_vir;

void mem_pool_init(uint32_t all_memory)
{
	put_str("	mem_pool_init start\n");

	uint32_t used_memory = PG_SIZE * 256 + 0x100000;
	uint32_t free_memory = (all_memory - used_memory); 
	uint32_t kernel_memory = free_memory / 2;
	uint32_t user_memory = all_memory - kernel_memory;

	uint32_t kernel_pages = kernel_memory / PG_SIZE;
	uint32_t user_pages = user_memory / PG_SIZE;
	
	uint32_t kernel_bm_len = kernel_pages / 8;
	uint32_t user_bm_len = user_pages / 8;
	
	kernel_pool.size = kernel_memory;
	user_pool.size = user_memory;
	
	kernel_pool.start = used_memory;
	user_pool.start = used_memory + kernel_pool.size;
	
	kernel_pool.bm.bm_addr = (void*)MEM_BITMAP;
	kernel_pool.bm.bm_len = kernel_bm_len;
	
	user_pool.bm.bm_addr = (void*)(MEM_BITMAP + kernel_bm_len);
	user_pool.bm.bm_len = user_bm_len;
	
	put_str("kernel_pool_bitmap start:");
	put_int((int)kernel_pool.bm.bm_addr);
	put_str("\n");
	put_str("kernel_pool_phy_addr-start:");
	put_int(kernel_pool.start);
	put_str("\n");

	put_str("	user_pool_bitmap start:");
	put_int((int)user_pool.bm.bm_addr);
	put_str("\n");
	put_str("	user_pool_phy_addr-start:");
	put_int(user_pool.start);
	put_str("\n");

	bitmap_init(&kernel_pool.bm);
	bitmap_init(&user_pool.bm);

	kernel_vir.v_bitmap.bm_addr = (void*)(MEM_BITMAP + kernel_bm_len + user_bm_len);
	kernel_vir.v_bitmap.bm_len = kernel_bm_len;
	kernel_vir.v_start = 0xc0100000;
	bitmap_init(&kernel_vir.v_bitmap);

	put_str("	mem_pool init done\n");
}

void mem_init()
{
	put_str("mem_init start\n");
	uint32_t mem_bytes_total = (*(uint32_t*)(0xb00));
	mem_pool_init(mem_bytes_total);
	put_str("mem_init done\n");
}


static void* vaddr_get(enum pool_flags pf, uint32_t pg_cnt)
{
	int32_t start = 0;
	if ( pf == PF_KERNEL ) {
		start = bitmap_scan(&kernel_vir.v_bitmap, pg_cnt);
		if ( start == -1 ) {
			return NULL;
		}
		uint32_t cnt = 0;
		while( cnt < pg_cnt ) {
			bitmap_set(&kernel_vir.v_bitmap, start + cnt, 1);
			cnt++;
		}

		start = kernel_vir.v_start + start * PG_SIZE;
	}
	else {

	}

	return (void*)start;
}

uint32_t* pte_ptr(uint32_t vaddr)
{
	uint32_t* ptr = (uint32_t*)(0xffc00000 + \
							   ((vaddr & 0xffc00000) >> 10) + \
							   PTE_IDX(vaddr) * 4);
	
	return ptr;
}

uint32_t* pde_ptr(uint32_t vaddr) 
{
	uint32_t* ptr = (uint32_t)(0xfffff000 + \
							  PDE_IDX(vaddr) * 4);

	return ptr;
}

static void* palloc(struct pool* p)
{
	int32_t start = bitmap_scan(&p->bm, 1);
	if ( start == -1 ) {
		return NULL;
	}
	
	bitmap_set(&p->bm, start, 1);

	start = p->start + start * PG_SIZE;

	return (void*)start;
}

static void page_table_add(void* _vaddr, void* _paddr)
{
	uint32_t vaddr = (uint32_t)_vaddr;
	uint32_t paddr = (uint32_t)_paddr;
	uint32_t* pde = pde_ptr(vaddr);
	uint32_t* pte = pte_ptr(vaddr);

	if ( *pde & 0x00000001 ) {
		
		ASSERT( !(*pte & 0x00000001) );

		if ( !(*pte & 0x00000001 ) ) {
			*pte = paddr | PG_US_U | PG_RW_W | PG_P_1;
		}

	}
	else {
		uint32_t pde_addr = (uint32_t)palloc(&kernel_pool);
		*pde = pde_addr | PG_US_U | PG_RW_W | PG_P_1;
		memset((void*)((int)pte & 0xfffff000), 0, PG_SIZE);

		*pte = paddr | PG_US_S | PG_RW_W | PG_P_1;
		ASSERT( !(*pte & 0x00000001) );
		*pte = paddr | PG_US_S | PG_RW_W | PG_P_1;

	}

}

void* malloc_page(enum pool_flags pf, uint32_t pg_cnt)
{
	ASSERT( pg_cnt > 0 && pg_cnt < 3840 );

	void* vaddr_start = vaddr_get(pf, pg_cnt);
	if ( vaddr_start == NULL ) {
		return NULL;
	}

	uint32_t vaddr = (uint32_t)vaddr_start;
	uint32_t cnt = pg_cnt;
	struct pool* p = pf & PF_KERNEL ? &kernel_pool : &user_pool;
	while ( cnt-- ) {
		void* page_phyaddr = palloc(p);
		if ( page_phyaddr == NULL ) {
			return NULL;
		}
		page_table_add((void*)vaddr, page_phyaddr);
		vaddr += PG_SIZE;
	}

	return vaddr_start;
}

void* get_kernel_pages(uint32_t pg_cnt) {
	void* vaddr = malloc_page(PF_KERNEL, pg_cnt);
	if ( vaddr != NULL ) {
		memset(vaddr, 0, pg_cnt * PG_SIZE);
	}

	return vaddr;
}
