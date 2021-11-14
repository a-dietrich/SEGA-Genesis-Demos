
// *****************************************************************************
//  Thunder Force II Intro
//
//  Written in 2021 by Andreas Dietrich
//
//  Based on the X68000 Thunder Force II version
//  Copyright(c) 1988 Tecnosoft
// *****************************************************************************

// -----------------------------------------------------------------------------
//  Includes
// -----------------------------------------------------------------------------

// SGDK
#include <genesis.h>

// Resources
#include "resources.h"

// Debug helpers
#include "debug.h"

// -----------------------------------------------------------------------------
//  Defines
// -----------------------------------------------------------------------------

#define TITLE_BGA_ADDR           0xC000
#define TITLE_BGB_ADDR           0xE000
#define TITLE_WINDOW_ADDR        0xF000
#define TITLE_HSCROLL_TABLE_ADDR 0xF800
#define TITLE_SPRITE_LIST_ADDR   0xFC00

#define WAV_COPYRIGHT            64
#define WAV_SORRY                65
#define WAV_THUNDERFORCE         66

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

static const u16 palette_title[256] =
{
    0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00,
    0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xE00, 0xE00, 0xE20, 0xE40, 0xE60, 0xE80, 0xEA0, 0xEC0, 0xEE0, 0xEE2,
    0xEE4, 0xEE6, 0xEE8, 0xEEA, 0xEEC, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0x200, 0x400, 0x600, 0x800, 0xA00, 0xC00,
    0xE00, 0xE00, 0xE20, 0xE40, 0xE60, 0xE80, 0xEA0, 0xEC0, 0xEE0, 0xEE2, 0xEE4, 0xEE6, 0xEE8, 0xEEA, 0xEEC, 0xEEE,
    0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00,
    0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xC00, 0xE00, 0xE00, 0xE20, 0xE40, 0xE60, 0xE80, 0xEA0,
    0xEC0, 0xEE0, 0xEE2, 0xEE4, 0xEE6, 0xEE8, 0xEEA, 0xEEC, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0x200, 0x400, 0x600,
    0x800, 0xA00, 0xC00, 0xE00, 0xE00, 0xE20, 0xE40, 0xE60, 0xE80, 0xEA0, 0xEC0, 0xEE0, 0xEE2, 0xEE4, 0xEE6, 0xEE8,
    0xEEA, 0xEEC, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE,
    0xEEE, 0xEEE, 0xEEE, 0xEEE, 0x00C, 0x00C, 0x00C, 0x00C, 0x00C, 0x00C, 0x00C, 0x00C, 0x00C, 0x00E, 0x00E, 0x22E,
    0x44E, 0x66E, 0x88E, 0xAAE, 0xCCE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0x002, 0x004, 0x006, 0x008,
    0x00A, 0x00E, 0x00E, 0x22E, 0x44E, 0x66E, 0x88E, 0xAAE, 0xCCE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE,
    0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xCCC, 0xAAA, 0x888, 0x666,
    0xE40, 0xE80, 0xEE0, 0xEEE, 0xCCC, 0xAAA, 0x888, 0x666, 0xE40, 0xE80, 0xEE0, 0xEEE, 0xEEE, 0xEEE, 0xEEE, 0xEEE,
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

void resetScreen()
{
    VDP_setHorizontalScroll(BG_A, 0);
    VDP_setHorizontalScroll(BG_B, 0);
    VDP_setVerticalScroll(BG_A, 0);
    VDP_setVerticalScroll(BG_B, 0);
    VDP_clearPlane(BG_A, TRUE);
    VDP_clearPlane(BG_B, TRUE);
}

// -----------------------------------------------------------------------------

void resetSprites()
{
    SPR_reset();
    SPR_update();
}

// -----------------------------------------------------------------------------

Sprite* drawSprite(const SpriteDefinition* spriteDef, s16 x, s16 y, u16 attribute)
{
    Sprite* sprite = SPR_addSprite(spriteDef, 0, 0, attribute);

    u16 numTiles;
    SPR_loadAllFrames(spriteDef, g_tileIndex, &numTiles);
    SPR_setAutoTileUpload(sprite, FALSE);
    SPR_setVRAMTileIndex(sprite, g_tileIndex);
    SPR_setPosition(sprite, x, y);
    SPR_setFrame(sprite, 0);
    SPR_setVisibility(sprite, VISIBLE);

    g_tileIndex += numTiles;

    return sprite;
}

// -----------------------------------------------------------------------------

void resetTileIndex()
{
    g_tileIndex = TILE_USERINDEX;
}

// -----------------------------------------------------------------------------

u16 getTileIndex()
{
    return g_tileIndex;
}

// -----------------------------------------------------------------------------

u16 loadTileData(TileSet* tileSet)
{
    const u16 startIndex = g_tileIndex;

    VDP_loadTileData(tileSet->tiles, startIndex, tileSet->numTile, CPU);
    g_tileIndex += tileSet->numTile;

    return startIndex;
}

// -----------------------------------------------------------------------------

void fadePalettes(const u16* pal0, const u16* pal1, const u16* pal2, const u16* pal3, u16 numFrames, bool async)
{
    u16 palTemp[64];
    if (pal0) memcpy(palTemp+ 0, pal0, 16*2);
    if (pal1) memcpy(palTemp+16, pal1, 16*2);
    if (pal2) memcpy(palTemp+32, pal2, 16*2);
    if (pal3) memcpy(palTemp+48, pal3, 16*2);

    PAL_fadeToAll(palTemp, numFrames, async);
}

// -----------------------------------------------------------------------------

void playPCM(u8 id)
{
    XGM_startPlayPCM(id, 1, SOUND_PCM_CH2);
    do
    { 
        SYS_doVBlankProcess();
    }
    while (XGM_isPlayingPCM(SOUND_PCM_CH2_MSK));
}

// *****************************************************************************
//
//  Scenes
//
// *****************************************************************************

void scene0()
{
    #if 0
        debugInfoScene0();
    #endif

    // --------------
    // Initialization
    // --------------

    resetScreen();
    resetSprites();
    resetTileIndex();

    const u16 indexTecnosoftBackground = loadTileData(image_Tecnosoft_Background.tileset);

    VDP_setMapEx(
        BG_B,
        image_Tecnosoft_Background.tilemap,
        TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, indexTecnosoftBackground),
        0, 0, 0, 0, image_Tecnosoft_Background.tilemap->w, image_Tecnosoft_Background.tilemap->h
    );

    const s16 yBackgroundOffset = IS_PALSYSTEM ? 0 : 8;
    VDP_setVerticalScroll(BG_B, yBackgroundOffset);

    // ------------
    // Play scene 0
    // ------------

    // Fade in text
    const u16 tecnosoftBackground[1] = { 0x0A0 };
    PAL_fadeTo(2, 2, image_Tecnosoft_Background.palette->data+2, 16, FALSE); waitMs(125);
    PAL_fadeTo(1, 1, image_Tecnosoft_Background.palette->data+1, 16, FALSE);
    PAL_fadeTo(1, 1, tecnosoftBackground,                         4, FALSE);

    // Play sample
    playPCM(WAV_COPYRIGHT);

    // Fade to black
    PAL_fadeToAll(palette_black, 16, FALSE);

    // Exit
    waitMs(250);
}

// -----------------------------------------------------------------------------

void scene1()
{
    #if 0
        debugInfoScene1();
    #endif

    // --------------
    // Initialization
    // --------------

    resetScreen();
    resetSprites();
    resetTileIndex();

    const u16 indexPlanetBackground0 = loadTileData(image_Planet_Background_0.tileset);
    const u16 indexPlanetBackground1 = loadTileData(image_Planet_Background_1.tileset);
    const u16 indexPlanetBackground2 = loadTileData(image_Planet_Background_2.tileset);
    const u16 indexPlanetOverlay     = loadTileData(image_Planet_Overlay.tileset);

    Sprite* spritePlanet = drawSprite(&sprite_Planet_Sprites, 320, 72, TILE_ATTR(PAL3, FALSE, FALSE, FALSE));

    SPR_update();

    SYS_doVBlankProcess();
    PAL_setPalette(PAL0, image_Planet_Background_0.palette->data, DMA);
    PAL_setPalette(PAL1, image_Planet_Background_1.palette->data, DMA);
    PAL_setPalette(PAL2, image_Planet_Background_2.palette->data, DMA);
    PAL_setPalette(PAL3, sprite_Planet_Sprites.palette->data,     DMA);

    // ------------
    // Play scene 1
    // ------------

    // Start background music
    XGM_startPlay(xgm_01);

    // Intro sound
    playPCM(WAV_SORRY);

    // Scroll background and sprites
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
    waitMs(3250);
    {
        PAL_fadeToAll(palette_white, 32, FALSE);
    }

    // Exit
    waitMs(250);
}

// -----------------------------------------------------------------------------

void scene2()
{
    #if 0
        debugInfoScene2();
    #endif

    // --------------
    // Initialization
    // --------------

    resetScreen();
    resetSprites();
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

    const s16 ySpriteOffset = IS_PALSYSTEM ? 0 : -16;
    const u16 attribute = TILE_ATTR(PAL1, FALSE, FALSE, FALSE);
    drawSprite(&sprite_Explosion_Sprites_0, 96,     104+ySpriteOffset, attribute);
    drawSprite(&sprite_Explosion_Sprites_1, 96+128, 104+ySpriteOffset, attribute);

    SPR_update();

    // ------------
    // Play scene 2
    // ------------

    // Fade in background and sprites
    fadePalettes(
        image_Explosion_Background.palette->data,
        sprite_Explosion_Sprites_0.palette->data,
        image_Explosion_Overlay.palette->data,
        NULL,
        32,
        FALSE
    );

    // Show overlay text
    waitMs(0250);
    {
        const u16 yOverlayOffset = IS_PALSYSTEM ? 2 : 0;
        VDP_waitVSync();
        VDP_setMapEx(BG_A, image_Explosion_Overlay.tilemap, TILE_ATTR_FULL(PAL2, TRUE, FALSE, FALSE, indexExplosionOverlay), 8, 20+yOverlayOffset, 0, 0, 24, 7);
    }

    // Fade to black
    waitMs(4750);
    {
        PAL_fadeToAll(palette_black, 32, FALSE);
    }

    // Exit
    waitMs(250);
}

// -----------------------------------------------------------------------------

void scene3()
{
    #if 0
        debugInfoScene3();
    #endif

    // --------------
    // Initialization
    // --------------

    resetScreen();
    resetSprites();
    resetTileIndex();

    const u16 indexHangarBackground = loadTileData(image_Hangar_Background.tileset);
    const u16 indexHangarOverlay    = loadTileData(image_Hangar_Overlay.tileset);

    VDP_setMapEx(
        BG_B,
        image_Hangar_Background.tilemap,
        TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, indexHangarBackground),
        0, 0, 0, 0, image_Hangar_Background.tilemap->w, image_Hangar_Background.tilemap->h
    );

    const u16 attribute1 = TILE_ATTR(PAL1, FALSE, FALSE, FALSE);
    drawSprite(&sprite_Hangar_Sprites_0, 0,    28, attribute1);
    drawSprite(&sprite_Hangar_Sprites_1, 0+64, 28, attribute1);

    const u16 attribute2 = TILE_ATTR(PAL2, FALSE, FALSE, FALSE);
    drawSprite(&sprite_Hangar_Sprites_2, 136, 152, attribute2);
    drawSprite(&sprite_Hangar_Sprites_3, 200, 152, attribute2);
    drawSprite(&sprite_Hangar_Sprites_4, 256,  72, attribute2);

    const u16 attribute3 = TILE_ATTR(PAL3, FALSE, FALSE, FALSE);
    drawSprite(&sprite_Hangar_Sprites_5, 136, 160, attribute3);
    drawSprite(&sprite_Hangar_Sprites_6, 200, 152, attribute3);

    SPR_update();

    // ------------
    // Play scene 3
    // ------------

    // Fade in background and sprites
    fadePalettes(
        image_Hangar_Background.palette->data,
        sprite_Hangar_Sprites_0.palette->data,
        sprite_Hangar_Sprites_2.palette->data,
        sprite_Hangar_Sprites_5.palette->data,
        32,
        FALSE
    );

    // Show overlay text
    waitMs(0250);
    {
        VDP_waitVSync();
        VDP_setMapEx(BG_A, image_Hangar_Overlay.tilemap, TILE_ATTR_FULL(PAL1, TRUE, FALSE, FALSE, indexHangarOverlay), 6, 1, 0, 0, 28, 9);
    }

    // Fade to black
    waitMs(5000);
    {
        PAL_fadeToAll(palette_black, 32, FALSE);
    }

    // Exit
    waitMs(250);
}

// -----------------------------------------------------------------------------

void scene4()
{
    #if 0
        debugInfoScene4();
    #endif

    // -------------------------------------------------------------------------
    //  Interrupt handlers
    // -------------------------------------------------------------------------

    static s16   vintLineDisplay;
    static u16   vintVScroll;
    static u16   vintTileIndex;
    static fix32 vintTimeEnd;
    static fix32 vintTimeSound;
    static bool  vintSoundOn;

    // -------------------------------------------------------------------------

    void HIntHandler()
    {
        const u16 color = palette_title[vintLineDisplay++ & 0xFF];

        while (GET_HCOUNTER < 154)
        {}

        *((vu32*) GFX_CTRL_PORT) = GFX_WRITE_CRAM_ADDR((u32)(63*2));
        *((vu16*) GFX_DATA_PORT) = color;
    }

    // -------------------------------------------------------------------------
    
    void VIntHandler()
    {
        vintLineDisplay = 256-(8*4)-vintVScroll;

        VDP_setVerticalScroll(BG_A, -vintVScroll);

        // Check if scrolling is finished
        const s16 vintVScrollEnd = IS_PALSYSTEM ? 240-8 : 224;
        if (vintVScroll >= vintVScrollEnd)
        {
            // Clear top plane if end of scene is reached
            if (getTimeAsFix32(1) > vintTimeEnd)
            {
                VDP_clearPlane(BG_A, TRUE);

                // Stop horizontal interrupt to free up for main thread
                VDP_setHInterrupt(0);
            }
            // Play speech sample
            else if (!vintSoundOn && getTimeAsFix32(1) > vintTimeSound)
            {
                XGM_startPlayPCM(WAV_THUNDERFORCE, 1, SOUND_PCM_CH2);
                vintSoundOn = TRUE;
            }
            // Else draw copyright message
            else
            {
                VDP_setMapEx(
                    BG_A,
                    image_Title_ThunderForce.tilemap,
                    TILE_ATTR_FULL(PAL3, TRUE, FALSE, FALSE, vintTileIndex),
                    8, (32-1)-2, 0, (image_Title_ThunderForce.tilemap->h-1)-2,
                    image_Title_ThunderForce.tilemap->w, 3
                );
            }

            return;
        }

        // Determine source and destination tile rows
        const u16 yTile = vintVScroll >> 3;
        const s16 ys = (image_Title_ThunderForce.tilemap->h-1) - yTile;
        const s16 yd = (32-1) - yTile;

        vintVScroll += 8;

        if ((ys < 0) || (yd > 32-1-3))
            return;

        VDP_setMapEx(
            BG_A,
            image_Title_ThunderForce.tilemap,
            TILE_ATTR_FULL(PAL3, TRUE, FALSE, FALSE, vintTileIndex),
            8, yd, 0, ys,
            image_Title_ThunderForce.tilemap->w, 1
        );
    }

    // -------------------------------------------------------------------------
    //  Main thread
    // -------------------------------------------------------------------------

    // --------------
    // Initialization
    // --------------

    resetScreen();
    resetSprites();
    resetTileIndex();

    const u16 indexTitleBackground   = loadTileData(image_Title_Background.tileset);
    const u16 indexTitleOverlay      = loadTileData(image_Title_Overlay.tileset);
    const u16 indexTitleThunderForce = loadTileData(image_Title_ThunderForce.tileset);

    VDP_setMapEx(
        BG_B,
        image_Title_Background.tilemap,
        TILE_ATTR_FULL(PAL0, FALSE, FALSE, FALSE, indexTitleBackground),
        0, 0, 0, 0, image_Title_Background.tilemap->w, image_Title_Background.tilemap->h
    );
    
    const s16 yBackgroundOffset = IS_PALSYSTEM ? 0 : 16;
    VDP_setVerticalScroll(BG_B, yBackgroundOffset);

    const s16 ySpriteOffset = IS_PALSYSTEM ? 0: -16;

    const u16 attribute1 = TILE_ATTR(PAL1, FALSE, FALSE, FALSE);
    drawSprite(&sprite_Title_Sprites_0,   32,   0+ySpriteOffset, attribute1);
    drawSprite(&sprite_Title_Sprites_1,   96, 112+ySpriteOffset, attribute1);
    drawSprite(&sprite_Title_Sprites_2,  152,  40+ySpriteOffset, attribute1);
    drawSprite(&sprite_Title_Sprites_3,    8, 152+ySpriteOffset, attribute1);
    drawSprite(&sprite_Title_Sprites_4,  152, 200+ySpriteOffset, attribute1);
    drawSprite(&sprite_Title_Sprites_5,  288, 112+ySpriteOffset, attribute1);
    drawSprite(&sprite_Title_Sprites_6,  232,  72+ySpriteOffset, attribute1);
    drawSprite(&sprite_Title_Sprites_7,  200,  16+ySpriteOffset, attribute1);
    drawSprite(&sprite_Title_Sprites_8,  216,  48+ySpriteOffset, attribute1);

    const u16 attribute2 = TILE_ATTR(PAL2, FALSE, FALSE, FALSE);
    drawSprite(&sprite_Title_Sprites_9,   72, 224+ySpriteOffset, attribute2);
    drawSprite(&sprite_Title_Sprites_10, 160, 144+ySpriteOffset, attribute2);
    drawSprite(&sprite_Title_Sprites_11,   8, 176+ySpriteOffset, attribute2);
    drawSprite(&sprite_Title_Sprites_12,  28,  54+ySpriteOffset, attribute2);
    drawSprite(&sprite_Title_Sprites_13, 288, 192+ySpriteOffset, attribute2);
    drawSprite(&sprite_Title_Sprites_14, 264, 120+ySpriteOffset, attribute2);
    drawSprite(&sprite_Title_Sprites_15, 240,  64+ySpriteOffset, attribute2);
    drawSprite(&sprite_Title_Sprites_16, 216,   0+ySpriteOffset, attribute2);

    const u16 attribute3 = TILE_ATTR(PAL3, FALSE, FALSE, FALSE);
    drawSprite(&sprite_Title_Sprites_17, 168, 120+ySpriteOffset, attribute3);
    drawSprite(&sprite_Title_Sprites_18, 240,  64+ySpriteOffset, attribute3);

    SPR_update();

    // ------------
    // Play scene 4
    // ------------

    // Fade in background and sprites
    fadePalettes(
        image_Title_Background.palette->data,
        sprite_Title_Sprites_0.palette->data,
        sprite_Title_Sprites_9.palette->data,
        image_Title_ThunderForce.palette->data,
        32,
        FALSE
    );

    // Show overlay text
    waitMs(250);
    {
        VDP_waitVSync();
        VDP_setMapEx(BG_A, image_Title_Overlay.tilemap, TILE_ATTR_FULL(PAL3, TRUE, FALSE, FALSE, indexTitleOverlay), 6, 1, 0, 0, 28, 5);
    }

    // Clear overlay text
    waitMs(4750);
    {
        VDP_waitVSync();
        VDP_clearPlane(BG_A, TRUE);
    }

    // Start interrupt handlers and scroll title
    waitMs(250);
    SYS_disableInts();
    {
        vintVScroll   = 0;
        vintTileIndex = indexTitleThunderForce;
        vintTimeEnd   = getTimeAsFix32(1) + FIX32(6);
        vintTimeSound = getTimeAsFix32(1) + FIX32(0.25);
        vintSoundOn   = FALSE;

        VDP_setHIntCounter(0);
        VDP_setHInterrupt(1);
        SYS_setHIntCallback(HIntHandler);
        SYS_setVIntCallback(VIntHandler);
    }
    SYS_enableInts();

    // Wait until end time of scene is reached
    while (getTimeAsFix32(1) < vintTimeEnd)
        SYS_doVBlankProcess();
    
    // Stop background music
    XGM_stopPlay();

    // Fade out
    waitMs(500);
    {
        PAL_fadeToAll(palette_black, 32, FALSE);
    }

    // Turn of interrupts
    SYS_disableInts();
    {
        VDP_setHInterrupt(0);
        SYS_setHIntCallback(0);
        SYS_setVIntCallback(0);
    }
    SYS_enableInts();

    // Exit
    waitMs(500);
}

// *****************************************************************************
//
//  Main
//
// *****************************************************************************

int main()
{
    // -------------
    // Initalization
    // -------------

    // VDP memory layout
    VDP_setBGAAddress         ( TITLE_BGA_ADDR           );
    VDP_setBGBAddress         ( TITLE_BGB_ADDR           );
    VDP_setWindowAddress      ( TITLE_WINDOW_ADDR        );
    VDP_setSpriteListAddress  ( TITLE_SPRITE_LIST_ADDR   );
    VDP_setHScrollTableAddress( TITLE_HSCROLL_TABLE_ADDR );

    // Setup VDP
    VDP_setScreenHeight240();
    VDP_setPlanSize(64, 32);
    VDP_setScrollingMode(HSCROLL_PLANE, VSCROLL_PLANE);

    // Init sprites
    SPR_init();

    // Clear palettes
    SYS_doVBlankProcess();
    PAL_setColors(0, palette_black, 64, DMA);

    // Register sound effects
    XGM_setPCM(WAV_COPYRIGHT,    wav_Copyright,    sizeof(wav_Copyright));
    XGM_setPCM(WAV_SORRY,        wav_Sorry,        sizeof(wav_Sorry));
    XGM_setPCM(WAV_THUNDERFORCE, wav_ThunderForce, sizeof(wav_ThunderForce));

    // ---------
    // Main loop
    // ---------

    while (TRUE)
    {
        scene0();
        scene1();    
        scene2();
        scene3();
        scene4();
    }
}
