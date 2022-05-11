
// *****************************************************************************
//  Console
//
//    TTY text console functions
//
//  Copyright (c) 2022 Andreas Dietrich
// *****************************************************************************

// Console
#include "console.h"

// stb
#ifndef STB_SPRINTF_IMPLEMENTATION
#define STB_SPRINTF_IMPLEMENTATION
#endif
#ifndef STB_SPRINTF_NOSTD
#define STB_SPRINTF_NOSTD
#endif
#include "../thirdparty/stb/stb_sprintf.h"

#define CONSOLE_TAB_SIZE           8
#define CONSOLE_LINE_BUFFER_SIZE 160 // 4 lines

// *****************************************************************************
//
//  Variables
//
// *****************************************************************************

static u16 m_consoleInit = 0;
static u16 m_consoleX    = 0;
static u16 m_consoleY    = 0;

static u16 m_consoleLeft   =  0;
static u16 m_consoleTop    =  0;
static u16 m_consoleWidth  = 40;
static u16 m_consoleHeight = 28;

static u16* m_consoleFrameBuffer = NULL;
static char m_consoleLinebuffer[CONSOLE_LINE_BUFFER_SIZE];

static TransferMethod     m_consoleTransferMethod = DMA;
static ConsoleSystemReset m_consoleSystemReset    = CSR_ON_WRITE;

// *****************************************************************************
//
//  Functions
//
// *****************************************************************************

// -----------------------------------------------------------------------------
//  Public printf functions
// -----------------------------------------------------------------------------

int CON_sprintf(char* buf, const char *fmt, ...)
{
    int len;

    va_list args;
    va_start(args, fmt);
    len = stbsp_vsprintf(buf, fmt, args);
    va_end(args);

    return len;
}

// -----------------------------------------------------------------------------

int CON_snprintf(char* buf, int count, const char *fmt, ...)
{
    int len;

    va_list args;
    va_start(args, fmt);
    len = stbsp_vsnprintf(buf, count, fmt, args);
    va_end(args);

    return len;
}

// -----------------------------------------------------------------------------
//  Private console functions
// -----------------------------------------------------------------------------

static u16 consoleGetBasetile()
{
    return TILE_ATTR_FULL(
        VDP_getTextPalette(),
        VDP_getTextPriority(),
        FALSE,
        FALSE,
        TILE_FONTINDEX
    );
}

// -----------------------------------------------------------------------------

static u16* consoleGetFrameBuffer()
{
    if (m_consoleSystemReset == CSR_ON_WRITE)
    {
        if (!m_consoleInit)
        {
            SYS_disableInts();
            YM2612_reset();
            PSG_init();
            Z80_init();
            VDP_init();

            m_consoleInit = 1;
        }
    }

    if (m_consoleFrameBuffer == NULL)
    {
        const u16 tiles    = m_consoleWidth * m_consoleHeight;
        const u16 basetile = consoleGetBasetile();

        m_consoleFrameBuffer = (u16*)MEM_alloc(tiles * 2);
        if (m_consoleFrameBuffer)
            memsetU16(m_consoleFrameBuffer, basetile, tiles);

    }

    return m_consoleFrameBuffer;
}

// -----------------------------------------------------------------------------

static void consoleScroll()
{
    u16* const dst = m_consoleFrameBuffer;
    u16* const src = dst + m_consoleWidth;
    
    const u16 tiles    = m_consoleWidth * (m_consoleHeight-1);
    const u16 basetile = consoleGetBasetile();

    memcpyU16(dst, src, tiles);
    memsetU16(dst+tiles, basetile, m_consoleWidth);
}

// -----------------------------------------------------------------------------

static void consoleBackspace()
{
    if (m_consoleX > 0)
        m_consoleX--;
}

// -----------------------------------------------------------------------------

static void consoleCarriageReturn()
{
    m_consoleX = 0;
}   

// -----------------------------------------------------------------------------

static void consoleLineFeed()
{
    if (++m_consoleY >= m_consoleHeight)
    {
        m_consoleY = m_consoleHeight - 1;
        consoleScroll();
    }
}

// -----------------------------------------------------------------------------

static void consoleNewLine()
{
    consoleCarriageReturn();
    consoleLineFeed();
}

// -----------------------------------------------------------------------------

static void consoleTab()
{
    if ((m_consoleX = (m_consoleX/CONSOLE_TAB_SIZE + 1) * CONSOLE_TAB_SIZE) >= m_consoleWidth)
        consoleNewLine();
}

// -----------------------------------------------------------------------------

static void consoleAdvance()
{
    if (++m_consoleX >= m_consoleWidth)
        consoleNewLine();
}

// -----------------------------------------------------------------------------

static void consolePrint(const char *str)
{
    u16* const buffer = consoleGetFrameBuffer();
    if (!buffer)
        return;

    const u16 basetile = consoleGetBasetile() - 32;

    char c;
    while ((c = *str++))
    {
        switch (c)
        {
            case '\b': consoleBackspace();      break;
            case '\r': consoleCarriageReturn(); break;
            case '\n': consoleNewLine();        break;
            case '\t': consoleTab();            break;
            case '\v': consoleLineFeed();       break;
        
            default:
            {
                buffer[m_consoleY * m_consoleWidth + m_consoleX] = basetile + c;
                consoleAdvance();
            }
        }
    }

    VDP_setTileMapDataRect(
        VDP_getTextPlane(),
		buffer,
		m_consoleLeft,
		m_consoleTop,
		m_consoleWidth,
		m_consoleHeight,
        m_consoleWidth,
		m_consoleTransferMethod
	);
}

// -----------------------------------------------------------------------------
//  Public console functions
// -----------------------------------------------------------------------------

void CON_reset()
{
    // Reset with default values
    CON_resetEx(
        0,           /* left colum */
        0,           /* top row */
        40,          /* width */
        28,          /* height */
        DMA,         /* transfer method */
        CSR_ON_WRITE /* system reset */
    );
}

// -----------------------------------------------------------------------------

void CON_resetEx(u16 left, u16 top, u16 width, u16 height, TransferMethod tm, ConsoleSystemReset systemReset)
{
    if (width && height)
    {
        m_consoleLeft   = left;
        m_consoleTop    = top;
        m_consoleWidth  = width;
        m_consoleHeight = height;
    }
    else
    {
        m_consoleLeft   = 0;
        m_consoleTop    = 0;
        m_consoleWidth  = VDP_getScreenWidth() / 8;
        m_consoleHeight = VDP_getScreenHeight() / 8;
    }

    m_consoleInit = 0;
    m_consoleX    = 0;
    m_consoleY    = 0;

    m_consoleTransferMethod = tm;
    m_consoleSystemReset    = systemReset;

    MEM_free(m_consoleFrameBuffer);
    m_consoleFrameBuffer = NULL;
}

// -----------------------------------------------------------------------------

int CON_write(const char *fmt, ...)
{
    int len;
    char *buffer = m_consoleLinebuffer;

    va_list args;
    va_start(args, fmt);
    len = stbsp_vsnprintf(buffer, CONSOLE_LINE_BUFFER_SIZE, fmt, args);
    va_end(args);

    consolePrint(buffer);

    return len;
}
