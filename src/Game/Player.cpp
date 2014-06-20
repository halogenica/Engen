#include "Game/Player.h"
#include "ObjectData/RenderData.h"
#include "ObjectData/PhysicsData.h"
#include "System/ObjectManager.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

Player::Player(const Vec3f pos, OpenAL::Sound* pSfx) :
                   ObjectData(new RenderData(loadResource(RES_SPHERE_OBJ), ColorAf(1.f, 0.f, 0.f, 1.f)),
                              NULL,
                              pos, Quatf(0.f,0.f,0.f), Vec3f(1.f,1.f,1.f)),
                   m_origPos(pos),
                   m_pSfx(pSfx)
{
    m_pRenderData->m_cellshadePercent = 0.2f;

    for (int i = 0; i < 20; i++)
    {
        Bullet* pBullet =   new Bullet(new RenderData(loadResource(RES_SPHERE_OBJ), ColorAf(1.f, 0.f, 0.f, 1.f)),
                                       new PhysicsData(new btSphereShape(0.25f), 1, COL_BULLET, COL_EVERYTHING),
                                       GetPos(), Quatf(0.f,0.f,0.f), Vec3f(0.5f,0.5f,0.5f));
        pBullet->m_pRenderData->m_cellshadePercent = 0.2f;
        m_bullets.push_back(pBullet);
    }
}

Player::~Player()
{
    for (auto pBullet : m_bullets)
    {
        delete(pBullet);
    }
}

void Player::Draw()
{
    ObjectData::Draw();
    for (auto pBullet : m_activeBullets)
    {
        pBullet->Draw();
    }
}

void Player::Update(float dt)
{
    ObjectData::Update(dt);
    for (auto pBullet : m_activeBullets)
    {
        pBullet->Update(dt);
    }
}

void Player::Shoot()
{
    Bullet* pBullet;
    if (m_activeBullets.size() >= m_bullets.size())
    {
        // If all bullets are active, recycle least recently shoot ones
        pBullet = m_activeBullets.front();
        m_activeBullets.pop_front();
        m_activeBullets.push_back(pBullet);
    }
    else
    {
        // If not all bullets are active, get one from the available ones
        pBullet = m_bullets.front();
        m_bullets.pop_front();
        m_bullets.push_back(pBullet);
        m_activeBullets.push_back(pBullet);
    }

    pBullet->SetPos(m_pos);
    pBullet->m_pPhysicsData->m_pBtRigidBody->applyImpulse(btVector3(0.f, 0.f, -50.f), btVector3(0.f, 0.f, 0.f));
    pBullet->m_sfxPlayed = false;

    m_pSfx->Play();
}

void Player::Reset()
{
    m_activeBullets.clear();
    SetPos(m_origPos);
}