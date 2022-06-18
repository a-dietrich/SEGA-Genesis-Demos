
// *****************************************************************************
//  Console
//
//    TTY text console functions
//
//  Copyright (c) 2022 Andreas Dietrich
// *****************************************************************************

/**
 *  \file   console.h
 *  \brief  Console -- Provides a simple TTY style console
 *  \author Andreas Dietrich
 *  \date   06/2022
 *
 *  This unit provides a simple TTY text console. Characters are written as a
 *  stream, where lines are automatically wrapped if the horizontal border of
 *  the screen is reached. When at the bottom of the screen window, the console
 *  content is moved up by one text line and a blank row is inserted.
 *
 *  Per default, the console occupies a standad screen of 40x28 tiles. All text
 *  attributes, such as font, palette, plane etc., are taken from SGDK text
 *  settings. Screen updates are done using DMA transfer mode (which can be
 *  changed with CON_setTransferMethod()).
 *
 *  One of the use cases are assert messages. To this end, the Genesis state can
 *  be automatically reset before text is displayed (see assert macro below).
 *
 *  For string formatting this makes use of stb_sprintf. <br>
 *  http://github.com/nothings/stb
 *
 */

#pragma once

// *****************************************************************************
//
//  Includes
//
// *****************************************************************************

// SGDK
#include <genesis.h>

// *****************************************************************************
//
//  Defines
//
// *****************************************************************************

/// Comment out to disable asserts
#define ENABLE_ASSERT_CHECKS

// *****************************************************************************
//
//  Types
//
// *****************************************************************************

typedef u32 size_t;
typedef u32 ptrdiff_t;

// *****************************************************************************
//
//  Macros
//
// *****************************************************************************

/// Helper macro to stringify numbers
#define str(s) xstr(s)
/// Helper macro
#define xstr(s) #s

/**
 *  \brief
 *      Assert a condition.
 *
 *  \param condition
 *      Expression that must evaluate to TRUE, otherwise an error error message
 *      in printed to the console. This will also reset the system to the SGDK
 *      default state.
 *
 *  Can be disabled by undefining ENABLE_ASSERT_CHECKS or by defining NDEBUG.
 */

#if defined(ENABLE_ASSERT_CHECKS) && !defined(NDEBUG)
#define assert(condition)                                                              \
    if (! (bool)(u32)(condition) )                                                     \
    {                                                                                  \
        CON_reset();                                                                   \
        CON_systemResetOnNextWrite();                                                  \
        CON_write(__FILE__":"str(__LINE__)": Assertion \'"str(condition)"\' failed."); \
        while (TRUE);                                                                  \
    }
#else
#define assert(condition)
#endif

/**
 *  \brief
 *      Assert a condition.
 *
 * Uppercase version. Same as assert().
 */

#define ASSERT(condition) assert(condition)

// *****************************************************************************
//
//  Function Declarations
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// printf functions
// -----------------------------------------------------------------------------

/**
 *  \brief
 *      Standard C library sprintf function.
 *
 *  \param buf
 *      Pointer to the buffer where the resulting C-string is stored.
 *  \param fmt
 *      C-string that contains the text to be written. It can optionally
 *      contain embedded format specifiers that are replaced by the values
 *      specified in subsequent additional arguments and formatted as requested.
 *      (See https://en.wikipedia.org/wiki/Printf_format_string)
 *  \param ... (additional arguments)
 *      The function may expect a sequence of additional arguments, each
 *      containing a value to be used to replace a format specifier in the
 *      format string.
 *  \return
 *      The total number of characters written is returned. This count does not
 *      include the additional null-character automatically appended at the end
 *      of the string.
 *
 *  Composes a string that would result from using a standard C library printf
 *  function. In case fmt includes format specifiers (beginning with %), the
 *  additional arguments are formatted and inserted in the resulting string
 *  replacing their respective specifiers.
 *
 *  This function is a wrapper for stbsp_vsprintf(). <br>
 *  See http://github.com/nothings/stb for more details.
 *
 *  allowed types:  sc uidBboXx p AaGgEef n <br>
 *  lengths      :  h ll j z t I64 I32 I
 */

int CON_sprintf(char* buf, const char *fmt, ...)  __attribute__ ((format (printf, 2, 3)));

/**
 *  \brief
 *      Standard C library snprintf function.
 *
 *  \param buf
 *      Pointer to the buffer where the resulting C-string is stored.
 *  \param count
 *      Maximum number of bytes to be used in the buffer. The generated string
 *      has a length of at most count-1, leaving space for the additional
 *      terminating null-character.
 *  \param fmt
 *      C-string that contains the text to be written. It can optionally
 *      contain embedded format specifiers that are replaced by the values
 *      specified in subsequent additional arguments and formatted as requested.
 *      (See https://en.wikipedia.org/wiki/Printf_format_string)
 *  \param ... (additional arguments)
 *      The function may expect a sequence of additional arguments, each
 *      containing a value to be used to replace a format specifier in the
 *      format string.
 *  \return
 *      The total number of characters written is returned. This count does not
 *      include the additional null-character automatically appended at the end
 *      of the string.
 *
 *  Composes a string that would result from using a standard C library printf
 *  function. In case fmt includes format specifiers (beginning with %), the
 *  additional arguments are formatted and inserted in the resulting string
 *  replacing their respective specifiers.
 *
 *  This function is a wrapper for stbsp_vsnprintf(). <br>
 *  See http://github.com/nothings/stb for more details.
 *
 *  allowed types:  sc uidBboXx p AaGgEef n <br>
 *  lengths      :  h ll j z t I64 I32 I
 */

int CON_snprintf(char* buf, int count, const char *fmt, ...)  __attribute__ ((format (printf, 3, 4)));

// -----------------------------------------------------------------------------
// Console setup
// -----------------------------------------------------------------------------

/**
 *  \brief
 *      Set the size of the console window.
 *
 *  \param left
 *      Position of the leftmost window column in tiles. Default is 0.
 *  \param top
 *      Position of the topmost window row in tiles. Default is 0.
 *  \param width
 *      Width of the console window in tiles. Default is 40.
 *  \param height
 *      Height of the console window in tiles. Default is 28.
 *
 *  If either width or height are 0 then VDP_getScreenWidth() and
 *  VDP_getScreenHeight() will be used to determine suitable default values.
 */

void CON_setSize(u16 left, u16 top, u16 width, u16 height);

/**
 *  \brief
 *      Set the transfer method used to upload the console tile buffer to VDP
 *      RAM.
 *
 *  \param tm
 *      Transfer method. <br>
 *      Accepted values are: <br>
 *      - CPU <br>
 *      - DMA <br>
 *      - DMA_QUEUE <br>
 *      - DMA_QUEUE_COPY
 *
 *  This sets the transfer method used with VDP_setTileMapDataRect(). The
 *  default value is DMA, which will cause an immediate upload. Note that when
 *  DMA_QUEUE or DMA_QUEUE_COPY is used, the user is responsible for triggering
 *  DMA upload, e.g., by calling SYS_doVBlankProcess().
 */

void CON_setTransferMethod(TransferMethod tm);

/**
 *  \brief
 *      Reset the console.
 *
 *  This function is a shortcut for:
 *
 *  CON_setSize(0, 0, 40, 28); <br>
 *  CON_setTransferMethod(DMA);
 */

void CON_reset();

/**
 *  \brief
 *      Reset the system to the SGDK default state when CON_write() is called
 *      next time.
 *
 *  To reset the system the following SGDK functions will be called:
 *
 *  - SYS_disableInts() <br>
 *  - YM2612_reset() <br>
 *  - PSG_init() <br>
 *  - Z80_init() <br>
 *  - VDP_init()
 */

void CON_systemResetOnNextWrite();

// -----------------------------------------------------------------------------
// Console write functions
// -----------------------------------------------------------------------------

/**
 *  \brief
 *      Clear the console window.
 *
 *  This clears the console window by filling it with space characters. It is
 *  assumed that the first tile of the font is a space character. DMA settings
 *  apply here as well.
 */

void CON_clear();

/**
 *  \brief
 *      Set a new cursor position.
 *
 *  \param x
 *      New cursor x position.
 *  \param y
 *      New cursor y position.
 *
 *  This function specifies a new column and row for the cursor. This is the
 *  position where the next character will appear when CON_write() is processed.
 *  A position of (0,0) relates to the tile in the top/left corner.
 */

void CON_setCursorPosition(u16 x, u16 y);

/**
 *  \brief
 *      Return the current cursor position.
 *
 *  \return
 *      A 2D vector containing the current (x,y) position of the cursor, where
 *      (0,0) means the top/left corner of the console window.
 */

V2u16 CON_getCursorPosition();

/**
 *  \brief
 *      Write a C-string to the console window.
 *
 *  \param fmt
 *      C-string that contains the text to be written. It can optionally
 *      contain embedded format specifiers that are replaced by the values
 *      specified in subsequent additional arguments and formatted as requested.
 *      (See https://en.wikipedia.org/wiki/Printf_format_string)
 *  \param ... (additional arguments)
 *      The function may expect a sequence of additional arguments, each
 *      containing a value to be used to replace a format specifier in the
 *      format string.
 *  \return
 *      The total number of characters written is returned. This count does not
 *      include the additional null-character automatically appended at the end
 *      of the string.
 *
 *  This function writes a C-string into the console window where lines are
 *  automatically wrapped if the horizontal border of the screen is reached.
 *  When at the bottom of the screen window, the console content is moved up by
 *  one text line and a blank row is inserted.
 *
 *  Internally it uses stbsp_vsnprintf() for formatting. See CON_snprintf() for
 *  more details of supported format specifiers.
 *
 *  While processing the string, this function evaluates and executes escape
 *  control sequences. The following control characters are supported:
 *
 *  - \\b : backspace <br>
 *  - \\n : new line (line feed) <br>
 *  - \\r : carriage return <br>
 *  - \\t : horizontal tab <br>
 *  - \\v : vertical tab
 */

int CON_write(const char *fmt, ...)  __attribute__ ((format (printf, 1, 2)));
