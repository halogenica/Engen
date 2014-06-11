#pragma once

#include "System/Common.h"

namespace gen
{

class Particle
{
public:
    Particle():m_alive(true){};
    virtual ~Particle(){};
    virtual void Draw() = 0;
    virtual void Update(float dt) = 0;

    bool m_alive;
};
}
