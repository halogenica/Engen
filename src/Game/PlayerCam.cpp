#include "Game/PlayerCam.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

PlayerCam::PlayerCam() : dx(0), dy(0)
{
    m_camera = CameraPersp();
    m_resetCamera = m_camera;
}

PlayerCam::PlayerCam(const CameraPersp& cam) : dx(0), dy(0)
{
    m_camera = cam;
    m_resetCamera = m_camera;
}


PlayerCam::~PlayerCam()
{
}

void PlayerCam::Update(float dt)
{
    float deltaX = dx*dt;
    float deltaY = dy*dt;
    Vec3f mW = m_camera.getViewDirection().normalized();
    bool invertMotion = (m_camera.getOrientation() * Vec3f::yAxis()).y < 0.0f;
    Vec3f mU = Vec3f::yAxis().cross(mW).normalized();

    if (invertMotion)
    {
        deltaX = -deltaX;
        deltaY = -deltaY;
    }
            
    Vec3f rotatedVec = Quatf(mU, deltaY) * (m_camera.getEyePoint() - m_camera.getCenterOfInterestPoint());
    rotatedVec = Quatf(Vec3f::yAxis(), deltaX) * rotatedVec;
    
    m_camera.setEyePoint(m_camera.getCenterOfInterestPoint() + rotatedVec);
    m_camera.setOrientation(m_camera.getOrientation() * Quatf(mU, deltaY) * Quatf(Vec3f::yAxis(), deltaX));
}

void PlayerCam::Reset()
{
    m_camera = m_resetCamera;
    dx = 0;
    dy = 0;
}

const CameraPersp& PlayerCam::GetCamera() const 
{ 
    return m_camera; 
}

void PlayerCam::SetCurrentCam( const CameraPersp& cam )
{ 
    m_camera = cam; 
    m_resetCamera = m_camera;
}
