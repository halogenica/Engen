#pragma once

#include "System/Common.h"

namespace gen
{
    
class ObjectData;   // Forward declare ObjectData
    
class PhysicsData
{

friend class ObjectData;    // ObjectData will call private function Initialize()

public:
    // Constructors and Destructors
    PhysicsData(btCollisionShape* pBtCollisionShape, btScalar mass, short collisionGroup = 0, short collisionMask = 0);
    ~PhysicsData();
    
    // Functions
    void RegisterPhysics();
    void UnregisterPhysics();
    void SetCollisionMask(short mask);
    short GetCollisionMask();
    short GetCollisionGroup();
    btTransform GetTransform();

    btCollisionObject* GetCollisionObject();

    // Public Data
    btCollisionShape*   m_pBtCollisionShape;
    btRigidBody*        m_pBtRigidBody;

    // Static functions
    static bool CollisionCallback(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1);

protected:

    PhysicsData():m_pBtCollisionShape(NULL),m_mass(0),m_collisionGroup(0),m_collisionMask(0){};

private:

    void Initialize(const btVector3& pos, const btQuaternion& rot, gen::ObjectData* pObjData);  // Called by ObjectData during constructor
    void ResetPhysics(const btVector3& pos, const btQuaternion& rot);

    // Private data because changing them requires resetting the physics object
    btScalar                m_mass;
    short                   m_collisionGroup;
    short                   m_collisionMask;
    btDefaultMotionState*   m_pBtMotionState;
};

inline void PhysicsData::RegisterPhysics()
{
    m_pBtRigidBody->setActivationState(DISABLE_DEACTIVATION);
    g_pBtDynamicsWorld->addRigidBody(m_pBtRigidBody, m_collisionGroup, m_collisionMask);
}

inline void PhysicsData::UnregisterPhysics()
{
    g_pBtDynamicsWorld->removeRigidBody(m_pBtRigidBody);
}

inline void PhysicsData::SetCollisionMask(short mask)
{
    g_pBtDynamicsWorld->removeRigidBody(m_pBtRigidBody);
    g_pBtDynamicsWorld->addRigidBody(m_pBtRigidBody, m_collisionGroup, mask);
}

inline short PhysicsData::GetCollisionMask()
{
    return m_collisionMask;
}

inline short PhysicsData::GetCollisionGroup()
{
    return m_collisionGroup;
}

inline btCollisionObject* PhysicsData::GetCollisionObject()
{
    return (btCollisionObject*)m_pBtRigidBody;
}

inline btTransform PhysicsData::GetTransform()
{
    btTransform btTrans;
    m_pBtMotionState->getWorldTransform(btTrans);
    return btTrans;
}
    
}
