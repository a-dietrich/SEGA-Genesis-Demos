
// *****************************************************************************
//  Console example
//
//  Written in 2022 by Andreas Dietrich
// *****************************************************************************

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------

// SGDK
#include <genesis.h>

// Resources
#include "resources.h"

// Console
#include "console.h"

// -----------------------------------------------------------------------------
//  Defines
// -----------------------------------------------------------------------------

// VDP addresses
#define HSCROLL_TABLE_ADDR 0xA800
#define SPRITE_LIST_ADDR   0xAC00
#define WINDOW_ADDR        0xB000
#define BGA_ADDR           0xC000
#define BGB_ADDR           0xE000

// *****************************************************************************
//
//  Subroutines
//
// *****************************************************************************

void setupBackground()
{
    // Background pattern size in tiles
    const u16 w = 64/8;
    const u16 h = 64/8;

    // Fill screen
    for (s16 x=0; x<16*w; x+=w)
        for (s16 y=-h; y<3*h; y+=h)
        {
            // Shift each image column 3 tiles further
            const s16 shift = 3*(x/w);
            VDP_drawImageEx(
                BG_B,
                &image_R_Type_BG,
                TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX),
                x, y+shift,
                FALSE,
                DMA
            );
        }
}

// -----------------------------------------------------------------------------

void setupForeground()
{
    // Foreground image occupies complete plane
    VDP_drawImageEx(
        BG_A,
        &image_R_Type_FG,
        TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX+64),
        0, 0,
        FALSE,
        DMA
    );
}

// -----------------------------------------------------------------------------

void setupWindow()
{
    // Clear visible window area on the right (16x28 tiles)
    VDP_fillTileMapRect(
        WINDOW,
        TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, 8),
		40-16,
		0,
		16,
		28
	);
}

// *****************************************************************************
//
//  Main
//
// *****************************************************************************

int main()
{
    VDP_setScreenWidth320();
    VDP_setScreenHeight224();
    VDP_setPlaneSize(128, 32, TRUE);
    VDP_setPaletteColors(0, palette_black, 64);

    // VDP memory layout
    VDP_setBGAAddress         ( BGA_ADDR           );
    VDP_setBGBAddress         ( BGB_ADDR           );
    VDP_setWindowAddress      ( WINDOW_ADDR        );
    VDP_setSpriteListAddress  ( SPRITE_LIST_ADDR   );
    VDP_setHScrollTableAddress( HSCROLL_TABLE_ADDR );

    VDP_loadFontData(tileset_Font_Namco_Opaque.tiles, 96, CPU);

    // Prepare tile maps
    setupBackground();
    setupForeground();
    setupWindow();

    PAL_fadeIn( 1, 15, image_R_Type_BG.palette->data+1, 16, TRUE);
    PAL_setPaletteColors (16, &palette_Font_Namco_Opaque, CPU);

    VDP_setTextPalette(PAL1);
    VDP_setTextPlane(WINDOW);

    bool windowVisible = FALSE;

    CON_setTransferMethod(DMA_QUEUE);

    do
    {
        for (s16 x=0; x<(128*8); x++)
        {
            SYS_doVBlankProcess();

            // Horizontal scroll values
            const s16 xb = -x/2;
            const s16 xa = -x;

            VDP_setHorizontalScroll(BG_B, xb);
            VDP_setHorizontalScroll(BG_A, xa);

            // Wait a while then gradually show window
            if (x>144 && !windowVisible)
            {
                // Set window size
                const s16 w = x - 144;
                VDP_setWindowHPos(TRUE, 20-w);

                // Window fully visible
                if (w == 8)
                {
                    // Print header
                    CON_setSize((40-16)+1, 1, 14, 27);
                    CON_write(" <START> for\n");
                    CON_write(" assert test\n");

                    // Place console area below header
                    CON_setSize((40-16)+1, 4, 14, 24);
                    windowVisible = TRUE;
                }
            }

            // Write scroll values
            if (windowVisible)
                CON_write("A:%04hd B:%04hd\n", -xa, -xb);

            // *****************************************************************
            // Simulated assert
            // *****************************************************************

            const u16 joyState = JOY_readJoypad(JOY_1);
            if (joyState & BUTTON_START)
            {
                // Simulate wrong x value
                x = 1025;
            }

            assert(x < 1024);

            // *****************************************************************
        }
    }
    while (TRUE);
}
