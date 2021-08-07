
// *****************************************************************************
//  Thunder Force II Intro
//
//  Written in 2021 by Andreas Dietrich
//
//  Based on the X68000 Thunder Force II version
//  Copyright(c) 1988 Tecnosoft
// *****************************************************************************

#define VDP_PRINT_1(x, y, fmt, arg0) \
{                                    \
    char buffer[128];                \
    sprintf(buffer, fmt, arg0);      \
    VDP_drawText(buffer, x, y);      \
}

// -----------------------------------------------------------------------------

inline void debugInfoScene0()
{
    VDP_resetScreen();

    VDP_PRINT_1(1, 1, "image_Tecnosoft_Background %0X", image_Tecnosoft_Background.tileset->numTile*32);

    u16 sum = image_Tecnosoft_Background.tileset->numTile*32;

    VDP_PRINT_1(1, 3, "Total sum: %0X", sum);

    while(1);
}