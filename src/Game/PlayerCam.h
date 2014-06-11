#pragma once

#include "System/Common.h"

namespace gen
{

class PlayerCam
{
public:
    PlayerCam();
    PlayerCam(const ci::CameraPersp& cam);
    ~PlayerCam();
    virtual void Update(float dt);
    virtual void Reset();

    const ci::CameraPersp& GetCamera() const;
    void SetCurrentCam(const ci::CameraPersp& cam);

    float dx;
    float dy;

private:
    ci::CameraPersp m_camera;
    ci::CameraPersp m_resetCamera;
};

}
