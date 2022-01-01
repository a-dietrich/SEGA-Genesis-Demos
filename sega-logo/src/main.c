
// *****************************************************************************
//  SEGA Logo
//
//  Written in 2021 by Andreas Dietrich
// *****************************************************************************

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------

// SGDK
#include <genesis.h>

// Resources
#include "resources.h"

// -----------------------------------------------------------------------------
//  Macros
// -----------------------------------------------------------------------------

#define CLAMP(x, minimum, maximum) (min(max((x),(minimum)),(maximum)))

// *****************************************************************************
//
//  Global variables
//
// *****************************************************************************

u16 lineHInt      = 0;
u32 frameCtr      = 0;
u16 tileIndexNext = TILE_USERINDEX;

u16 paletteBuffer[4*16];
s16 vScrollBuffer[32*16];

u16 colorsSEGAGradient[26] =
{
    // Blue/cyan cycling gradient
    0x200, 0x400, 0x600, 0x800, 0xA00, 0xC00, 0xE00, 0xE20,
    0xE40, 0xE60, 0xE80, 0xEA0, 0xEC0, 0xEE0, 0xEC0, 0xEA0,
    0xE80, 0xE60, 0xE40, 0xE20, 0xE00, 0xC00, 0xA00, 0x800,
    0x600, 0x400
};

// *****************************************************************************
//
//  Subroutines
//
// *****************************************************************************

void fillVerticalScrollTile(VDPPlane plan, u16 tile, s16 value, u16 len, TransferMethod tm)
{
    s16 buffer[32];
    memsetU16((u16*)buffer, (u16)value, len & 0x1F);

    VDP_setVerticalScrollTile(plan, tile, buffer, len, tm);
}

// -----------------------------------------------------------------------------

u16 drawImage(VDPPlane plane, const Image *image, u16 basetile, u16 x, u16 y, u16 loadpal, bool dma)
{
    const u16 tileIndex = tileIndexNext;
    basetile |= tileIndex;

    VDP_drawImageEx(plane, image, basetile, x, y, loadpal, dma);
    tileIndexNext += image->tileset->numTile;  

    return tileIndex;
}

// -----------------------------------------------------------------------------

void changePaletteBrightness(u16 *dstPal, u16 *srcPal, s16 val, u16 count)
{
    for (u16 i=0; i<count; i++)
    {
        const u16 color = srcPal[i];

        s16 b = color & 0xF00;
        s16 g = color & 0x0F0;
        s16 r = color & 0x00F;

        const s16 db = val * 256;
        const s16 dg = val *  16;
        const s16 dr = val;

        b = CLAMP(b + db, 0, 0xF00);
        g = CLAMP(g + dg, 0, 0x0F0);
        r = CLAMP(r + dr, 0, 0x00F);

        dstPal[i] = b | g | r;
    }
}

// -----------------------------------------------------------------------------

void cyclePaletteUp(u16 *pal, u16 index, u16 count)
{
    if (count < 2)
        return;

    u16 tmp = pal[index+count-1];
    for (u16 i=index+count-1, n=index; i>n; i--)
        pal[i] = pal[i-1];
    pal[index] = tmp;
}

// *****************************************************************************
//
//  Main
//
// *****************************************************************************

int main()
{
    // -------------------------------------------------------------------------
    //  Interrupt handlers
    // -------------------------------------------------------------------------

    void HIntHandler()
    {
        vu16 *data;
        vu32 *ctrl;

        // Check if we are outside the logo
        const u16 y = lineHInt - 96;
        if (y >= 32)
        {
            // Prime control and data ports
            __asm volatile (
                "   lea     0xC00004,%0;"
                "   move.w  #0x8F04,(%0);"     // Set auto-increment to 4
                "   move.l  #0x401E0010,(%0);" // Set VSRAM address (two-tile column 7)
                : "=>a" (ctrl)
                : "0" (ctrl)
            );

            lineHInt++;
            return;
        }

        // Change vscroll values if inside the logo
        s16 *addr = vScrollBuffer+(y<<3);
	    __asm volatile (
            "   lea     0xC00000,%1;"
            "	move.l  (%0)+,(%1);"
            "	move.l  (%0)+,(%1);"
            "	move.l  (%0)+,(%1);"
            "   lea     4(%1),%2;"
            "   move.l  #0x401E0010,(%2);" // Reset VSRAM address (two-tile column 7)
            : "=>a" (addr), "=>a" (data), "=>a" (ctrl)
            : "0" (addr), "1" (data), "2" (ctrl)
        );

        lineHInt++;
    }

    void VIntHandler()
    {
        frameCtr++;
        lineHInt = 0;
    }

    // -------------------------------------------------------------------------
    //  Main thread
    // -------------------------------------------------------------------------

    //
    // Initalization
    //

    // Setup VDP
    VDP_setPlanSize(64, 32);
    VDP_setScrollingMode(HSCROLL_TILE, VSCROLL_2TILE);
    VDP_setPaletteColors(0, palette_black, 64);

    // Draw SEGA logo
    {
        inline u16 attr(u16 pal) { return TILE_ATTR_FULL(pal, FALSE, FALSE, FALSE, 0); }

        // Fill top plane with solid black tiles
        VDP_fillTileMapRect(BG_A, attr(PAL0)|1, 0, 0, 40, 28); 

        // Draw logo outline and letters. There are two versions using different color registers.
        const u16 tileIndexSEGAOutline   = drawImage(BG_A, &image_SEGA_Outline,    attr(PAL0), 14, 12, FALSE, DMA);
        const u16 tileIndexSEGAGradient1 = drawImage(BG_B, &image_SEGA_Gradient_1, attr(PAL0), 14,  0, FALSE, DMA);
        const u16 tileIndexSEGAGradient0 = drawImage(BG_B, &image_SEGA_Gradient_0, attr(PAL1), 14,  4, FALSE, DMA);

        // Copy letters using different palettes
        VDP_setTileMapEx(BG_B, image_SEGA_Gradient_1.tilemap, attr(PAL1)|tileIndexSEGAGradient1, 14,  8, 0, 0, 12, 4, DMA);
        VDP_setTileMapEx(BG_B, image_SEGA_Gradient_0.tilemap, attr(PAL2)|tileIndexSEGAGradient0, 14, 12, 0, 0, 12, 4, DMA);
        VDP_setTileMapEx(BG_B, image_SEGA_Gradient_1.tilemap, attr(PAL2)|tileIndexSEGAGradient1, 14, 16, 0, 0, 12, 4, DMA);
        VDP_setTileMapEx(BG_B, image_SEGA_Gradient_0.tilemap, attr(PAL3)|tileIndexSEGAGradient0, 14, 20, 0, 0, 12, 4, DMA);
        VDP_setTileMapEx(BG_B, image_SEGA_Gradient_1.tilemap, attr(PAL3)|tileIndexSEGAGradient1, 14, 24, 0, 0, 12, 4, DMA);  
    }

    // Setup vertical scroll values
    fillVerticalScrollTile(BG_A, 7,     0, 6, DMA); // Logo outline position
    fillVerticalScrollTile(BG_B, 7, -32*3, 6, DMA); // Backdrop (show normal colored letters) 

    // Draw instruction text
    VDP_drawText("A: Cycle", 1, 25);
    VDP_drawText("B: Move bar", 1, 26);

    // Fade in graphics
    PAL_fadeTo(1, 15, image_SEGA_Outline.palette->data+1, 16, FALSE);

    // Setup interrupt handlers
    SYS_disableInts();
    {
        VDP_setHIntCounter(0);
        VDP_setHInterrupt(1);
        SYS_setHIntCallback(HIntHandler);
        SYS_setVIntCallback(VIntHandler);
    }
    SYS_enableInts();

    //
    // Display loop
    //

    s16 highlightBarY = 10;

    while (TRUE)
    {
        const u16 joyState = JOY_readJoypad(JOY_1);

        // Cycle base palette
        if (joyState & BUTTON_A)
            if (frameCtr % 2 == 0)
                cyclePaletteUp(colorsSEGAGradient, 0, 26);
    
        // Create brightened versions of the original palette
        for (u16 i=0; i<7; i++)
            changePaletteBrightness(paletteBuffer+(i+1)*8+1, colorsSEGAGradient, i*2, 7);

        // Move highlight bar
        if (joyState & BUTTON_B)
            if (frameCtr % 2 == 0)
                if (++highlightBarY >= 32+6)
                    highlightBarY = -14;

        // Reset vscroll values so that the logo with base colors is shown
        memsetU16((u16*)vScrollBuffer, -3*32, 32*8);

        // Compute vscroll values for highligh bar
        for (s16 line=1; line<=12; line++)
        {
            // The bar covers 6 two-tile columns
            for (u16 column=0; column<6; column++)
            {
                const s16 tilt       = column;                                   // shift per column
                const s16 brightness = (line >= 7) ? (13 - line) : line;         // brightness per line
                const s16 index      = (highlightBarY + line - tilt)*8 + column; // buffer index of vscroll value

                // Insert vscroll values for highlight bar
                if ((index >= 0) && (index < 32*8))
                    vScrollBuffer[index] = (brightness - 3) * 32;
            }
        }

        // Update all palettes
        SYS_doVBlankProcess();
        PAL_setColors(8, paletteBuffer+8, 7*8, DMA);
    }
}
