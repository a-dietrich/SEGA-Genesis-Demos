
// *****************************************************************************
//  Console
//
//    TTY text console functions
//
//  Copyright (c) 2022 Andreas Dietrich
// *****************************************************************************

#pragma once

// SGDK
#include <genesis.h>

// *****************************************************************************
//
//  Defines
//
// *****************************************************************************

#define ENABLE_ASSERT_CHECKS

// *****************************************************************************
//
//  Types
//
// *****************************************************************************

typedef u32                size_t;
typedef u32                ptrdiff_t;
typedef u16                bool;
typedef unsigned long long u64;
typedef signed   long long s64;

typedef enum ConsoleSystemReset
{
    CSR_NOW,
    CSR_ON_WRITE,
    CSR_NEVER
}
ConsoleSystemReset;

// *****************************************************************************
//
//  Macros
//
// *****************************************************************************

#define str(s) xstr(s)
#define xstr(s) #s

#if defined(ENABLE_ASSERT_CHECKS) && !defined(NDEBUG)
#define assert(condition)                                                              \
    if (! (bool)(u32)(condition) )                                                     \
    {                                                                                  \
        CON_reset();                                                                   \
        CON_write(__FILE__":"str(__LINE__)": Assertion \'"str(condition)"\' failed."); \
        while (TRUE);                                                                  \
    }
#else
#define assert(condition)
#endif


#define ASSERT(condition) assert(condition)

// *****************************************************************************
//
//  Function Declarations
//
// *****************************************************************************

// Printf functions
int CON_sprintf(char* buf, const char *fmt, ...)  __attribute__ ((format (printf, 2, 3)));
int CON_snprintf(char* buf, int count, const char *fmt, ...)  __attribute__ ((format (printf, 3, 4)));

// Console functions
void CON_reset();
void CON_resetEx(u16 left, u16 top, u16 width, u16 height, TransferMethod tm, ConsoleSystemReset systemReset);
int  CON_write(const char *fmt, ...)  __attribute__ ((format (printf, 1, 2)));
