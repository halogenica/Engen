#pragma once

#include "System/Common.h"

namespace gen
{
// forward declarations
class TileSet;
class SpriteSheet;

class RenderData
{
public:
    RenderData(const ci::DataSourceRef& mesh,
               const ci::ColorA&        color = ci::ColorA(1.0f, 1.0f, 1.0f, 1.0f),
               const ci::DataSourceRef& tex = ci::DataSourceRef());
    
    RenderData(const ci::DataSourceRef& mesh,
               SpriteSheet* spriteSheet,
               const UINT index = 0);

    RenderData(const ci::DataSourceRef& mesh,
               SpriteSheet* spriteSheet,
               const ci::Vec2i spriteRange,
               float secondsPerSprite = 1.0f,
               float spritePlayTime = 0.0f);

    virtual ~RenderData();

    // Functions
    void Draw(const ci::Vec3f& pos, const ci::Quatf& rot, const ci::Vec3f& scale);
    void Update(float dt);

    void SetSpriteRange(const ci::Vec2i& range);

    // Data
    ci::TriMesh         m_mesh;
    ci::ColorA          m_color;
    ci::gl::Texture     m_tex;

    SpriteSheet*        m_pSpriteSheet;
    ci::Vec2i           m_spriteRange;
    float               m_secondsPerSprite;
    float               m_spritePlayTime;

    // Custom data
    float               m_cellshadePercent;
};
}
