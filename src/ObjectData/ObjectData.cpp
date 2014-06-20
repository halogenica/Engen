#include "ObjectData/ObjectData.h"
#include "ObjectData/RenderData.h"
#include "ObjectData/PhysicsData.h"
#include "Utils/Stats.h"

using namespace gen;
using namespace ci;

gen::ObjectData::ObjectData(RenderData*          pRenderData,   // pRenderData can be NULL
                            PhysicsData*         pPhysicsData,  // pPhysicsData can be NULL
                            const Vec3f&     pos,
                            const Quatf&     rot,
                            const Vec3f&     scale) :
                                m_pRenderData(pRenderData),
                                m_pPhysicsData(pPhysicsData),
                                m_pos(pos),
                                m_rot(rot),
                                m_scale(scale)
{
    if (m_pPhysicsData)
    {
        m_pPhysicsData->Initialize(btVector3(m_pos.x, m_pos.y, m_pos.z), btQuaternion(m_rot.v.x, m_rot.v.y, m_rot.v.z, m_rot.w), this);
    }
}

gen::ObjectData::~ObjectData()
{
    if (m_pRenderData)
    {
        delete m_pRenderData;
    }
    if (m_pPhysicsData)
    {
        delete m_pPhysicsData;
    }
}

void gen::ObjectData::Update(float dt)
{
    if (m_pRenderData)
    {
        m_pRenderData->Update(dt);
    }
    if (m_pPhysicsData)
    {
        btTransform btTrans;
        m_pPhysicsData->m_pBtMotionState->getWorldTransform(btTrans);

        btVector3 pos = btTrans.getOrigin();
        btQuaternion rot = btTrans.getRotation();

        // Overwrite this ObjectData's transform data with those from the physics engine
        m_pos = Vec3f(pos.x(), pos.y(), pos.z());
        m_rot = Quatf(rot.w(), rot.x(), rot.y(), rot.z());
    }
}

void gen::ObjectData::Draw()
{
    if (m_pRenderData)
    {
        m_pRenderData->Draw(m_pos, m_rot, m_scale);
    }
}

void ObjectData::SetPos(Vec3f pos)
{
    m_pos = pos;
    if (m_pPhysicsData)
    {
        m_pPhysicsData->ResetPhysics(btVector3(m_pos.x, m_pos.y, m_pos.z), btQuaternion(m_rot.v.x, m_rot.v.y, m_rot.v.z, m_rot.w));
    }
}

void ObjectData::SetRot(Quatf rot)
{
    m_rot = rot;
    if (m_pPhysicsData)
    {
        m_pPhysicsData->ResetPhysics(btVector3(m_pos.x, m_pos.y, m_pos.z), btQuaternion(m_rot.v.x, m_rot.v.y, m_rot.v.z, m_rot.w));
    }
}

void ObjectData::SetScale(Vec3f scale)
{
//    ASSERT(!"Not Implemented!");
    m_scale = scale;
}
