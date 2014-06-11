#pragma once

#include "System/Common.h"
#include "Particles/Particle.h"

namespace gen
{

class SquareParticle: public Particle
{
public:
    SquareParticle();
    ~SquareParticle();
    virtual void Draw();
    virtual void Update(float dt);
    virtual void Reset();

    ci::Vec3f   m_pos;
    ci::Quatf   m_rot;
    float       m_rotSpeed;
    float       m_scale;
    ci::ColorAf m_color;
    float       m_aliveTime;
    float       m_timeToLive;
};
}
