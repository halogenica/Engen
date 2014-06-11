#include "Particles/SquareParticle.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

SquareParticle::SquareParticle() : 
                    Particle(),
                    m_pos(Vec3f(0,0,0)),
                    m_rot(Quatf(0,0,0)),
                    m_rotSpeed(0.0f),
                    m_scale(0.0f),
                    m_color(ColorAf(0,0,0,1)),
                    m_timeToLive(0.0f),
                    m_aliveTime(0)

{
    Reset();
}

SquareParticle::~SquareParticle()
{

}

void SquareParticle::Draw()
{
    ColorAf color = m_color;
    color.a = color.a*sin(m_aliveTime/m_timeToLive*PI);

    gl::pushMatrices();
    gl::color(color);
    gl::translate(m_pos);
    gl::rotate(m_rot);
    gl::scale(Vec3f(m_scale, m_scale, m_scale));
    gl::drawSolidRect(Rectf(-1, 1, 1, -1));
//    gl::drawSolidRect(Rectf(m_pos.x - m_scale*0.5, m_pos.y + m_scale*0.5, m_pos.x + m_scale*0.5, m_pos.y - m_scale*0.5));
    gl::popMatrices();
}

void SquareParticle::Update(float dt)
{
    m_aliveTime+=dt;

    if (m_aliveTime > m_timeToLive)
    {
        Reset();
    }

    m_rot *= Quatf(0, 0, m_rotSpeed);
}

void SquareParticle::Reset()
{
    m_pos = Vec3f((rand()%20000-10000)/200.f, (rand()%20000-10000)/400.f, -3-(rand()%10000/1000.f));
    m_scale = rand()%100/800.f;
    m_color = ColorAf(0.2, 0.2, 0.2, rand()%255/255.f);
    m_rot = Quatf(0, 0, (rand()%5000)/5000.f*PI);
    m_rotSpeed = rand()%100/50000.f;
    m_timeToLive = (rand()%150 + 50)/200.f*5;
    m_aliveTime = 0;
}
