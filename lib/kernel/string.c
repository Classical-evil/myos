#include "global.h"
#include "string.h"
#include "debug.h"

void memset(void* _dst_, uint8_t value, uint32_t size)
{
	ASSERT(_dst_ != NULL);

	uint8_t* dst = (uint8_t*)_dst_;
	while(size--) {
		*dst++ = value;
	}
}

void memcpy(void* _dst_, void* _src_, uint32_t size)
{
	ASSERT(_dst_ != NULL && _src_ != NULL);
	uint8_t* dst = _dst_;
	const uint8_t* src = _src_;
	while(size--) {
		*dst++ = *src++;
	}
}

int8_t memcmp(const void* _a_, const void* _b_, uint32_t size)
{
	ASSERT(_a_ != NULL && _b_ != NULL);
	if(size == 0) return 0;
	const uint8_t* a = _a_;
	const uint8_t* b = _b_;
	while(size--) {
		if (*a != *b) {
			return *a > *b ? 1: -1;
		}
		a++;
		b++;
		
	}
	return 0;
}

char* strcpy(char* _dst_, const char* _src_)
{
	ASSERT( _dst_ != NULL && _src_ != NULL );
	char* dst = _dst_;
	while(*dst++ = *_src_++);
	return dst;
}

uint32_t strlen(const char* _src_)
{
	ASSERT(_src_ != NULL);
	const char* s = _src_;
	while(*_src_++);
	return _src_-s-1;
}

int8_t strcmp(const char* _a_, const char* _b_)
{
	ASSERT(_a_ != NULL && _b_ != NULL);
	while (*_a_ != 0 && *_a_ == *_b_) {
		_a_++;
		_b_++;
	}
	return (*_a_ == *_b_) ? 0 : (*_a_ > *_b_) ? 1 : -1;
}

char* strchr(const char* _str_, const uint8_t ch)
{
	ASSERT(_str_ != NULL);
	do {
		if(*_str_ == ch) {
			return (char*)_str_;
		}
		_str_++;

	}while(*_str_ != 0);
	return NULL;
}

char* strrchr(const char* _str_, const uint8_t ch)
{
	ASSERT(_str_ != NULL);
	char* s = NULL;
	do {
		if(*_str_ == ch) {
			s = (char*)_str_;
		}
	}while(*_str_ != 0);
	return s;
}

char* strcat(char* _dst_, const char* _src_)
{
	ASSERT(_dst_ != NULL && _src_ != NULL);
	char* dst = _dst_;
	while(*_dst_++);
	_dst_--;
	while(*_dst_++ = *_src_++);
	return dst;

}

uint32_t strchrs(const char* _str_, uint8_t ch) 
{
	ASSERT( _str_ != NULL );
	uint32_t count = 0;
	while(*_str_ != 0) {
		if (*_str_ == ch) {
			count++;
		}
		_str_++;
	}
	return count;
}
