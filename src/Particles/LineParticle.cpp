#include "Particles/LineParticle.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

LineParticle::LineParticle(const ci::Vec3f&     startPos,
                           const ci::Vec3f&     endPos) :
                               Particle(),
                               m_startPos(startPos),
                               m_endPos(endPos),
                               m_aliveTime(0)
{

}

LineParticle::~LineParticle()
{

}

void LineParticle::Draw()
{
    float timeToLive = 0.1f;
    if (m_aliveTime < timeToLive)
    {
        gl::color(ColorAf(0,0,0,(timeToLive-m_aliveTime)/timeToLive));
        gl::lineWidth(3.0);
        gl::drawLine(m_startPos, m_endPos);
    }
    else
    {
        m_alive = false;
    }
}

void LineParticle::Update(float dt)
{
    m_aliveTime+=dt;

    Vec3f dir = m_endPos - m_startPos;
    if (dir.length() <= 0.005)
    {
        m_alive = false;
    }
    else
    {
        dir.normalize();
        m_startPos += dir*0.02;
        m_endPos += dir*0.01;
    }
}
