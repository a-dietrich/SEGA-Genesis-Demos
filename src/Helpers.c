
// *****************************************************************************
// *****************************************************************************

// Genesis Utils
#include "Helpers.h"


// *****************************************************************************
//
//  Variables
//
// *****************************************************************************

static u16       m_vdpSpriteIndexNext;

// *****************************************************************************
//
//  Functions
//
// *****************************************************************************

// -----------------------------------------------------------------------------
//  Sprite functions
// -----------------------------------------------------------------------------

void GUT_resetMetaSprites()
{
    m_vdpSpriteIndexNext = 0;

    VDP_resetSprites();
}

// -----------------------------------------------------------------------------

u16 GUT_getNextMetaSpritesIndex()
{
    return m_vdpSpriteIndexNext;
}

// -----------------------------------------------------------------------------

MetaSprite GUT_addMetaSprite(const SpriteDefinition *spriteDef, u16 animation, u16 frame, u16 pal, u16 tileIndex)
{
    MetaSprite sprite = { 320, 0, 0, 0, 0, tileIndex, tileIndex };
#if 0
    AnimationFrame *animationFrame = spriteDef->animations[animation]->frames[frame];

    const u32 *tiles   = animationFrame->tileset->tiles;
    const u16  numTile = animationFrame->tileset->numTile;

    VDP_loadTileData(tiles, tileIndex, numTile, DMA);

    const s16 w = animationFrame->w / 8;
    const s16 h = animationFrame->h / 8;

    char buf[40];
    sprintf(buf, "w       : %d", w);
    VDP_drawText(buf, 1, 20);
    sprintf(buf, "h       : %d", h);
    VDP_drawText(buf, 1, 21);

    u8 size = animationFrame->frameInfos[0].frameVDPSprites[0]->size;
    sprintf(buf, "size    : %02x", size);
    VDP_drawText(buf, 1, 22);
    u8 nt = animationFrame->frameInfos[0].frameVDPSprites[0]->numTile;
    sprintf(buf, "numTile : %02d", nt);
    VDP_drawText(buf, 1, 23);
    u8 oX = animationFrame->frameInfos[0].frameVDPSprites[0]->offsetX;
    sprintf(buf, "offsetX : %02d", oX);
    VDP_drawText(buf, 1, 24);
    u8 oY = animationFrame->frameInfos[0].frameVDPSprites[0]->offsetY;
    sprintf(buf, "offsetY : %02d", oY);
    VDP_drawText(buf, 1, 25);

    sprite.vdpSpriteIndex = m_vdpSpriteIndexNext;

    for (s16 x=w, xt; x>0; x-=xt)
    {
        xt = (x > 3) ? 4 : x;
        for (s16 y=h, yt; y>0; y-=yt)
        {
            yt = (y > 3) ? 4 : y;
            VDP_setSpriteFull( m_vdpSpriteIndexNext,
                               320, 0,
                               SPRITE_SIZE(xt, yt),
                               TILE_ATTR_FULL(pal, FALSE, FALSE, FALSE, sprite.vdpTileIndexEnd),
                               m_vdpSpriteIndexNext+1 );

            sprite.vdpTileIndexEnd += xt*yt;
            m_vdpSpriteIndexNext++;

            if (sprite.columns == 0)
                sprite.rows++;
        }

        sprite.columns++;
    }

    if (sprite.vdpSpriteIndex > 0)
        VDP_setSpriteLink(sprite.vdpSpriteIndex-1, sprite.vdpSpriteIndex);

    VDP_setSpriteLink(m_vdpSpriteIndexNext-1, 0);
    VDP_updateSprites(m_vdpSpriteIndexNext, TRUE);
#endif
    return sprite;
}

// -----------------------------------------------------------------------------

MetaSprite GUT_addMetaSpriteDirect(u16 w, u16 h, u16 pal, u16 tileIndex)
{
    MetaSprite sprite = { 320, 0, 0, 0, 0, tileIndex, tileIndex };

    sprite.vdpSpriteIndex = m_vdpSpriteIndexNext;

    for (s16 y=h, yt; y>0; y-=yt)
    {
        yt = (y > 3) ? 4 : y;
        for (s16 x=w, xt; x>0; x-=xt)
        {
            xt = (x > 3) ? 4 : x;
            VDP_setSpriteFull( m_vdpSpriteIndexNext,
                               320, 0,
                               SPRITE_SIZE(xt, yt),
                               TILE_ATTR_FULL(pal, TRUE, FALSE, FALSE, sprite.vdpTileIndexEnd),
                               m_vdpSpriteIndexNext+1 );

            sprite.vdpTileIndexEnd += xt*yt;
            m_vdpSpriteIndexNext++;

            if (sprite.rows == 0)
                sprite.columns++;
        }

        sprite.rows++;
    }

    if (sprite.vdpSpriteIndex > 0)
        VDP_setSpriteLink(sprite.vdpSpriteIndex-1, sprite.vdpSpriteIndex);

    VDP_setSpriteLink(m_vdpSpriteIndexNext-1, 0);
    VDP_updateSprites(m_vdpSpriteIndexNext, TRUE);

    return sprite;
}

// -----------------------------------------------------------------------------

void GUT_setMetaSpritePosition(MetaSprite *metaSprite, s16 x, s16 y)
{
    metaSprite->x = x;
    metaSprite->y = y;

    u16 spriteIndex = metaSprite->vdpSpriteIndex;

    for (u16 j=0; j<metaSprite->columns; j++)
        for (u16 i=0; i<metaSprite->rows; i++)
        {
            const s16 xp = CLAMP(x + j*32, -32, 320);
            const s16 yp = CLAMP(y + i*32, -32, 224);

            VDP_setSpritePosition(spriteIndex++, xp, yp);
        }
}

// -----------------------------------------------------------------------------

void GUT_updateMetaSprites(u16 queue)
{
    VDP_updateSprites(m_vdpSpriteIndexNext, queue);
}
