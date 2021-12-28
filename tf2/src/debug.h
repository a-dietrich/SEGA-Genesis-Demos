
// *****************************************************************************
//  Thunder Force II Intro
//
//  Written in 2021 by Andreas Dietrich
//
//  Based on the X68000 Thunder Force II version
//  Copyright (c) 1988 Tecnosoft
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

    VDP_PRINT_1(1, 1, "image_Tecnosoft_Background %4X", image_Tecnosoft_Background.tileset->numTile*32);

    u16 sum = image_Tecnosoft_Background.tileset->numTile*32;

    VDP_PRINT_1(1, 3, "Total sum: %4X", sum);

    while(1);
}

// -----------------------------------------------------------------------------

inline void debugInfoScene1()
{
    VDP_resetScreen();

    VDP_PRINT_1(1,1, "image_Planet_Background_0 %4X", image_Planet_Background_0.tileset->numTile*32);
    VDP_PRINT_1(1,2, "image_Planet_Background_1 %4X", image_Planet_Background_1.tileset->numTile*32);
    VDP_PRINT_1(1,3, "image_Planet_Background_2 %4X", image_Planet_Background_2.tileset->numTile*32);
    VDP_PRINT_1(1,4, "image_Planet_Overlay      %4X", image_Planet_Overlay.tileset->numTile*32);

    u16 sum = image_Planet_Background_0.tileset->numTile*32
            + image_Planet_Background_1.tileset->numTile*32
            + image_Planet_Background_2.tileset->numTile*32
            + image_Planet_Overlay.tileset->numTile*32;

    VDP_PRINT_1(1, 6, "BG sum: %4X", sum);

    VDP_PRINT_1(1, 8, "sprite_Planet_Sprites %4X", sprite_Planet_Sprites.maxNumTile*32);

    sum += sprite_Planet_Sprites.maxNumTile*32;

    VDP_PRINT_1(1, 10, "Total sum: %4X", sum);

    while(1);
}

// -----------------------------------------------------------------------------

inline void debugInfoScene2()
{
    VDP_resetScreen();

    VDP_PRINT_1(1, 1, "image_Explosion_Background %4X", image_Explosion_Background.tileset->numTile*32);
    VDP_PRINT_1(1, 2, "image_Explosion_Overlay    %4X", image_Explosion_Overlay.tileset->numTile*32);

    u16 sum = image_Explosion_Background.tileset->numTile*32
            + image_Explosion_Overlay.tileset->numTile*32;

    VDP_PRINT_1(1, 4, "BG sum: %4X", sum);

    VDP_PRINT_1(1, 6, "sprite_Explosion_Sprites_0 %4X", sprite_Explosion_Sprites_0.maxNumTile*32);
    VDP_PRINT_1(1, 7, "sprite_Explosion_Sprites_1 %4X", sprite_Explosion_Sprites_1.maxNumTile*32);

    sum += sprite_Explosion_Sprites_0.maxNumTile*32;
    sum += sprite_Explosion_Sprites_1.maxNumTile*32;

    VDP_PRINT_1(1, 9, "Total sum: %4X", sum);

    while(1);
}
 
// -----------------------------------------------------------------------------

inline void debugInfoScene3()
{
    VDP_resetScreen();

    VDP_PRINT_1(1,1, "image_Hangar_Background %4X",image_Hangar_Background.tileset->numTile*32);
    VDP_PRINT_1(1,2, "image_Hangar_Overlay    %4X",image_Hangar_Overlay.tileset->numTile*32);

    u16 sum = image_Hangar_Background.tileset->numTile*32
            + image_Hangar_Overlay.tileset->numTile*32;

    VDP_PRINT_1(1, 4, "BG sum: %4X", sum);

    VDP_PRINT_1(1, 6, "sprite_Hangar_Sprites_0 %4X", 32*sprite_Hangar_Sprites_0.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1, 7, "sprite_Hangar_Sprites_1 %4X", 32*sprite_Hangar_Sprites_1.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1, 8, "sprite_Hangar_Sprites_2 %4X", 32*sprite_Hangar_Sprites_2.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1, 9, "sprite_Hangar_Sprites_3 %4X", 32*sprite_Hangar_Sprites_3.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,10, "sprite_Hangar_Sprites_4 %4X", 32*sprite_Hangar_Sprites_4.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,11, "sprite_Hangar_Sprites_5 %4X", 32*sprite_Hangar_Sprites_5.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,12, "sprite_Hangar_Sprites_6 %4X", 32*sprite_Hangar_Sprites_6.animations[0]->frames[0]->tileset->numTile);

    sum += 32*sprite_Hangar_Sprites_0.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_Sprites_1.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_Sprites_2.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_Sprites_3.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_Sprites_4.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_Sprites_5.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Hangar_Sprites_6.animations[0]->frames[0]->tileset->numTile;

    VDP_PRINT_1(1, 14, "Total sum: %4X", sum);

    while(1);
}

// -----------------------------------------------------------------------------

inline void debugInfoScene4()
{
    VDP_resetScreen();

    VDP_PRINT_1(1,1, "image_Title_Background   %4X",image_Title_Background.tileset->numTile*32);
    VDP_PRINT_1(1,2, "image_Title_Overlay      %4X",image_Title_Overlay.tileset->numTile*32);
    VDP_PRINT_1(1,3, "image_Title_ThunderForce %4X",image_Title_ThunderForce.tileset->numTile*32);

    u16 sum = image_Title_Background.tileset->numTile*32
            + image_Title_Overlay.tileset->numTile*32
            + image_Title_ThunderForce.tileset->numTile*32;

    VDP_PRINT_1(1, 5, "BG sum: %4X", sum);

    VDP_PRINT_1(1, 7, "sprite_Title_Sprites_0  %4X", 32*sprite_Title_Sprites_0 .animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1, 8, "sprite_Title_Sprites_1  %4X", 32*sprite_Title_Sprites_1 .animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1, 9, "sprite_Title_Sprites_2  %4X", 32*sprite_Title_Sprites_2 .animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,10, "sprite_Title_Sprites_3  %4X", 32*sprite_Title_Sprites_3 .animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,11, "sprite_Title_Sprites_4  %4X", 32*sprite_Title_Sprites_4 .animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,12, "sprite_Title_Sprites_5  %4X", 32*sprite_Title_Sprites_5 .animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,13, "sprite_Title_Sprites_6  %4X", 32*sprite_Title_Sprites_6 .animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,14, "sprite_Title_Sprites_7  %4X", 32*sprite_Title_Sprites_7 .animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,15, "sprite_Title_Sprites_8  %4X", 32*sprite_Title_Sprites_8 .animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,16, "sprite_Title_Sprites_9  %4X", 32*sprite_Title_Sprites_9 .animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,17, "sprite_Title_Sprites_10 %4X", 32*sprite_Title_Sprites_10.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,18, "sprite_Title_Sprites_11 %4X", 32*sprite_Title_Sprites_11.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,19, "sprite_Title_Sprites_12 %4X", 32*sprite_Title_Sprites_12.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,20, "sprite_Title_Sprites_13 %4X", 32*sprite_Title_Sprites_13.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,21, "sprite_Title_Sprites_14 %4X", 32*sprite_Title_Sprites_14.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,22, "sprite_Title_Sprites_15 %4X", 32*sprite_Title_Sprites_15.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,23, "sprite_Title_Sprites_16 %4X", 32*sprite_Title_Sprites_16.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,24, "sprite_Title_Sprites_17 %4X", 32*sprite_Title_Sprites_17.animations[0]->frames[0]->tileset->numTile);
    VDP_PRINT_1(1,25, "sprite_Title_Sprites_18 %4X", 32*sprite_Title_Sprites_18.animations[0]->frames[0]->tileset->numTile);

    sum += 32*sprite_Title_Sprites_0 .animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_1 .animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_2 .animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_3 .animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_4 .animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_5 .animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_6 .animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_7 .animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_8 .animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_9 .animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_10.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_11.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_12.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_13.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_14.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_15.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_16.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_17.animations[0]->frames[0]->tileset->numTile;
    sum += 32*sprite_Title_Sprites_18.animations[0]->frames[0]->tileset->numTile;

    VDP_PRINT_1(1, 27, "Total sum: %4X", sum);

    while(1);
}
