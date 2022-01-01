
#include <genesis.h>

void print(u16 x, u16 y, char* fmt)
{
    char buffer[128];
    sprintf(buffer, fmt);
    VDP_drawText(buffer, x, y);
}

void fadePalettes(const u16* pal0, const u16* pal1, const u16* pal2, const u16* pal3, u16 numFrames)
{
    u16 palTemp[64];
    if (pal0) memcpy(palTemp+ 0, pal0, 16*2);
    if (pal1) memcpy(palTemp+16, pal1, 16*2);
    if (pal2) memcpy(palTemp+32, pal2, 16*2);
    if (pal3) memcpy(palTemp+48, pal3, 16*2);

    PAL_fadeToAll(palTemp, numFrames, FALSE);
}

int main()
{
    PAL_setColors(0, palette_black, 64, DMA);

    for (u16 i=0; i<4; i++)
    {
        VDP_setTextPalette(i);
        print(1, i+1, "0 2 4 6 8 A C E");
    }

    while (TRUE)
    {
        fadePalettes(
            palette_grey,
            palette_red,
            palette_green,
            palette_blue,
            48
        );

        waitMs(1000);

        fadePalettes(
            palette_black,
            palette_black,
            palette_black,
            palette_black,
            48
        );
    }
}
