
// *****************************************************************************
//  SOR4 Slideshow
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

// *****************************************************************************
//
//  Subroutines
//
// *****************************************************************************

void showImage(const Image* image)
{
    VDP_drawImageEx( BG_B,
                     image,
                     TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX),
                     0, 2,
                     FALSE,
                     DMA );

    PAL_fadeIn(0, 15, image->palette->data, 20, FALSE);
    waitMs(7500);
    PAL_fadeOut(0, 15, 20, FALSE);
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
    VDP_setVerticalScroll(BG_B, -4);
    VDP_setPaletteColors(0, palette_black, 64);

    waitMs(1000);

    XGM_startPlay(xgm_sor403);

    while (TRUE)
    {
        showImage(&image_SOR4_1);
        showImage(&image_SOR4_2);
        showImage(&image_SOR4_3);
        showImage(&image_SOR4_4);
        showImage(&image_SOR4_5);
    }
}
