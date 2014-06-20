#include "Game/Enemy.h"
#include "Game/Player.h"
#include "ObjectData/RenderData.h"
#include "ObjectData/PhysicsData.h"
#include "System/ObjectManager.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

Enemy::Enemy(const TYPE         type,
             const Vec3f    pos,
             const Quatf    rot,
             const Vec3f    scale,
             OpenAL::Sound*     pSfx) :
                 ObjectData(new RenderData(loadResource(RES_CUBE_OBJ), ColorAf(1.f*type, 0.f, 0.f, 1.f)),
                            new PhysicsData(new btBoxShape(btVector3(0.5f*scale.x, 0.5f*scale.y, 0.5f*scale.z)), 5, COL_ENEMY, COL_EVERYTHING),
                            pos, rot, scale),
                 m_type(type),
                 m_pSfx(pSfx)
{
    m_pRenderData->m_cellshadePercent = 0.2f;
}

void Enemy::Update(float dt)
{
    ObjectData::Update(dt);

    m_flashTime -= dt;

    if (m_type == BLACK)
    {
        m_pRenderData->m_color.r = m_flashTime * 5;
    }
    m_pRenderData->m_color.g = m_flashTime * 5;
    m_pRenderData->m_color.b = m_flashTime * 5;
}

void Enemy::Collision(ObjectData* pCollider)
{
    if (pCollider->m_pPhysicsData->GetCollisionGroup() & COL_BULLET)
    {
        m_flashTime = 0.2;

        Bullet* pBullet = reinterpret_cast<Bullet*>(pCollider);
        if (!pBullet->m_sfxPlayed)
        {
            pBullet->m_sfxPlayed = true;
            m_pSfx->Play(false);
        }

        if (m_type == RED && g_pObjectManager->m_gameState == PLAYING) 
        {
            g_pObjectManager->GameOver();
        }
    }
}
