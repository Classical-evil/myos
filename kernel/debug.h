#ifndef _KERNEL_DEBUG_H
#define _KERNEL_DEBUG_H
void panic_spin(char* filename, int line, const char* function, const char* condition);

#define PAINC(...) panic_spin(__FILE__, __LINE__, __func__, __VA_ARGS__)

#ifdef NDEBUG
	#define ASSERT(condition) ((void)0)
#else 
	#define ASSERT(condition) if (condition) {} \
	else {PAINC(#condition);}

#endif
#endif
