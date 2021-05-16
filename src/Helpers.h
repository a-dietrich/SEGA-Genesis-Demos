
// *****************************************************************************
//  Genesis Utils
//
//    Helper functions for SGDK
//
//  Copyright (c) 2018-2021 Andreas Dietrich
// *****************************************************************************

#pragma once

// SGDK
#include <genesis.h>

// *****************************************************************************
//
//  Defines
//
// *****************************************************************************

#define PAL0IDX  0
#define PAL1IDX 16
#define PAL2IDX 32
#define PAL3IDX 48

#define IO_VERSION_ADDR          0xA10001

#define IO_VERSION_MODE          0x80
#define IO_VERSION_VMOD          0x40
#define IO_VERSION_DISK          0x20
#define IO_VERSION_VER           0x0F

#define IO_VERSION_MODE_DOMESTIC 0x00
#define IO_VERSION_MODE_OVERSEAS 0x80

#define IO_VERSION               (*((u8*)IO_VERSION_ADDR))

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

// *****************************************************************************
//
//  Structures
//
// *****************************************************************************

typedef struct MetaSprite
{
    s16 x;
    s16 y;

    u16 rows;
    u16 columns;

    u16 vdpSpriteIndex;
    u16 vdpTileIndexStart;
    u16 vdpTileIndexEnd;
}
MetaSprite;

// -----------------------------------------------------------------------------

typedef enum MenuItemType
{
    MIT_S16,
    MIT_BOOL
}
MenuItemType;

typedef struct MenuItemS16
{
    s16* value;
    s16  min;
    s16  max;
    s16  step;
}
MenuItemS16;

typedef struct MenuItemBool
{
    bool* value;
}
MenuItemBool;

typedef struct MenuItem
{
    u16   x;
    u16   y;

    char* description;

    union
    {
        MenuItemS16  vs16;
        MenuItemBool vbool;
    };
    MenuItemType type;
}
MenuItem;

// *****************************************************************************
//
//  Data Declarations
//
// *****************************************************************************

extern const fix32 arctab32[1024];

// *****************************************************************************
//
//  Function Declarations
//
// *****************************************************************************

// Interrupt functions
extern void GUT_setVHInts(VoidCallback *VIntCB, VoidCallback *HIntCB, u8 HIntCounter);
extern void GUT_unsetVHInts();

// Color palette functions
extern void GUT_fadeInBlue(u16 fromcol, u16 tocol, const u16 *pal, u16 numframeBlue, u16 numframeGreenRed);
extern void GUT_fadeOutBlue(u16 fromcol, u16 tocol, u16 numframeBlue, u16 numframeGreenRed);
extern bool GUT_fadeOutBlueStep(u16 fromcol, u16 tocol);
extern bool GUT_fadeOutStep(u16 fromcol, u16 tocol);
extern void GUT_copyPaletteColors(u16 dstIndex, u16 srcIndex, u16 count);
extern void GUT_fillPaletteColors(u16 index, u16 value, u16 count);
extern void GUT_cycleColorsUp(u16 *pal, u16 index, u16 count);
extern void GUT_cycleColorsDown(u16 *pal, u16 index, u16 count);
extern void GUT_changeBrightness(u16 *dstPal, u16 *srcPal, s16 val, u16 count);

// Tile functions
extern void GUT_fillVerticalScrollTile(VDPPlane plan, u16 tile, s16 value, u16 len, TransferMethod tm);
extern u16  GUT_getTileMapXY(VDPPlane plane, u16 x, u16 y);
extern u16  GUT_drawImage(VDPPlane plane, const Image *image, u16 basetile, u16 x, u16 y, u16 loadpal, bool dma);
extern u16  GUT_getTileIndex();
extern void GUT_setTileIndex(u16 index);
extern void GUT_resetTileIndex();

// Sprite functions
extern u16        GUT_getNextMetaSpritesIndex();
extern void       GUT_resetMetaSprites();
// XXX - fix names
extern MetaSprite GUT_addMetaSprite(const SpriteDefinition *spriteDef, u16 animation, u16 frame, u16 pal, u16 tileIndex);
extern MetaSprite GUT_addMetaSpriteDirect(u16 w, u16 h, u16 pal, u16 tileIndex);
extern void       GUT_setMetaSpritePosition(MetaSprite *metaSprite, s16 x, s16 y);
extern void       GUT_easeInMetaSprite(MetaSprite *metasprite, fix32 x0, fix32 x1, fix32 y0, fix32 y1, u16 speed);
extern void       GUT_updateMetaSprites(u16 queue);

// Text functions
extern void GUT_setPrint(VDPPlane plan, u16 basetile);
extern void GUT_print(u16 x, u16 y, const char *str);
extern void GUT_printCentered(u16 y, const char *str);
extern void GUT_printLeftAligned(u16 y, const char *str);
extern void GUT_printRighAligned(u16 y, const char *str);
extern void GUT_printTiles(const char *str, u16 tileIndex, u32 *tileData);
extern int  GUT_sprintf(char* buf, const char *fmt, ...)  __attribute__ ((format (printf, 2, 3)));
extern int  GUT_snprintf(char* buf, int count, const char *fmt, ...)  __attribute__ ((format (printf, 3, 4)));

// Console functions
extern int  GUT_writeConsole(const char *fmt, ...)  __attribute__ ((format (printf, 1, 2)));
extern void GUT_writeConsoleI1(const char *fmt, s16 arg0);
extern void GUT_writeConsoleI2(const char *fmt, s16 arg0, s16 arg1);
extern void GUT_error(bool condition, char *message);

// Menu functions
extern void GUT_initMenu(MenuItem* items, u16 num);
extern void GUT_initMenuEx(MenuItem* items, u16 num, char* pointer);
extern void GUT_resetMenu();
extern void GUT_updateMenuVDP();
extern void GUT_updateMenuItems();

// *****************************************************************************
//
//  Macros
//
// *****************************************************************************

#define WAIT_SCANLINE(line) { while (GET_VCOUNTER != (line)); }
#define WAIT_FOREVER { while (TRUE); }

// -----------------------------------------------------------------------------

#define SECONDS_TO_FRAMES(seconds) (IS_PALSYSTEM ? (seconds)*50 : (seconds)*60)

// -----------------------------------------------------------------------------

#define CENTERED_PIXELS(screenSize, sizeInTiles) (((screenSize) - (sizeInTiles)*8) / 2)

// -----------------------------------------------------------------------------

#define TILE_IDX(pal, index) TILE_ATTR_FULL((pal), FALSE, FALSE, FALSE, (index))

// -----------------------------------------------------------------------------

#define CLAMP(x, minimum, maximum) (min(max((x),(minimum)),(maximum)))

// -----------------------------------------------------------------------------

#define SWAP(a, b) do { typeof(a) tmp = a; a = b; b = tmp; } while (0)

// -----------------------------------------------------------------------------

#define VDP_PRINT_0(x, y, fmt)  \
{                               \
    char buffer[128];           \
    sprintf(buffer, fmt);       \
    VDP_drawText(buffer, x, y); \
}

#define VDP_PRINT_1(x, y, fmt, arg0) \
{                                    \
    char buffer[128];                \
    sprintf(buffer, fmt, arg0);      \
    VDP_drawText(buffer, x, y);      \
}

#define VDP_PRINT_2(x, y, fmt, arg0, arg1) \
{                                          \
    char buffer[128];                      \
    sprintf(buffer, fmt, arg0, arg1);      \
    VDP_drawText(buffer, x, y);            \
}

// -----------------------------------------------------------------------------

#ifdef ENABLE_ERROR_CHECKS
#   define ERROR(condition, message) GUT_error((bool)(u32)(condition), (message))
#else
#   define ERROR(condition, message)
#endif

// *****************************************************************************
//
//  Inline Functions
//
// *****************************************************************************

inline float wrapf(float x, float min, float max)
{
    const float range = max - min;

    while (x < min)
        x += range;

    while (x >= max)
        x -= range;

    return x;
}

// -----------------------------------------------------------------------------

inline int wrapi(int x, int min, int max)
{
    const int range = max - min;

    while (x < min)
        x += range;

    while (x >= max)
        x -= range;

    return x;
}
// -----------------------------------------------------------------------------

inline unsigned modi(int x, unsigned n)
{
    return ((x % (int)n) + (int)n) % (int)n;
}

// -----------------------------------------------------------------------------

inline fix32 arcFix32(u16 x)
{
    return arctab32[x & 1023];
}
