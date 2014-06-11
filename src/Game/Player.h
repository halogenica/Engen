#pragma once

#include "System/Common.h"
#include "ObjectData/ObjectData.h"

namespace gen
{

class Bullet : public ObjectData
{
public:
    Bullet(RenderData*          pRenderData,
           PhysicsData*         pPhysicsData,
           const ci::Vec3f&     pos,
           const ci::Quatf&     rot,
           const ci::Vec3f&     scale) : 
               ObjectData(pRenderData, pPhysicsData, pos, rot, scale),
               m_sfxPlayed(false)
    {}

    bool m_sfxPlayed;   // prevent collision noise from playing more than once
};

class Player : public ObjectData
{
public:
    Player(const ci::Vec3f pos, OpenAL::Sound* pSfx);
    ~Player();
    void Draw();
    void Update(float dt);
    void Shoot();
    void Reset();

private:
    std::deque<Bullet*> m_bullets;
    std::deque<Bullet*> m_activeBullets;
    OpenAL::Sound*  m_pSfx;
    const ci::Vec3f m_origPos;
};
}
