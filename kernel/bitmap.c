#include "bitmap.h"
#include "stdint.h"
#include "string.h"
#include "interrupt.h"
#include "debug.h"
#include "print.h"

void bitmap_init(struct bitmap* bm) 
{
	memset(bm->bm_addr, 0, bm->bm_len);
}

uint8_t bitmap_scan_test(struct bitmap* bm, uint32_t index)
{
	uint32_t bm_index = index / 8;
	uint8_t offset   = index % 8;

	uint8_t result = bm->bm_addr[bm_index] & (1 << offset);
	result = result >> offset;
	
	return result;
}

int32_t bitmap_scan(struct bitmap* bm, uint32_t cnt)
{
	uint32_t index = 0;
	
	while((0xff == bm->bm_addr[index]) && index < bm->bm_len) {
		index++;
	}

	ASSERT(index < bm->bm_len);
	
	uint32_t start = index * 8;
	while(bitmap_scan_test(bm, start)) {
		start++;
	}
	uint32_t bit_start = start;
	if(cnt == 1) {
		return bit_start;
	}

	uint32_t bit_left = bm->bm_len * 8 - bit_start;
	uint32_t count = 1;
	uint32_t next = bit_start + 1;
	bit_start = -1;
	while(bit_left--) {
		if( !bitmap_scan_test(bm, next) ) {
			count++;
		}
		else {
			count = 0;
		}

		if( count == cnt ) {
			bit_start = next - cnt + 1;
			break;
		}
		next++;
	} 
	
	return bit_start;
}

void bitmap_set(struct bitmap* bm, uint32_t index, uint8_t val) 
{
	ASSERT(val == 0 || val == 1);
	uint32_t bm_index = index / 8;
	uint8_t  offset   = index % 8;
	if ( val ) {
		bm->bm_addr[bm_index] |= (1 << offset);
	}
	else {
		bm->bm_addr[bm_index] &= ~(1 << offset);
	}
}
