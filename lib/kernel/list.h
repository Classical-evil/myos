#ifndef _LIB_KERNEL_H
#define _LIB_KERNEL_H

#include "stdint.h"
#include "global.h"

#define offset(type, member) (int)(&((type*)0)->member)
#define elem2entry(type, member, elem_ptr) \
		(type*)((int)elem_ptr - offset(type, member))


struct list_elem {
	struct list_elem* prev;
	struct list_elem* next;
};

struct list {
	struct list_elem head;
	struct list_elem tail;
};

typedef int8_t (*function)(struct list_elem* j, int arg);
//typedef void (*thread_func)(void*);


void list_init(struct list*);
void list_insert_before(struct list_elem* before, struct list_elem* elem);
void list_push(struct list* plist, struct list_elem* elem);
void list_iterate(struct list* plist);
void list_append(struct list* l, struct list_elem* elem);
void list_remove(struct list_elem* elem);
struct list_elem* list_pop(struct list* l);
int8_t list_empty(struct list* l);
struct list_elem* list_traversal(struct list* l, function func, int arg);
int8_t elem_find(struct list* l, struct list_elem* e);
uint32_t list_len(struct list* l);

#endif
