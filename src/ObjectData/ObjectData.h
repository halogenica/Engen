#pragma once

#include "System/Common.h"

namespace gen
{

class RenderData;
class PhysicsData;

class ObjectData
{
public:    
    ObjectData(RenderData*          pRenderData,
               PhysicsData*         pPhysicsData,
               const ci::Vec3f&     pos,
               const ci::Quatf&     rot,
               const ci::Vec3f&     scale);

    virtual ~ObjectData();

    virtual void Draw();
    virtual void Update(float dt);
    virtual void Collision(ObjectData* pCollider){}

    ci::Vec3f GetPos();
    ci::Quatf GetRot();
    ci::Vec3f GetScale();
    void SetPos(ci::Vec3f pos);
    void SetRot(ci::Quatf rot);
    void SetScale(ci::Vec3f scale);

    RenderData*     m_pRenderData;
    PhysicsData*    m_pPhysicsData;

protected:
    // These variables are overwritten by valid physics data on each Update call
    ci::Vec3f       m_pos;
    ci::Quatf       m_rot;
    ci::Vec3f       m_scale;
};

inline ci::Vec3f ObjectData::GetPos()
{
    return m_pos;
}
inline ci::Quatf ObjectData::GetRot()
{
    return m_rot;
}
inline ci::Vec3f ObjectData::GetScale()
{
    return m_scale;
}

}
