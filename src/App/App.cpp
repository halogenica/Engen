#include "System/Common.h"
#include "System/ObjectManager.h"
#include "App/App.h"
#include "App/Input.h"
#include "ObjectData/PhysicsData.h"
#include "Game/World.h"
#include "Game/PlayerCam.h"

using namespace gen;

// externs
extern ContactProcessedCallback gContactProcessedCallback;

// global declarations
gen::App*                   g_pApp;                 // the application
gen::Stats*                 g_pStats;               // the graphics and physics statistics
gen::ObjectManager*         g_pObjectManager;       // the object manager
btDiscreteDynamicsWorld*    g_pBtDynamicsWorld;     // the physics world
mowa::sgui::SimpleGUI*      g_pGui;                 // the GUI

// timestep variables
float       timeNow;                // the timestamp of the current frame in seconds
float       timePrev;               // the timestamp of the previous frame in seconds
float       timeFrame;              // the time delta for the current frame in seconds
float       timeSum;                // accumulation of time in seconds
const float timeDelta = 1 * .001f;  // the constant update time delta in seconds
const float timeMax = 16 * .001f;   // the constant maximum time delta in seconds

// bullet variables
btBroadphaseInterface*                  pBtBroadphase;              // the broadphase
btDefaultCollisionConfiguration*        pBtCollisionConfiguration;  // the collision config
btCollisionDispatcher*                  pBtDispatcher;              // the dispatch config
btSequentialImpulseConstraintSolver*    pBtSolver;                  // the physics solver
btDefaultMotionState*                   pBtGroundMotionState;
btRigidBody*                            pBtGroundRigidBody;

// crt debug variables
#ifdef _CRTDBG_MAP_ALLOC
_CrtMemState     memStateBegin;
_CrtMemState     memStateEnd;
_CrtMemState     memStateDiff;
#endif

/**************************************
Private functions
**************************************/
void gen::App::prepareSettings(Settings *settings)
{
    settings->setWindowSize(SCREEN_WIDTH, SCREEN_HEIGHT);
	settings->enableHighDensityDisplay();
    settings->setFrameRate(1000.0f);
    settings->setTitle("Game");
}

void gen::App::setup()
{
#ifdef _CRTDBG_MAP_ALLOC
    _CrtMemCheckpoint(&memStateBegin);
#endif
    g_pApp = this;

    timePrev = static_cast<float>(ci::app::getElapsedSeconds());
    timeNow  = 0.f;
    timeSum = 0.f;

    InitGamepad();

    InitBullet();   // Must init bullet before statistics

    OpenAL::InitOpenAL();

    srand(time(NULL));

    g_pStats = new Stats();
    g_pGui = new mowa::sgui::SimpleGUI(g_pApp, Font(loadResource(RES_STATS_FONT), 12));
    g_pObjectManager = new ObjectManager();
}

void gen::App::shutdown()
{
    delete g_pObjectManager;
    delete g_pGui;
    delete g_pStats;

    OpenAL::DestroyOpenAL();
    DestroyBullet();
    Gamepad_shutdown();

#ifdef _CRTDBG_MAP_ALLOC
    _CrtMemCheckpoint(&memStateEnd);
    _CrtMemDifference(&memStateDiff, &memStateBegin, &memStateEnd);
// TODO: _CrtDumpMemoryLeaks() finds leaks in many third party libraries, but we just want to know about this engine    
// TODO: _CrtMemDumpAllObjectsSince() also finds leaks in Cinder
    _CrtMemDumpAllObjectsSince(&memStateBegin);
#endif
}

void gen::App::update()
{
    timeNow = static_cast<float>(ci::app::getElapsedSeconds());
    timeFrame = timeNow - timePrev;
    timePrev = timeNow;

    g_pStats->UpdateFPS(timeFrame);

    // sound timer has higher resolution than game timer
    g_pObjectManager->UpdateSoundTimer(timeFrame);

    // practical limitation, don't let updates overcome framerate
    if (timeFrame > timeMax)
    {
        timeFrame = timeMax;
    }

    timeSum += timeFrame;

    // update at a rate of timeDiff, regardless of framerate
    while (timeSum >= timeDelta)
    {
        g_pObjectManager->Update(timeDelta);

        g_pBtDynamicsWorld->stepSimulation(timeDelta, 1, timeMax);
        g_pStats->UpdateUPS(timeDelta);

        timeSum -= timeDelta;
    }
}

void gen::App::draw()
{
    g_pObjectManager->Draw();
}

void gen::App::resize()
{
    g_pObjectManager->m_pWorld->Resize();
    g_pObjectManager->Resize();
}

void gen::App::keyDown(ci::app::KeyEvent event)
{
    const auto key = event.getCode();

    // Handle global keyboard events, regardless of player
    switch (key)
    {
#ifdef DEBUG
        case KEY_TOGGLE_STATS:
            g_pObjectManager->m_displayStats = !g_pObjectManager->m_displayStats;
            break;

        case KEY_TOGGLE_WIREFRAME:
            g_pObjectManager->m_wireframe = !g_pObjectManager->m_wireframe;
            break;

        case KEY_ADVANCE_GAME_STATE:
            switch (g_pObjectManager->m_gameState)
            {
                case TITLE:
                    g_pObjectManager->m_gameState = RULES;
                    break;
                case RULES:
                    g_pObjectManager->m_gameState = PLAYING;
                    break;
                case PLAYING:
                    g_pObjectManager->m_gameState = GAME_OVER;
                    break;
                case GAME_OVER:
                    g_pObjectManager->m_gameState = CREDITS;
                    break;
                case CREDITS:
                    g_pObjectManager->m_gameState = TITLE;
                    break;
                default:
                    break;
            }
            break;
#endif

        case KEY_TOGGLE_GUI:
            g_pObjectManager->m_displayGui = !g_pObjectManager->m_displayGui;
            break;

        case KEY_TOGGLE_FULLSCREEN:
            g_pObjectManager->m_fullscreen = !g_pObjectManager->m_fullscreen;
            g_pObjectManager->FullscreenToggled();
            break;

        case KEY_EXIT:
            g_pObjectManager->m_gameState = CREDITS;
            break;

        default:
            break;
    }
}

void gen::App::InitGamepad()
{
    Gamepad_deviceAttachFunc(gen::Input::DeviceAttached, (void *) 0x1);
    Gamepad_deviceRemoveFunc(gen::Input::DeviceRemoved, (void *) 0x2);
    Gamepad_buttonDownFunc(gen::Input::ButtonDown, (void *) 0x3);
    Gamepad_buttonUpFunc(gen::Input::ButtonUp, (void *) 0x4);
    Gamepad_axisMoveFunc(gen::Input::AxisMoved, (void *) 0x5);
    Gamepad_init();
}

void gen::App::InitBullet()
{
    // Build the broadphase
    pBtBroadphase = new btDbvtBroadphase();
 
    // Set up the collision configuration and dispatcher
    pBtCollisionConfiguration = new btDefaultCollisionConfiguration();
    pBtDispatcher = new btCollisionDispatcher(pBtCollisionConfiguration);
 
    // The actual physics solver
    pBtSolver = new btSequentialImpulseConstraintSolver;
 
    // The physics world
    g_pBtDynamicsWorld = new btDiscreteDynamicsWorld(pBtDispatcher,pBtBroadphase,pBtSolver,pBtCollisionConfiguration);
    g_pBtDynamicsWorld->setGravity(btVector3(0, GRAVITY_CONST, 0));

    // Register callbacks
    gContactProcessedCallback = (ContactProcessedCallback)PhysicsData::CollisionCallback;
}

void gen::App::DestroyBullet()
{
    delete g_pBtDynamicsWorld;
    delete pBtSolver;
    delete pBtDispatcher;
    delete pBtCollisionConfiguration;
    delete pBtBroadphase;
}
