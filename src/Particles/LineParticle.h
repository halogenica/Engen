#pragma once

#include "System/Common.h"
#include "Particles/Particle.h"

namespace gen
{

class LineParticle: public Particle
{
public:
    LineParticle(const ci::Vec3f&     startPos,
                 const ci::Vec3f&     endPos);
    ~LineParticle();
    virtual void Draw();
    virtual void Update(float dt);

private:
    ci::Vec3f m_startPos;
    ci::Vec3f m_endPos;
    float m_aliveTime;
};
}
