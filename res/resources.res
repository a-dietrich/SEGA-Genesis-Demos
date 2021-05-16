
//
// Scene 1
//

IMAGE   image_Planet_Layer0_0       "gfx/Planet_Layer0-0.png" NONE DUPLICATE
IMAGE   image_Planet_Layer0_1       "gfx/Planet_Layer0-1.png" NONE DUPLICATE
IMAGE   image_Planet_Layer0_2       "gfx/Planet_Layer0-2.png" NONE DUPLICATE

IMAGE   image_Planet_L              "gfx/Planet_120x240_16_L_512.png" NONE DUPLICATE
IMAGE   image_Planet_M              "gfx/Planet_128x240_16_M_512.png" NONE DUPLICATE
IMAGE   image_Planet_R              "gfx/Planet_072x240_16_R_512.png" NONE DUPLICATE

IMAGE   image_Overlay         "gfx/Overlay_16.png"        NONE DUPLICATE


IMAGE   Test_R              "gfx/Untitled.png" NONE NONE

SPRITE  sprite_Planet_M             "gfx/Planet_128x128_16_M_512.png" 16 16 NONE 0 NONE TILE
PALETTE palette_Planet_M            "gfx/Planet_128x128_16_M_512.png"

//
// Scene 2
//

IMAGE   image_Scene2_0      "gfx/Explosion_Layer0.png" NONE DUPLICATE
IMAGE   image_Scene2_1      "gfx/Explosion_Layer1.png" NONE NONE

SPRITE  sprite_Scene2_0            "gfx/Explosion_Layer1.0.png" 16 17 NONE 0 NONE NONE
SPRITE  sprite_Scene2_1            "gfx/Explosion_Layer1.1.png" 15 13 NONE 0 NONE NONE
SPRITE  sprite_Scene2_2            "gfx/Explosion_Layer1.2.png"  6  8 NONE 0 NONE NONE

IMAGE   image_Scene2_Overlay       "gfx/Explosion_Overlay_16.png"  NONE DUPLICATE

PALETTE palette_Scene2_0           "gfx/Explosion_Layer0.png"
PALETTE palette_Scene2_1           "gfx/Explosion_Layer1.0.png"
PALETTE palette_Scene2_2           "gfx/Explosion_Overlay_16.png"

//
// Scene 3
//

IMAGE   image_Scene3_0      "gfx/Hangar_Layer0.png" NONE ALL
IMAGE   image_Scene3_1      "gfx/Hangar_Layer1.png" NONE ALL
IMAGE   image_Scene3_2      "gfx/Hangar_Layer2.png" NONE ALL

IMAGE   image_Overlay_2       "gfx/Hangar_Overlay_16.png" NONE ALL

SPRITE  sprite_Hangar_0            "gfx/Hangar_Sprites_1_0.png" 9 21   NONE 0 NONE TILE
SPRITE  sprite_Hangar_1            "gfx/Hangar_Sprites_1_1.png" 5 8    NONE 0 NONE TILE
SPRITE  sprite_Hangar_2            "gfx/Hangar_Sprites_1_2.png" 5 2    NONE 0 NONE TILE
SPRITE  sprite_Hangar_3            "gfx/Hangar_Sprites_1_3.png" 13 7   NONE 0 NONE TILE

SPRITE  sprite_Hangar_4            "gfx/Hangar_Sprites_2_0.png" 4 7    NONE 0 NONE TILE
SPRITE  sprite_Hangar_5            "gfx/Hangar_Sprites_2_1.png" 3 2    NONE 0 NONE TILE
SPRITE  sprite_Hangar_6            "gfx/Hangar_Sprites_2_2.png" 11 11  NONE 0 NONE TILE

PALETTE palette_Hangar_0           "gfx/Hangar_Layer0.png"
PALETTE palette_Hangar_1           "gfx/Hangar_Sprites_1_0.png"
PALETTE palette_Hangar_2           "gfx/Hangar_Sprites_2_0.png"
PALETTE palette_Hangar_3           "gfx/Hangar_Overlay_16.png"

XGM     xgm_01                      "music/01_-_The_Wind_Blew_All_Day_Long_(OpeningTheme).vgm"
