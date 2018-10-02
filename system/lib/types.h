#ifndef TYPES_H
#define TYPES_H

#include "bool.h"

typedef unsigned int u32;
typedef int s32;
typedef unsigned short u16;
typedef short s16;
typedef unsigned char u8;
typedef char s8;

typedef unsigned int uint32_t;
typedef int int32_t;
typedef unsigned short uint16_t;
typedef short int16_t;
typedef unsigned char uint8_t;
typedef char int8_t;

typedef int8_t bool;

#define NULL 0


#define low_16(address) (u16)((address) & 0xFFFF)
#define high_16(address) (u16)(((address) >> 16) & 0xFFFF)
#define low_8(address) (u8)(divisor & 0xFF)
#define high_8(address) (u8)( (divisor>>8) & 0xFF)

#endif