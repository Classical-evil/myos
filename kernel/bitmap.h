#ifndef _KERNEL_BITMAP_H
#define _KERNEL_BITMAP_H
#include "stdint.h"
#include "global.h"

struct bitmap {
	uint8_t* bm_addr;
	uint32_t bm_len;
};

void bitmap_init(struct bitmap* bm);
uint8_t bitmap_scan_test(struct bitmap* bm, uint32_t index);
int32_t bitmap_scan(struct bitmap* bm, uint32_t cnt);
void bitmap_set(struct bitmap* bm, uint32_t index, uint8_t val);

#endif
