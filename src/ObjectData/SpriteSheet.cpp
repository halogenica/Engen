#include "ObjectData/SpriteSheet.h"

using namespace gen;
using namespace ci;

SpriteSheet::SpriteSheet(const ci::DataSourceRef& tex,
                         const UINT numTilesWide,
                         const UINT numTilesHigh) :
                             m_numTilesWide(numTilesWide),
                             m_numTilesHigh(numTilesHigh)
{
    m_tex = gl::Texture(ci::loadImage(tex));

    m_tileWidth = m_tex.getWidth() / m_numTilesWide;
    ASSERT(m_tileWidth * m_numTilesWide == m_tex.getWidth());

    m_tileHeight = m_tex.getHeight() / m_numTilesHigh;
    ASSERT(m_tileHeight * m_numTilesHigh == m_tex.getHeight());

    // disable bilinear filtering
    m_tex.setMinFilter(GL_NEAREST);
    m_tex.setMagFilter(GL_NEAREST);
}

ci::Vec2f SpriteSheet::GetTextureScale() const
{
    return Vec2f(m_tileWidth/(float)m_tex.getWidth(), m_tileHeight/(float)m_tex.getHeight());
}
/*
ci::Vec2f SpriteSheet::GetTextureTranslate(const UINT index) const
{
    ASSERT(range.x < (m_numTilesWide * m_numTilesHigh));
    ASSERT(range.y >= 0);
    return Vec2f(index%m_numTilesWide, index/m_numTilesWide);
}
*/
ci::Vec2f SpriteSheet::GetTextureTranslate(const ci::Vec2i& range, float time, float secondsPerSprite) const
{
    ASSERT(range.x >= 0);
    ASSERT(range.x < (m_numTilesWide * m_numTilesHigh));
    ASSERT(range.y >= 0);
    ASSERT(range.y < (m_numTilesWide * m_numTilesHigh));
    ASSERT(range.x <= range.y);

    UINT index = time/secondsPerSprite;
    index += range.x;
    
    ASSERT(index <= range.y);

    return Vec2f(index%m_numTilesWide, index/m_numTilesWide);
}