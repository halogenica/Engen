#pragma once

#include "System/Common.h"

namespace gen
{

// SpriteSheet represents a complete set of sprites for one character.
// Setting a new sprite group resets the internal timer tracking sprite index.
class SpriteSheet
{
public:
    SpriteSheet(const ci::DataSourceRef& tex,
                const UINT numTilesWide,
                const UINT numTilesHigh);
     
    virtual ~SpriteSheet(){};

    // Functions
    ci::Vec2f GetTextureScale() const;
    ci::Vec2f GetTextureTranslate(const ci::Vec2i& range, float time, float secondsPerSprite) const;
    
    // Data
    ci::gl::Texture     m_tex;
    UINT                m_numTilesWide;
    UINT                m_numTilesHigh;
    UINT                m_tileWidth;
    UINT                m_tileHeight;
};
}
