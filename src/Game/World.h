#pragma once

#include "System/Common.h"

namespace gen
{
// prototypes
class Player;
class PlayerCam;
class Input;
class ObjectData;

class World
{
public:
    World();
    ~World();
    void Draw();
    void Update(float dt);
    void Reset();
    void PlayBGM(GameState state);
    void StopBGM();
    void Resize();

    PlayerCam*      m_pCamera;

private:
    Player*         m_pPlayer1;
    Player*         m_pPlayer2;

    Input*          m_pInputP1;
    Input*          m_pInputP2;

    OpenAL::Sound*  m_pBgm1;
    OpenAL::Sound*  m_pBgm2;
    OpenAL::Sound*  m_pSfx1;
    OpenAL::Sound*  m_pSfx2;

    std::deque<ObjectData*> m_enemies;

    std::deque<ObjectData*> m_objects;
};
}
