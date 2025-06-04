#ifndef _KERNEL_INTERRUPT_H
#define _KERNEL_INTERRUPT_H

#include "stdint.h"

enum intr_status {
	INTR_OFF,
	INTR_ON,
};
enum intr_status intr_enable();
enum intr_status intr_disable();
enum intr_status intr_set_status(enum intr_status);
enum intr_status intr_get_status();
typedef void* intr_handler;

void register_handler(uint8_t vector_no, intr_handler function);
static void general_intr_handler(uint8_t vec_nr); 
void idt_init();

#endif
