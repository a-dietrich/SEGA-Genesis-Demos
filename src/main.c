
// *****************************************************************************
//  Scaling Example
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

#include "Helpers.h"

// *****************************************************************************
//
//  Data
//
// *****************************************************************************

static const u16 palette_white[64] =
{
    0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE,
    0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE,
    0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE,
    0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE
};

// *****************************************************************************
//
//  Global variables
//
// *****************************************************************************

u16 g_tileIndex = 0;

// *****************************************************************************
//
//  Subroutines
//
// *****************************************************************************

#define VDP_PRINT_1(x, y, fmt, arg0) \
{                                    \
    char buffer[128];                \
    sprintf(buffer, fmt, arg0);      \
    VDP_drawText(buffer, x, y);      \
}
    
Sprite* drawSprite(const SpriteDefinition* spriteDef, s16 x, s16 y, u16 attribute, u16* index)
{
    Sprite* sprite = SPR_addSprite(spriteDef, 0, 0, attribute);

    u16 numTiles;
    SPR_loadAllFrames(spriteDef, *index, &numTiles);
    SPR_setAutoTileUpload(sprite, FALSE);
    SPR_setVRAMTileIndex(sprite, *index);
    SPR_setPosition(sprite, x, y);
    SPR_setFrame(sprite, 0);
    SPR_setVisibility(sprite, VISIBLE);

    *index += numTiles;

    return sprite;
}

void resetScreen()
{
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setHorizontalScroll(BG_B, 0);
    VDP_setVerticalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_B, 0);
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
    VDP_clearSprites();
}

void resetTileIndex()
{
    g_tileIndex = TILE_USERINDEX;
}

u16 getTileIndex()
{
    return g_tileIndex;
}

u16 loadTileData(TileSet* tileSet)
{
    const u16 startIndex = g_tileIndex;

    VDP_loadTileData(tileSet->tiles, startIndex, tileSet->numTile, DMA);
    g_tileIndex += tileSet->numTile;

    return startIndex;
}

// *****************************************************************************
//
//  Main
//
// *****************************************************************************

#define TITLE_BGA_ADDR           0xC000
#define TITLE_BGB_ADDR           0xE000
#define TITLE_WINDOW_ADDR        0xF000
#define TITLE_HSCROLL_TABLE_ADDR 0xF800
#define TITLE_SPRITE_LIST_ADDR   0xFC00

#define TILE_STARTINDEX          TILE_USERINDEX

void scene1()
{
#if 0
    VDP_PRINT_1(1,1, "image_Planet_Background_0 %0X", image_Planet_Background_0.tileset->numTile*32);
    VDP_PRINT_1(1,2, "image_Planet_Background_1 %0X", image_Planet_Background_1.tileset->numTile*32);
    VDP_PRINT_1(1,3, "image_Planet_Background_2 %0X", image_Planet_Background_2.tileset->numTile*32);

    u16 sum = image_Planet_Background_0.tileset->numTile*32
            + image_Planet_Background_1.tileset->numTile*32
            + image_Planet_Background_2.tileset->numTile*32;

    VDP_PRINT_1(1, 4, "BG sum: %0X", sum);

    VDP_PRINT_1(1, 6, "sprite_Planet_Sprites %0X", sprite_Planet_Sprites.maxNumTile*32);

    VDP_PRINT_1(1, 8, "image_Planet_Overlay %0X", image_Planet_Overlay.tileset->numTile*32);

    sum += sprite_Planet_Sprites.maxNumTile*32;
    sum += image_Planet_Overlay.tileset->numTile*32;

    VDP_PRINT_1(1, 10, "Total sum: %0X", sum);

    while(1);
#endif

    resetScreen();
    resetTileIndex();

    const u16 indexPlanetBackground0 = loadTileData(image_Planet_Background_0.tileset);
    const u16 indexPlanetBackground1 = loadTileData(image_Planet_Background_1.tileset);
    const u16 indexPlanetBackground2 = loadTileData(image_Planet_Background_2.tileset);
    const u16 indexPlanetOverlay     = loadTileData(image_Planet_Overlay.tileset);

    SPR_reset();

    Sprite* spritePlanet = drawSprite(&sprite_Planet_Sprites, 320, 72, TILE_ATTR(PAL3, FALSE, FALSE, FALSE), &g_tileIndex);

    SPR_update();

    VDP_waitVSync();
    VDP_setPaletteColors( 0, image_Planet_Background_0.palette->data, 16);
    VDP_setPaletteColors(16, image_Planet_Background_1.palette->data, 16);
    VDP_setPaletteColors(32, image_Planet_Background_2.palette->data, 16);
    VDP_setPaletteColors(48, sprite_Planet_Sprites.palette->data,     16);

    // Scroll background
    {
        for (s16 hScroll=0; hScroll<=320; hScroll+=8)
        {
            SPR_setPosition(spritePlanet, 320+120-hScroll-8, 72);
            SPR_setVisibility(spritePlanet, !((320+120-hScroll-8)<320));

            SYS_doVBlankProcess();

            if (hScroll<320)
                SPR_update();

            VDP_setHorizontalScroll(BG_B, -hScroll);

            const u16 xd = hScroll >> 3;

            const u16 xd0 = image_Planet_Background_0.tilemap->w;
            const u16 xd1 = image_Planet_Background_0.tilemap->w + image_Planet_Background_1.tilemap->w;

            if (xd < xd0)
            {
                const u16 xs = xd;
                VDP_setMapEx(BG_B, image_Planet_Background_0.tilemap, TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, indexPlanetBackground0), 40+xd, 0, xs, 0, 1, 30);
            }
            else if (xd < xd1)
            {
                const u16 xs = xd - xd0;
                VDP_setMapEx(BG_B, image_Planet_Background_1.tilemap, TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, indexPlanetBackground1), 40+xd, 0, xs, 0, 1, 30);
            }
            else
            {
                const u16 xs = xd - xd1;
                VDP_setMapEx(BG_B, image_Planet_Background_2.tilemap, TILE_ATTR_FULL(PAL2, FALSE, FALSE, FALSE, indexPlanetBackground2), 40+xd, 0, xs, 0, 1, 30);
            }
        }
    }

    // Show overlay text
    waitMs(250);
    {
        const u16 yOverlayOffset = IS_PALSYSTEM ? 2 : 0;
        VDP_waitVSync();
        VDP_setHorizontalScroll(BG_A, 4);
        VDP_setMapEx(BG_A, image_Planet_Overlay.tilemap, TILE_ATTR_FULL(PAL2, TRUE, FALSE, FALSE, indexPlanetOverlay), 7, 18+yOverlayOffset, 0, 0, 26, 9);
    }

    // Fade to white
    waitMs(2000);
    {
        PAL_fadeToAll(palette_white, 32, FALSE);
    }

    // Exit
    waitMs(500);
}

void scene2()
{
    resetScreen();
    resetTileIndex();

    const u16 indexExplosionBackground = loadTileData(image_Explosion_Background.tileset);
    const u16 indexExplosionOverlay    = loadTileData(image_Explosion_Overlay.tileset);
 
    VDP_setMapEx(
        BG_B,
        image_Explosion_Background.tilemap,
        TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, indexExplosionBackground),
        0, 0, 0, 0, image_Explosion_Background.tilemap->w, image_Explosion_Background.tilemap->h
    );

    const s16 yBackgroundOffset = IS_PALSYSTEM ? 0 : 16;
    VDP_setVerticalScroll(BG_B, yBackgroundOffset);

    SPR_reset();

    const s16 ySpriteOffset = IS_PALSYSTEM ? 0: -16;
    const u16 attribute = TILE_ATTR(PAL1, FALSE, FALSE, FALSE);
    drawSprite(&sprite_Explosion_Sprites_0, 96,     104+ySpriteOffset, attribute, &g_tileIndex);
    drawSprite(&sprite_Explosion_Sprites_1, 96+128, 104+ySpriteOffset, attribute, &g_tileIndex);

    SPR_update();

    // Fade in background and sprites
    {
        u16 palTemp[64];
        memcpy(palTemp+ 0, image_Explosion_Background.palette->data, 16*2);
        memcpy(palTemp+16, sprite_Explosion_Sprites_0.palette->data, 16*2);
        memcpy(palTemp+32, image_Explosion_Overlay.palette->data,    16*3);

        PAL_fadeToAll(palTemp, 48, FALSE);
    }

    // Show overlay text
    waitMs(0250);
    {
        const u16 yOverlayOffset = IS_PALSYSTEM ? 2 : 0;
        VDP_waitVSync();
        VDP_setMapEx(BG_A, image_Explosion_Overlay.tilemap, TILE_ATTR_FULL(PAL2, TRUE, FALSE, FALSE, indexExplosionOverlay), 8, 20+yOverlayOffset, 0, 0, 24, 7);
    }

    // Fade to black
    waitMs(2000);
    {
        PAL_fadeToAll(palette_black, 32, FALSE);
    }

    // Exit
    waitMs(500);
}

void scene3()
{
    resetScreen();
    resetTileIndex();

#if 0
    VDP_PRINT_1(1,1, "image_Scene3_0 %0X",  image_Scene3_0.tileset->numTile*32);
    VDP_PRINT_1(1,2, "image_Overlay_2 %0X", image_Overlay_2.tileset->numTile*32);

    u16 sum = image_Scene3_0.tileset->numTile*32
            + image_Overlay_2.tileset->numTile*32;

    VDP_PRINT_1(1, 4, "BG sum: %0X", sum);

    VDP_PRINT_1(1, 6, "sprite_Hangar_0 %0X", 32*sprite_Hangar_0.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1, 7, "sprite_Hangar_1 %0X", 32*sprite_Hangar_1.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1, 8, "sprite_Hangar_2 %0X", 32*sprite_Hangar_2.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1, 9, "sprite_Hangar_3 %0X", 32*sprite_Hangar_3.animations[0]->frames[0]->tileset->numTile);

    VDP_PRINT_1(1,10, "sprite_Hangar_4 %0X", 32*sprite_Hangar_4.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,11, "sprite_Hangar_5 %0X", 32*sprite_Hangar_5.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,12, "sprite_Hangar_6 %0X", 32*sprite_Hangar_6.animations[0]->frames[0]->tileset->numTile);

    sum += 32*sprite_Hangar_0.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_1.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_2.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_3.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_4.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_5.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_6.animations[0]->frames[0]->tileset->numTile;

    VDP_PRINT_1(1, 14, "Total sum: %0X", sum);

    while(1);
#endif

    u16 index = TILE_STARTINDEX;

    const u16 indexScene3_0 = index;
    VDP_loadTileData(image_Scene3_0.tileset->tiles, index, image_Scene3_0.tileset->numTile, DMA);
    index += image_Scene3_0.tileset->numTile;

    VDP_setMapEx(
        BG_B,
        image_Scene3_0.tilemap,
        TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, indexScene3_0),
        0, 0, 0, 0, image_Scene3_0.tilemap->w, image_Scene3_0.tilemap->h
    );

#if 0
   VDP_drawImageEx( BG_B,
                    &image_Scene3_0,
                    TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, TILE_USERINDEX),
                    0, 0,
                    TRUE,
                    DMA );
   VDP_drawImageEx( BG_A,
                    &image_Scene3_2,
                    TILE_ATTR_FULL(PAL1, FALSE, FALSE, FALSE, TILE_USERINDEX+0x7000/32),
                    0, 0,
                    TRUE,
                    DMA );
#endif

    u16 numTile;

    Sprite* drawSprite(const SpriteDefinition* spriteDef, s16 x, s16 y, u16 attribute)
    {
        Sprite* sprite = SPR_addSprite(spriteDef, 0, 0, attribute);

        SPR_loadAllFrames(spriteDef, index, &numTile);
        SPR_setAutoTileUpload(sprite, FALSE);
        SPR_setVRAMTileIndex(sprite, index);
        SPR_setPosition(sprite, x, y);
        SPR_setFrame(sprite, 0);
        SPR_setVisibility(sprite, VISIBLE);

        index += numTile;

        return sprite;
    }

    SPR_init();

    // Sprite* ship = SPR_addSprite(&sprite_Hangar_0, 0, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));
    // Sprite* spr1 = SPR_addSprite(&sprite_Hangar_1, 0, 0, TILE_ATTR(PAL2, FALSE, FALSE, FALSE));

    u16 attribute = TILE_ATTR(PAL1, FALSE, FALSE, FALSE);

    drawSprite(&sprite_Hangar_0,   0,  32, attribute);
    drawSprite(&sprite_Hangar_1,  72, 152, attribute);
    drawSprite(&sprite_Hangar_2, 160, 160, attribute);
    drawSprite(&sprite_Hangar_3, 216, 152, attribute);

    attribute = TILE_ATTR(PAL2, FALSE, FALSE, FALSE);

    drawSprite(&sprite_Hangar_4,  72, 152, attribute);
    drawSprite(&sprite_Hangar_5, 168, 152, attribute);
    drawSprite(&sprite_Hangar_6, 232,  72, attribute);

    SPR_update();

    u16 palTemp[64];
    memcpy(palTemp+ 0, palette_Hangar_0.data, 16*2);
    memcpy(palTemp+16, palette_Hangar_1.data, 16*2);
    memcpy(palTemp+32, palette_Hangar_2.data, 16*2);
    memcpy(palTemp+48, palette_Hangar_3.data, 16*2);

    PAL_fadeToAll(palTemp, 48, FALSE);

#if 1
    const u16 indexPlanetOverlay = index;
    VDP_loadTileData(image_Overlay_2.tileset->tiles, index, image_Overlay_2.tileset->numTile, DMA);
    index += image_Overlay_2.tileset->numTile;

    VDP_setMapEx(BG_A, image_Overlay_2.tilemap, TILE_ATTR_FULL(PAL3, TRUE, FALSE, FALSE, indexPlanetOverlay), 1, 1, 0, 0, 38, 9);
#endif
}

int main()
{
    // -------------------------------------------------------------------------
    //  Interrupt handlers
    // -------------------------------------------------------------------------

    void HIntHandler()
    {
    }
    void VIntHandler()
    {
    }

    // -------------------------------------------------------------------------
    //  Main thread
    // -------------------------------------------------------------------------

    //
    // Initalization
    //

#if 1
    // VDP memory layout
    VDP_setBGAAddress         ( TITLE_BGA_ADDR           );
    VDP_setBGBAddress         ( TITLE_BGB_ADDR           );
    VDP_setWindowAddress      ( TITLE_WINDOW_ADDR        );
    VDP_setSpriteListAddress  ( TITLE_SPRITE_LIST_ADDR   );
    VDP_setHScrollTableAddress( TITLE_HSCROLL_TABLE_ADDR );
#endif

    // Setup VDP
    VDP_setScreenHeight240();
    VDP_setPlanSize(64, 32);
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

    SPR_init();

    // PAL_setColors(0, palette_black, 64);

    XGM_startPlay(xgm_01);

    scene1();    
    scene2();
    scene3();
}
