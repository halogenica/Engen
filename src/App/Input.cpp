#include "System/ObjectManager.h"
#include "App/Input.h"
#include "App/App.h"
#include "ObjectData/ObjectData.h"
#include "ObjectData/PhysicsData.h"
#include "ObjectData/RenderData.h"
#include "Game/Player.h"
#include "Game/World.h"

using namespace gen;
using namespace ci;
using namespace ci::app;

#define POLL_ITERATION_INTERVAL 10

std::deque<Input*> Input::s_inputs;
std::deque<unsigned int> Input::s_gamepads;
std::map<unsigned int, Input*> Input::s_gamepadDeviceMap;

Input::Input(Player* pPlayer, const KeyMap& keymap, const PadMap& padmap) : m_pPlayer(pPlayer), m_keymap(keymap), m_padmap(padmap), m_keyState(), m_padState()
{
    g_pApp->getWindow()->connectKeyDown(&Input::KeyDown, this);
    g_pApp->getWindow()->connectKeyUp(&Input::KeyUp, this);
    g_pApp->getWindow()->connectMouseDown(&Input::MouseDown, this);
    g_pApp->getWindow()->connectMouseDrag(&Input::MouseDrag, this);

    AddInput(this);
}

Input::~Input()
{
    RemoveInput(this);
}

void Input::AddInput(Input* pInput)
{
    if (s_gamepads.size() > s_inputs.size())
    {
        // If there's an available gamepad, map it to this input
        s_inputs.push_front(pInput);
        auto gamepad = s_gamepads.back();
        s_gamepads.pop_back();
        s_gamepads.push_front(gamepad);
        s_gamepadDeviceMap[gamepad] = pInput;
    }
    else
    {
        s_inputs.push_back(pInput);
    }
}

void Input::RemoveInput(Input* pInput)
{
    s_inputs.erase(remove(s_inputs.begin(), s_inputs.end(), pInput), s_inputs.end());
    auto itr = s_gamepadDeviceMap.begin();
    while (itr != s_gamepadDeviceMap.end())
    {
        if (itr->second == pInput)
        {
            auto gamepad = itr->first;
            itr = s_gamepadDeviceMap.erase(itr);
            // re-add the gamepad this input was mapped to so it's available to other inputs
            RemoveGamepad(gamepad);
            AddGamepad(gamepad);
        }
        else
        {
            itr++;
        }
    }
}

void Input::Update(float dt)
{
	static unsigned int iterationsToNextPoll = POLL_ITERATION_INTERVAL;
	
	iterationsToNextPoll--;
	if (iterationsToNextPoll == 0) {
		Gamepad_detectDevices();
		iterationsToNextPoll = POLL_ITERATION_INTERVAL;
	}
    
    Gamepad_processEvents();

    // handle constant "button down" events, like flying

    if (g_pObjectManager->m_gameState == PLAYING)
    {
        if (m_keyState.moveLeft)
        {
            m_pPlayer->SetPos(m_pPlayer->GetPos() + (Vec3f(-10.f, 0.f, 0.f) * dt));
        }
        if (m_keyState.moveRight)
        {
            m_pPlayer->SetPos(m_pPlayer->GetPos() + (Vec3f(10.f, 0.f, 0.f) * dt));
        }
        if (m_keyState.moveUp)
        {
            m_pPlayer->SetPos(m_pPlayer->GetPos() + (Vec3f(0.f, 10.f, 0.f) * dt));
        }
        if (m_keyState.moveDown)
        {
            m_pPlayer->SetPos(m_pPlayer->GetPos() + (Vec3f(0.f, -10.f, 0.f) * dt));
        }

        if (abs(m_padState.moveXAxis) > 0.2f)
        {
            m_pPlayer->SetPos(m_pPlayer->GetPos() + (Vec3f(10.f, 0.f, 0.f) * dt * m_padState.moveXAxis));
        }
        if (abs(m_padState.moveYAxis) > 0.2f)
        {
            m_pPlayer->SetPos(m_pPlayer->GetPos() + (Vec3f(0.f, -10.f, 0.f) * dt * m_padState.moveYAxis));
        }
    }
}

bool Input::KeyDown(KeyEvent event)
{
    const auto key = event.getCode();
    bool retval = true;

    // Handle input depending on state
    if (g_pObjectManager->m_gameState == TITLE)
    {
        if (key == m_keymap.shoot)
        {
            g_pObjectManager->m_gameState = RULES;
        }
    }
    else if (g_pObjectManager->m_gameState == RULES)
    {
        if (key == m_keymap.shoot)
        {
            g_pObjectManager->m_gameState = PLAYING;
        }
    }
    else if (g_pObjectManager->m_gameState == PLAYING)
    {
        if (key == m_keymap.moveUp)
        {
            m_keyState.moveUp = true;
        }
        else if (key == m_keymap.moveDown)
        {
            m_keyState.moveDown = true;
        }
        else if (key == m_keymap.moveLeft)
        {
            m_keyState.moveLeft = true;
        }
        else if (key == m_keymap.moveRight)
        {
            m_keyState.moveRight = true;
        }
        else if (key == m_keymap.shoot)
        {
            m_pPlayer->Shoot();
        }
        else
        {
            retval = false;   // the input was not handled
        }
    }
    else if (g_pObjectManager->m_gameState == GAME_OVER)
    {
        if (key == m_keymap.shoot)
        {
            if (g_pObjectManager->m_gameoverTimer <= 0)
            {
                g_pObjectManager->m_gameState = PLAYING;
                g_pObjectManager->Restart();
            }
        }
    }

    return (retval);
}

bool Input::KeyUp(KeyEvent event)
{
    const auto key = event.getCode();
    bool retval = true;

    if (key == m_keymap.moveUp)
    {
        m_keyState.moveUp = false;
    }
    else if (key == m_keymap.moveDown)
    {
        m_keyState.moveDown = false;
    }
    else if (key == m_keymap.moveLeft)
    {
        m_keyState.moveLeft = false;
    }
    else if (key == m_keymap.moveRight)
    {
        m_keyState.moveRight = false;
    }
    else
    {
        retval = false;   // the input was not handled
    }

    return retval;
}

bool Input::MouseDown(MouseEvent event)
{
    return false;
}

bool Input::MouseDrag(MouseEvent event)
{
    return false;
}

bool Input::MouseUp(MouseEvent event)
{
    return false;
}

bool Input::MouseWheel(MouseEvent event)
{
    return false;
}

bool Input::TouchesBegan(TouchEvent event)
{
    return false;
}

bool Input::TouchesMoved(TouchEvent event)
{
    return false;
}

bool Input::TouchesEnded(TouchEvent event)
{
    return false;
}

/**************************************
Gamepad code
**************************************/
#ifdef DEBUG
bool verbose = true;
#else
bool verbose = false;
#endif

void Input::ButtonDown(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context) 
{
	if (verbose) 
    {
        console() << "Button " << buttonID << " down on device " << device->deviceID << " at " << timestamp << " with context " << context << std::endl;
	}

    Input* pInput = NULL;
    pInput = s_gamepadDeviceMap[device->deviceID];

    if (pInput)
    {
        if (g_pObjectManager->m_gameState == TITLE)
        {
            if (buttonID == pInput->m_padmap.shoot)
            {
                g_pObjectManager->m_gameState = RULES;
            }
        }
        else if (g_pObjectManager->m_gameState == RULES)
        {
            if (buttonID == pInput->m_padmap.shoot)
            {
                g_pObjectManager->m_gameState = PLAYING;
            }
        }
        else if (g_pObjectManager->m_gameState == PLAYING)
        {
            if (buttonID == pInput->m_padmap.shoot)
            {
                pInput->m_pPlayer->Shoot();
            }
        }
        else if (g_pObjectManager->m_gameState == GAME_OVER)
        {
            if (buttonID == pInput->m_padmap.shoot)
            {
                if (g_pObjectManager->m_gameoverTimer <= 0)
                {
                    g_pObjectManager->m_gameState = PLAYING;
                    g_pObjectManager->Restart();
                }
            }
        }
    }
}

void Input::ButtonUp(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context) 
{
	if (verbose) 
    {
        console() << "Button " << buttonID << " up on device " << device->deviceID << " at " << timestamp << " with context " << context << std::endl;
	}
}

void Input::AxisMoved(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context) 
{
	if (verbose && axisID < 38) // PS3 controllers register motion sensors as axes starting around 38, so mute them
    {
        if (value <= -0.3f || value >= 0.3f)
        {
        console() << "Axis " << axisID << " moved from " << lastValue <<" to " << value << " on device " << device->deviceID << " at " << timestamp << " with context " << context << std::endl;
        }
	}

    Input* pInput = NULL;
    pInput = s_gamepadDeviceMap[device->deviceID];

    if (pInput)
    {
        if (g_pObjectManager->m_gameState == PLAYING)
        {
            if (axisID == pInput->m_padmap.moveXAxis)
            {
                pInput->m_padState.moveXAxis = value;
            }
            else if (axisID == pInput->m_padmap.moveYAxis)
            {
                pInput->m_padState.moveYAxis = value;
            }
        }
    }
}

void Input::DeviceAttached(struct Gamepad_device * device, void * context) 
{
	if (verbose) 
    {
        console() << "Device ID " << device->deviceID << " attached (vendor = " << device->vendorID <<"; product = " << device->productID << ") with context" << context << std::endl;
    }

    AddGamepad(device->deviceID);
}

void Input::DeviceRemoved(struct Gamepad_device * device, void * context) 
{
	if (verbose) 
    {
        console() << "Device ID " << device->deviceID << " removed with context " << context << std::endl;
	}

    RemoveGamepad(device->deviceID);
}

void Input::AddGamepad(unsigned int gamepad)
{
    if (s_inputs.size() > s_gamepads.size())
    {
        // If there's an available input, map it to this gamepad
        s_gamepads.push_front(gamepad);
        auto pInput = s_inputs.back();
        s_inputs.pop_back();
        s_inputs.push_front(pInput);
        s_gamepadDeviceMap[gamepad] = pInput;
    }
    else
    {
        s_gamepads.push_back(gamepad);
    }
}

void Input::RemoveGamepad(unsigned int gamepad)
{
    s_gamepads.erase(remove(s_gamepads.begin(), s_gamepads.end(), gamepad), s_gamepads.end());
    auto itr = s_gamepadDeviceMap.begin();
    while (itr != s_gamepadDeviceMap.end())
    {
        if (itr->first == gamepad)
        {
            auto pInput = itr->second;
            itr = s_gamepadDeviceMap.erase(itr);
            // re-add the input this gamepad was mapped to so it's available to other gamepads
            RemoveInput(pInput);
            AddInput(pInput);
        }
        else
        {
            itr++;
        }
    }
}
