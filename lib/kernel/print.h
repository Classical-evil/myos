#ifndef _LIB_KERNEL_PRINT_H
#define _LIB_KERNEL_PRINT_H

#include "stdint.h"
void put_char(uint8_t char_asci);
void put_str(char* ptr);
void put_int(int);
void set_cursor(uint32_t cursor_pos);
#endif
		
