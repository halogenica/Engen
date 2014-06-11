#pragma once

#include "System/Common.h"

namespace gen
{
// prototypes
class ObjectManager;
class Player;

enum INPUT_KEY
{
    KEY_TOGGLE_GUI          = ci::app::KeyEvent::KEY_F1,
#ifdef DEBUG
    KEY_TOGGLE_STATS        = ci::app::KeyEvent::KEY_F2,
    KEY_TOGGLE_WIREFRAME    = ci::app::KeyEvent::KEY_F3,
    KEY_ADVANCE_GAME_STATE  = ci::app::KeyEvent::KEY_F4, 
#endif
    KEY_TOGGLE_FULLSCREEN   = ci::app::KeyEvent::KEY_f,
    KEY_EXIT                = ci::app::KeyEvent::KEY_ESCAPE,
};

struct KeyMap
{
    UINT moveUp;
    UINT moveDown;
    UINT moveLeft;
    UINT moveRight;
    UINT shoot;
};

struct PadMap
{
    int moveYAxis;
    int moveXAxis;
    int shoot;
};

// TODO: add mappings for multiple gamepads on a single platform with common names
// TODO: split axis into positive and negative directions
enum GAMEPAD
{
#ifdef __APPLE__    // Support PS3 controllers on MacOSX
//  PS3 - vendorID == 1356
    PAD_1           = 14,   // Cross
    PAD_2           = 13,   // Circle
    PAD_3           = 15,   // Square
    PAD_4           = 12,   // Triangle
    PAD_L1          = 10,
    PAD_R1          = 11,
    PAD_SELECT      = 0,
    PAD_START       = 3,
    PAD_L3          = 1,
    PAD_R3          = 2,
    PAD_LSTICK_X    = 0,    // -1 left to 1 right
    PAD_LSTICK_Y    = 1,    // 1 down to -1 up
    PAD_RSTICK_X    = 2,    // -1 left to 1 right
    PAD_RSTICK_Y    = 3,    // 1 down to -1 up

    // The following axes are handled differently on PS3 and 360 controllers
    PAD_L2_AXIS     = 14,   // -1 to 1
    PAD_L2_BUTTON   = 8,
    PAD_R2_AXIS     = 15,   // -1 to 1
    PAD_R2_BUTTON   = 9,
    PAD_DPAD_RIGHT  = 4,
    PAD_DPAD_UP     = 5,
    PAD_DPAD_DOWN   = 6,
    PAD_DPAD_LEFT   = 7,

#else           // Support XBOX controllers on Windows
//  360 - vendorID == 1118
    PAD_1           = 0,    // A
    PAD_2           = 1,    // B
    PAD_3           = 2,    // X
    PAD_4           = 3,    // Y
    PAD_L1          = 4,
    PAD_R1          = 5,
    PAD_SELECT      = 6,
    PAD_START       = 7,
    PAD_L3          = 8,
    PAD_R3          = 9,
    PAD_LSTICK_X    = 0,    // -1 left to 1 right
    PAD_LSTICK_Y    = 1,    // 1 down to -1 up
    PAD_RSTICK_X    = 4,    // -1 left to 1 right
    PAD_RSTICK_Y    = 3,    // 1 down to -1 up

    // The following axes are handled differently on PS3 and 360 controllers
    PAD_L2R2        = 2,    // 0 to 1
    PAD_DPAD_X      = 5,    // -1 left or 1 right
    PAD_DPAD_Y      = 6,    // 1 down or -1 up
#endif

    PAD_UNKNOWN     = -1,
};
    
struct KeyState // For continuous input
{
    // TODO: These expand to bytes. big deal?
    bool moveUp;
    bool moveDown;
    bool moveLeft;
    bool moveRight;
};

struct PadState // For continuous input
{
    // TODO: These expand to bytes. big deal?
    float moveXAxis;
    float moveYAxis;
};

class Input
{
public:
    Input(Player* pPlayer, const KeyMap& keymap, const PadMap& padmap);
    virtual ~Input();
    void Update(float dt);  // For continuous input

    // cinder events
    bool KeyDown(ci::app::KeyEvent event);
    bool KeyUp(ci::app::KeyEvent event);
    bool MouseDown(ci::app::MouseEvent event);
    bool MouseDrag(ci::app::MouseEvent event);
    bool MouseUp(ci::app::MouseEvent event);
    bool MouseWheel(ci::app::MouseEvent event);
    bool TouchesBegan(ci::app::TouchEvent event);
    bool TouchesMoved(ci::app::TouchEvent event);
    bool TouchesEnded(ci::app::TouchEvent event);

    // joystick events
    static void ButtonDown(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context);
    static void ButtonUp(struct Gamepad_device * device, unsigned int buttonID, double timestamp, void * context);
    static void AxisMoved(struct Gamepad_device * device, unsigned int axisID, float value, float lastValue, double timestamp, void * context);
    static void DeviceAttached(struct Gamepad_device * device, void * context);
    static void DeviceRemoved(struct Gamepad_device * device, void * context);
    
private:
    // track inputs and gamepads, unmapped elements always at the back
    static std::deque<Input*> s_inputs;
    static std::deque<unsigned int> s_gamepads;
    // map inputs to gamepads
    static std::map<unsigned int, Input*> s_gamepadDeviceMap;

    static void AddGamepad(unsigned int gamepad);
    static void RemoveGamepad(unsigned int gamepad);
    static void AddInput(Input* pInput);
    static void RemoveInput(Input* pInput);

    Player*     m_pPlayer;
    KeyState    m_keyState;
    PadState    m_padState;
    KeyMap      m_keymap;
    PadMap      m_padmap;
};

}
