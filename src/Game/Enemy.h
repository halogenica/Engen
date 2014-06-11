#pragma once

#include "System/Common.h"
#include "ObjectData/ObjectData.h"

namespace gen
{

class Enemy : public ObjectData
{
public:
    enum TYPE
    {
        BLACK   = 0,
        RED     = 1,
    };

    Enemy(const TYPE        type,
          const ci::Vec3f   pos,
          const ci::Quatf   rot,
          const ci::Vec3f   scale,
          OpenAL::Sound*    pSfx);

    virtual void Update(float dt);

    virtual void Collision(ObjectData* pCollider);

private:
    const TYPE      m_type;
    OpenAL::Sound*  m_pSfx;
    float           m_flashTime;
};
}
