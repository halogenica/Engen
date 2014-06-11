#include "ObjectData/PhysicsData.h"
#include "ObjectData/ObjectData.h"
#include "Utils/Stats.h"

using namespace gen;


void PhysicsData::Initialize(const btVector3& pos, const btQuaternion& rot, ObjectData* pObjectData)
{
    m_pBtMotionState = new btDefaultMotionState(btTransform(rot, pos));
    btVector3 initInertia(0,0,0);
    m_pBtCollisionShape->calculateLocalInertia(m_mass, initInertia);

    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(m_mass,
                                                         m_pBtMotionState,
                                                         m_pBtCollisionShape,
                                                         initInertia);

    m_pBtRigidBody = new btRigidBody(rigidBodyCI);
    m_pBtRigidBody->setUserPointer(reinterpret_cast<void*>(pObjectData)); // Used for collision callbacks.
    m_pBtRigidBody->setActivationState(DISABLE_DEACTIVATION);
    g_pBtDynamicsWorld->addRigidBody(m_pBtRigidBody, m_collisionGroup, m_collisionMask);
}

PhysicsData::PhysicsData(btCollisionShape* pBtCollisionShape, btScalar mass, short collisionGroup, short collisionMask) :
    m_pBtCollisionShape(pBtCollisionShape), m_mass(mass), m_collisionGroup(collisionGroup), m_collisionMask(collisionMask)
{
    g_pStats->AddRigidBodies(1);
}

PhysicsData::~PhysicsData()
{
    g_pStats->SubRigidBodies(1);
    g_pBtDynamicsWorld->removeRigidBody(m_pBtRigidBody);
    delete m_pBtMotionState;
    delete m_pBtRigidBody;
    delete m_pBtCollisionShape;
}

void PhysicsData::ResetPhysics(const btVector3& pos, const btQuaternion& rot)
{
    UnregisterPhysics();

    btTransform t;
    t.setRotation(rot);
    t.setOrigin(pos);

    m_pBtRigidBody->setLinearVelocity(btVector3(0, 0, 0));
    m_pBtRigidBody->setAngularVelocity(btVector3(0, 0, 0));
    m_pBtRigidBody->setCenterOfMassTransform(t);

    m_pBtMotionState->setWorldTransform(btTransform(rot, pos));

    RegisterPhysics();

    m_pBtRigidBody->clearForces();
    m_pBtRigidBody->activate();
}

bool PhysicsData::CollisionCallback(btManifoldPoint& point, btCollisionObject* body0, btCollisionObject* body1)
{
    ObjectData* pObjA = static_cast<ObjectData*>(body0->getUserPointer());
    ObjectData* pObjB = static_cast<ObjectData*>(body1->getUserPointer());

    pObjA->Collision(pObjB);
    pObjB->Collision(pObjA);

    return true;
}
