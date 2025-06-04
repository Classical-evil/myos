#ifndef _LIB_KERNEL_STRING_H
#define _LIB_KERNEL_STRING_H

#ifndef NULL
#define NULL 0
#endif
void memset(void* _dst_, uint8_t value, uint32_t size);
void memcpy(void* _dst_, void* _src_, uint32_t size);
int8_t memcmp(const void* _a_, const void* _b_, uint32_t size);
char* strcpy(char* _dst_, const char* _src_);
uint32_t strlen(const char* _src_);
int8_t strcmp(const char* _a_, const char* _b_);
char* strchr(const char* _str_, const uint8_t ch);
char* strrchr(const char* _str_, const uint8_t ch);
char* strcat(char* _dst_, const char* _src_);
uint32_t strchrs(const char* _str_, uint8_t ch); 

#endif
