#include "System/ObjectManager.h"
#include "ObjectData/RenderData.h"
#include "ObjectData/SpriteSheet.h"
#include "Utils/Stats.h"

using namespace gen;
using namespace ci;

gen::RenderData::RenderData(const ci::DataSourceRef& mesh,
                            const ci::ColorA& color,
                            const ci::DataSourceRef& tex):
                                m_color(color),
                                m_cellshadePercent(0.0f),
                                m_pSpriteSheet(NULL),
                                m_spriteRange(0,0),
                                m_secondsPerSprite(1.0f),
                                m_spritePlayTime(0.0f)
{
    ci::ObjLoader loader(mesh);
    loader.load(&m_mesh);
    
    if (tex)    // implicitly shared resources default construct to NULL
    {
        m_tex = gl::Texture(ci::loadImage(tex));

        // disable bilinear filtering
        m_tex.setMinFilter(GL_NEAREST);
        m_tex.setMagFilter(GL_NEAREST);
    }
    
    g_pStats->AddVertices(m_mesh.getNumVertices());
    g_pStats->AddTriangles(m_mesh.getNumTriangles());
}

gen::RenderData::RenderData(const ci::DataSourceRef& mesh,
                            SpriteSheet* spriteSheet,
                            const UINT index):
                                m_tex(spriteSheet->m_tex),
                                m_color(ci::ColorA(1.0f, 1.0f, 1.0f, 1.0f)),
                                m_cellshadePercent(0.0f),
                                m_pSpriteSheet(spriteSheet),
                                m_spriteRange(index,index),
                                m_secondsPerSprite(1.0f),
                                m_spritePlayTime(0.0f)
{
    ci::ObjLoader loader(mesh);
    loader.load(&m_mesh);

    g_pStats->AddVertices(m_mesh.getNumVertices());
    g_pStats->AddTriangles(m_mesh.getNumTriangles());
}

gen::RenderData::RenderData(const ci::DataSourceRef& mesh,
                            SpriteSheet* spriteSheet,
                            const ci::Vec2i spriteRange,
                            float secondsPerSprite,
                            float spritePlayTime) :
                                m_tex(spriteSheet->m_tex),
                                m_color(ci::ColorA(1.0f, 1.0f, 1.0f, 1.0f)),
                                m_cellshadePercent(0.0f),
                                m_pSpriteSheet(spriteSheet),
                                m_spriteRange(spriteRange),
                                m_secondsPerSprite(secondsPerSprite),
                                m_spritePlayTime(spritePlayTime)
{    
    ci::ObjLoader loader(mesh);
    loader.load(&m_mesh);

    g_pStats->AddVertices(m_mesh.getNumVertices());
    g_pStats->AddTriangles(m_mesh.getNumTriangles());
}

gen::RenderData::~RenderData()
{
    g_pStats->SubVertices(m_mesh.getNumVertices());
    g_pStats->SubTriangles(m_mesh.getNumTriangles());
}

void gen::RenderData::Draw(const ci::Vec3f& pos, const ci::Quatf& rot, const ci::Vec3f& scale)
{
    if (m_pSpriteSheet)
    {
        ASSERT(m_tex);
        ASSERT(m_mesh.hasTexCoords());
        gl::pushMatrices();
        gl::color(1,1,1,1); // Add color to texture and clamp to (0,1). If texture present, set color to 0.

        glMatrixMode(GL_MODELVIEW);
        gl::translate(pos);
        gl::rotate(rot);
        gl::scale(scale);

        glEnable(GL_TEXTURE_2D);
        m_tex.enableAndBind();
        glMatrixMode(GL_TEXTURE);
        glPushMatrix();
        glLoadIdentity();
        gl::scale(m_pSpriteSheet->GetTextureScale());
        gl::translate(m_pSpriteSheet->GetTextureTranslate(m_spriteRange, m_spritePlayTime, m_secondsPerSprite));

        gl::draw(m_mesh);

        glMatrixMode(GL_TEXTURE);
        glPopMatrix();
        m_tex.disable();
        m_tex.unbind();
        glMatrixMode(GL_MODELVIEW);

        gl::popMatrices();
    }
    else
    {
        gl::pushMatrices();
        ci::Matrix44f modelView;
        gl::multModelView(modelView); // TODO: use this

        if (m_cellshadePercent > 0)
        {
            gl::pushMatrices();
            glCullFace(GL_FRONT);
            gl::color(0,0,0,1);
            gl::translate(pos);
            gl::rotate(rot);
            gl::scale(scale);

            gl::draw(m_mesh);
            gl::popMatrices();
        }

        glCullFace(GL_BACK);
        gl::color(m_color);
        gl::translate(pos);
        gl::rotate(rot);
        gl::scale(scale * Vec3f(1-m_cellshadePercent, 1-m_cellshadePercent, 1-m_cellshadePercent));

        if (m_tex && m_mesh.hasTexCoords())
        {
            gl::color(1,1,1,1);
            glEnable(GL_TEXTURE_2D);
            m_tex.enableAndBind();
        }

        gl::draw(m_mesh);

        if (m_tex && m_mesh.hasTexCoords())
        {
            m_tex.disable();
            m_tex.unbind();
        }

        gl::popMatrices();
    }
}

void gen::RenderData::Update(float dt)
{
    if (m_pSpriteSheet)
    {
        m_spritePlayTime += dt;
        while (m_spritePlayTime > (m_secondsPerSprite * (m_spriteRange.y  - m_spriteRange.x + 1)))
        {
            m_spritePlayTime -= (m_secondsPerSprite * (m_spriteRange.y  - m_spriteRange.x + 1));
        }
    }
}


void gen::RenderData::SetSpriteRange(const ci::Vec2i& range)
{
    m_spritePlayTime = 0.0f;
    m_spriteRange = range;
}