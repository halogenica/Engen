#include "Game/World.h"
#include "Game/Enemy.h"
#include "Game/Player.h"
#include "Game/PlayerCam.h"
#include "App/App.h"
#include "App/Input.h"
#include "ObjectData/ObjectData.h"
#include "ObjectData/RenderData.h"
#include "ObjectData/PhysicsData.h"

using namespace gen;
using namespace ci;

World::World()
{
    // Setup Camera
    // Could be one per player, but this demo has a single camera
    CameraPersp     camera;
    camera.setPerspective(60, getWindowAspectRatio(), 1, 1000);
    camera.lookAt(Vec3f(0, 0, 10), Vec3f::zero());
    camera.setCenterOfInterestPoint(Vec3f::zero());
    m_pCamera = new PlayerCam(camera);

    // Load Sounds
    m_pBgm1 = new OpenAL::Sound(ci::app::loadResource(RES_BGM1_SOUND));
    m_pBgm2 = new OpenAL::Sound(ci::app::loadResource(RES_BGM2_SOUND));
    m_pSfx1 = new OpenAL::Sound(ci::app::loadResource(RES_SFX1_SOUND));
    m_pSfx2 = new OpenAL::Sound(ci::app::loadResource(RES_SFX2_SOUND));

    // Create Players
    m_pPlayer1 = new Player(Vec3f(-2.5f, 0.f, 3.f), m_pSfx1);
    m_pPlayer2 = new Player(Vec3f(2.5f, 0.f, 3.f), m_pSfx1);

    // Setup Input
    KeyMap keymap1 = {app::KeyEvent::KEY_UNKNOWN};
    keymap1.moveUp       = ci::app::KeyEvent::KEY_UP;
    keymap1.moveDown     = ci::app::KeyEvent::KEY_DOWN;
    keymap1.moveLeft     = ci::app::KeyEvent::KEY_LEFT;
    keymap1.moveRight    = ci::app::KeyEvent::KEY_RIGHT;
    keymap1.shoot        = ci::app::KeyEvent::KEY_SPACE;

    KeyMap keymap2 = {app::KeyEvent::KEY_UNKNOWN};
    keymap2.moveUp       = ci::app::KeyEvent::KEY_w;
    keymap2.moveDown     = ci::app::KeyEvent::KEY_s;
    keymap2.moveLeft     = ci::app::KeyEvent::KEY_a;
    keymap2.moveRight    = ci::app::KeyEvent::KEY_d;
    keymap2.shoot        = ci::app::KeyEvent::KEY_SPACE;

    PadMap padmap = {PAD_UNKNOWN};
    padmap.moveYAxis    = PAD_LSTICK_Y;
    padmap.moveXAxis    = PAD_LSTICK_X;
    padmap.shoot        = PAD_1;

    m_pInputP1 = new gen::Input(m_pPlayer1, keymap1, padmap);
    m_pInputP2 = new gen::Input(m_pPlayer2, keymap2, padmap);

    // Create Enemies
    float rot = GetRandomMinMax(-PI/4, PI/4);

    Enemy* pEnemy1 = new Enemy(Enemy::BLACK, Vec3f(0.f,0.f,0.f), Quatf(0.f,0.f,rot), Vec3f(2.f,2.f,2.f), m_pSfx2);
    m_enemies.push_back(pEnemy1);
    rot = GetRandomMinMax(-PI/4, PI/4);

    Enemy* pEnemy2 = new Enemy(Enemy::BLACK, Vec3f(0.f,3.f,0.f), Quatf(0.f,0.f,rot), Vec3f(2.f,2.f,2.f), m_pSfx2);
    m_enemies.push_back(pEnemy2);
    rot = GetRandomMinMax(-PI/4, PI/4);

    Enemy* pEnemy3 = new Enemy(Enemy::RED,   Vec3f(0.f,6.f,0.f), Quatf(0.f,0.f,rot), Vec3f(2.f,2.f,2.f), m_pSfx2);
    m_enemies.push_back(pEnemy3);

    // Create Scene
    ObjectData* pGround =   new ObjectData(new RenderData(ci::app::loadResource(RES_SQUARE_OBJ), ci::ColorAf(0.8f, 0.8f, 0.8f, 1.0f)),
                                           new PhysicsData(new btStaticPlaneShape(btVector3(0,0,1), 1), 0, COL_GROUND, COL_EVERYTHING),
                                           Vec3f(0, -5, 0), Quatf(-PI/2, 0, 0), Vec3f(1000.f, 1000.f, 1.f));   // infinite plane
    m_objects.push_back(pGround);
}

World::~World()
{
    for (auto pEnemy : m_enemies)
    {
        delete(pEnemy);
    }

    for (auto pObject : m_objects)
    {
        delete(pObject);
    }

    delete m_pInputP1;
    delete m_pInputP2;

    delete m_pPlayer1;
    delete m_pPlayer2;

    delete m_pBgm1;
    delete m_pBgm2;
    delete m_pSfx1;
    delete m_pSfx2;

    delete m_pCamera;
}

void World::Draw()
{
    m_pPlayer1->Draw();
    m_pPlayer2->Draw();

    for (auto pObject : m_objects)
    {
        pObject->Draw();
    }

    for (auto pEnemy : m_enemies)
    {
        pEnemy->Draw();
    }
}

void World::Update(float dt)
{
    m_pCamera->Update(dt);
    m_pInputP1->Update(dt);
    m_pInputP2->Update(dt);
    m_pPlayer1->Update(dt);
    m_pPlayer2->Update(dt);

    for (auto pObject : m_objects)
    {
        pObject->Update(dt);
    }

    for (auto pEnemy : m_enemies)
    {
        pEnemy->Update(dt);
    }
}

void World::Reset()
{
    m_pCamera->Reset();
    m_pPlayer1->Reset();
    m_pPlayer2->Reset();

    float rot = GetRandomMinMax(-PI/4, PI/4);
    float height = 0.f;

    for (auto pEnemy : m_enemies)
    {
        pEnemy->SetPos(Vec3f(0.f, height, 0.f));
        pEnemy->SetRot(Quatf(0.f, 0.f, rot));
        height += 3.f;
        rot = GetRandomMinMax(-PI/4, PI/4);
    }
}

void World::PlayBGM(GameState state)
{
    if (state == TITLE)
    {
        m_pBgm1->Play();
    }
    else
    {
        m_pBgm2->Play();
    }
}

void World::StopBGM()
{
    m_pBgm1->Stop();
    m_pBgm2->Stop();
}

void World::Resize()
{
    CameraPersp cam = m_pCamera->GetCamera();
    cam.setPerspective(60, getWindowAspectRatio(), 1, 1000);
    m_pCamera->SetCurrentCam(cam);
}
