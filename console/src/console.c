
// *****************************************************************************
//  Console
//
//    TTY text console functions
//
//  Copyright (c) 2022 Andreas Dietrich
// *****************************************************************************

// *****************************************************************************
//
//  Includes
//
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
#ifndef STB_SPRINTF_NOUNALIGNED
#define STB_SPRINTF_NOUNALIGNED
#endif

#include "../thirdparty/stb/stb_sprintf.h"

// *****************************************************************************
//
//  Defines
//
// *****************************************************************************

#define CONSOLE_USE_STB              // disbale use of SGDK sprintf
#define CONSOLE_TAB_SIZE           8 // horizontal tab width
#define CONSOLE_LINE_BUFFER_SIZE 160 // buffer keeps 4 lines

// *****************************************************************************
//
//  Variables
//
// *****************************************************************************

static bool m_consoleDoBufferReset = TRUE;
static bool m_consoleDoSystemReset = FALSE;

static u16 m_consoleX      =  0;
static u16 m_consoleY      =  0;
static u16 m_consoleLeft   =  0;
static u16 m_consoleTop    =  0;
static u16 m_consoleWidth  = 40;
static u16 m_consoleHeight = 28;

static TransferMethod m_consoleTransferMethod = DMA;

static u16* m_consoleFrameBuffer = NULL;
static char m_consoleLinebuffer[CONSOLE_LINE_BUFFER_SIZE] = {};

// *****************************************************************************
//
//  Functions
//
// *****************************************************************************

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
    if (m_consoleDoSystemReset)
    {
        SYS_disableInts();
        YM2612_reset();
        PSG_init();
        Z80_init();
        VDP_init();

        m_consoleDoSystemReset = FALSE;
    }

    if (m_consoleDoBufferReset)
    {
        const u16 basetile = consoleGetBasetile();
        const u16 tiles    = m_consoleWidth * m_consoleHeight;
        const u16 bytes    = tiles * 2;

        MEM_free(m_consoleFrameBuffer);
        m_consoleFrameBuffer = (u16*)MEM_alloc(bytes);

        if (m_consoleFrameBuffer)
        {
            memsetU16(m_consoleFrameBuffer, basetile, tiles);
            m_consoleDoBufferReset = FALSE;
        }
    }

    return m_consoleFrameBuffer;
}

// -----------------------------------------------------------------------------

static void consoleClearFrameBuffer()
{
    if (m_consoleFrameBuffer)
    {
        const u16 basetile = consoleGetBasetile();
        const u16 tiles    = m_consoleWidth * m_consoleHeight;

        memsetU16(m_consoleFrameBuffer, basetile, tiles);
    }
}

// -----------------------------------------------------------------------------

static void consoleScroll()
{
    if (m_consoleFrameBuffer)
    {
        u16* const dst = m_consoleFrameBuffer;
        u16* const src = dst + m_consoleWidth;
        
        const u16 tiles    = m_consoleWidth * (m_consoleHeight-1);
        const u16 basetile = consoleGetBasetile();

        memcpyU16(dst, src, tiles);
        memsetU16(dst+tiles, basetile, m_consoleWidth);
    }
}

// -----------------------------------------------------------------------------

static void consoleUploadFrameBuffer()
{
    if (m_consoleFrameBuffer)
    {
        VDP_setTileMapDataRect(
            VDP_getTextPlane(),
            m_consoleFrameBuffer,
            m_consoleLeft,
            m_consoleTop,
            m_consoleWidth,
            m_consoleHeight,
            m_consoleWidth,
            m_consoleTransferMethod
        );
    }
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

static void consoleHorizontalTab()
{
    m_consoleX = min((m_consoleX/CONSOLE_TAB_SIZE + 1) * CONSOLE_TAB_SIZE,
                      m_consoleWidth-1);
}

// -----------------------------------------------------------------------------

static void consoleVerticalTab()
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
    consoleVerticalTab();
}

// -----------------------------------------------------------------------------

static void consoleCharacter(char c, u16* buffer, u16 basetile)
{
    if (m_consoleX >= m_consoleWidth)
        consoleNewLine();

    m_consoleX = min(m_consoleX, m_consoleWidth-1);
    m_consoleY = min(m_consoleY, m_consoleHeight-1);

    buffer[m_consoleY * m_consoleWidth + m_consoleX] = basetile + c;
    m_consoleX++;
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
            case '\n': consoleNewLine();        break;
            case '\r': consoleCarriageReturn(); break;
            case '\t': consoleHorizontalTab();  break;
            case '\v': consoleVerticalTab();    break;
        
            default:
                consoleCharacter(c, buffer, basetile);
        }
    }
}

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
//  Public console functions
// -----------------------------------------------------------------------------

void CON_setSize(u16 left, u16 top, u16 width, u16 height)
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

    m_consoleX = 0;
    m_consoleY = 0;

    m_consoleDoBufferReset = TRUE;
}

// -----------------------------------------------------------------------------

void CON_setTransferMethod(TransferMethod tm)
{
    m_consoleTransferMethod = tm;
}

// -----------------------------------------------------------------------------

void CON_reset()
{
    CON_setSize(0, 0, 40, 28);
    CON_setTransferMethod(DMA);
}

// -----------------------------------------------------------------------------

void CON_systemResetOnNextWrite()
{
    m_consoleDoSystemReset = TRUE;
}

// -----------------------------------------------------------------------------

void CON_clear()
{
    m_consoleX = m_consoleY = 0;

    consoleClearFrameBuffer();
    consoleUploadFrameBuffer();
}

// -----------------------------------------------------------------------------

void CON_setCursorPosition(u16 x, u16 y)
{
    m_consoleX = min(x, m_consoleWidth-1);
    m_consoleY = min(y, m_consoleHeight-1);
}

// -----------------------------------------------------------------------------

V2u16 CON_getCursorPosition()
{
    V2u16 result = { m_consoleX, m_consoleY };

    return result;
}

// -----------------------------------------------------------------------------

int CON_write(const char *fmt, ...)
{
    int len;
    char *buffer = m_consoleLinebuffer;

    va_list args;
    va_start(args, fmt);

#ifdef CONSOLE_USE_STB
    len = stbsp_vsnprintf(buffer, CONSOLE_LINE_BUFFER_SIZE, fmt, args);
#else
    extern u16 vsprintf(char *buf, const char *fmt, va_list args);
    len = vsprintf(buffer, fmt, args);
#endif

    va_end(args);

    consolePrint(buffer);
    consoleUploadFrameBuffer();

    return len;
}
