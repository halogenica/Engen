#pragma once

#include <stdio.h>
#include <stdarg.h>
#include <stdint.h>
#include <math.h>
#include <cassert>
#include <sstream>
#include <algorithm>
#include <map>
#include <deque>

#include "cinder/app/AppNative.h"
#include "cinder/app/KeyEvent.h"
#include "cinder/gl/Texture.h"
#include "cinder/gl/TextureFont.h"
#include "cinder/gl/Fbo.h"
#include "cinder/audio/Output.h"
#include "cinder/audio/Io.h"
#include "cinder/ObjLoader.h"
#include "cinder/Camera.h"
#include "cinder/MayaCamUI.h"
#include "cinder/Cinder.h"
#include "cinder/Color.h"
#include "cinder/Quaternion.h"
#include "cinder/Text.h"
#include "cinder/ImageIo.h"
#include "cinder/Arcball.h"
#if ! defined(CINDER_GLES)  // shader support only on desktop
#include "cinder/gl/GlslProg.h"
#endif

// Compiled resources
#include "System/Resources.h"

// Bullet block
#include "btBulletDynamicsCommon.h"

// SimpleGUI block
#include "SimpleGUI.h"

// Gamepad block
extern "C" {
#include "Gamepad.h"
}

// OpenAL block
#include "OpenAL.h"

// Detect type of apple device
#ifdef __APPLE__
#include "TargetConditionals.h"
#if TARGET_OS_IPHONE
// iOS device
#elif TARGET_IPHONE_SIMULATOR
// iOS Simulator
#elif TARGET_OS_MAC
// Other kinds of Mac OS
#endif
#endif

/**********************************************************\
*    Macros
\**********************************************************/
// Run time assert
#ifdef DEBUG
    #define ASSERT(x) {if (!(x)) {__debugbreak();}} // could also use abort()
#else
    #define ASSERT(x)
#endif

// Compile time assert
#define CASSERT(predicate) _impl_CASSERT_LINE(predicate,__LINE__,__FILE__)
#define _impl_PASTE(a,b) a##b
#define _impl_CASSERT_LINE(predicate, line, file) \
    typedef char _impl_PASTE(assertion_failed_##file##_,line)[2*!!(predicate)-1];

#define STRINGIFY( name ) # name

/**********************************************************\
*    Defines
\**********************************************************/
// define scene information
#define SCREEN_WIDTH    1280
#define SCREEN_HEIGHT   720

#define GRAVITY_CONST   -20.0f
#define MAX_VELOCITY    500.0f

// Specify memory tracking
// TODO: currently triggers memory leaks from Cinder
#define DEBUG_MEM_TRACKING  0

#if defined DEBUG && DEBUG_MEM_TRACKING
    #define _CRTDBG_MAP_ALLOC
    #include <crtdbg.h>
// TODO: bullet overloads operator new and doesn't provide definitions for crtdbg
//    #define new new(_NORMAL_BLOCK,__FILE__,__LINE__)
#endif


/**********************************************************\
*    Prototypes
\**********************************************************/
namespace gen
{
    class App;
    class Input;
    class Stats;
    class ObjectManager;
}

namespace mowa
{
    namespace sgui
    {
        class SimpleGUI;
    }
}


/**********************************************************\
*    Globals
\**********************************************************/
extern gen::App*                g_pApp;               // application
extern gen::Stats*              g_pStats;             // statistics
extern gen::ObjectManager*      g_pObjectManager;     // the object manager
extern btDiscreteDynamicsWorld* g_pBtDynamicsWorld;   // the physics world
extern mowa::sgui::SimpleGUI*   g_pGui;               // the GUI


/**********************************************************\
*    Enums
\**********************************************************/
namespace gen
{
enum CollisionTypes 
{
    COL_NOTHING     = 0x0,
    COL_GROUND      = 0x1 << 0,
    COL_BULLET      = 0x1 << 1,
    COL_PLAYER      = 0x1 << 2,
    COL_ENEMY       = 0x1 << 3,

    COL_EVERYTHING  = (0x1 << 4) - 1
};

enum GameState
{
    TITLE               = 0x0,
    RULES               = 0x1,
    PLAYING             = 0x2,
    GAME_OVER           = 0x3,
    CREDITS             = 0x4,

    NUM_GAME_STATES     = 0x5,
};
}


/**********************************************************\
*    Typedefs
\**********************************************************/
/*
namespace gen
{
typedef unsigned long       ULONG;
typedef unsigned int        UINT;
typedef unsigned short      USHORT;
typedef unsigned char       UCHAR;

typedef boost::uint64_t     QWORD;
typedef boost::uint32_t     DWORD;
typedef boost::uint16_t     WORD;
typedef boost::uint8_t      BYTE;
}
*/

#ifdef __APPLE__
typedef unsigned long       ULONG;
typedef unsigned int        UINT;
typedef unsigned short      USHORT;
typedef unsigned char       UCHAR;

typedef unsigned long long  QWORD;
typedef unsigned long       DWORD;
typedef unsigned short      WORD;
typedef unsigned char       BYTE;
#endif


/**********************************************************\
*    Constants
\**********************************************************/
namespace gen
{
const float PI       = 3.14159265358979323f;
const float MATH_EPS = 0.0001f;
}


/**********************************************************\
*    Helpers
\**********************************************************/
namespace gen
{

inline float GetRandomMinMax( float fMin, float fMax )
{
    float fRandNum = (float)rand() / RAND_MAX;
    return fMin + (fMax - fMin) * fRandNum;
}

inline ci::Vec3f GetRandomVector( void )
{
    ci::Vec3f vVector;

    // Pick a random Z between -1.0f and 1.0f.
    vVector.z = GetRandomMinMax( -1.0f, 1.0f );
    
    // Get radius of this circle
    float radius = (float)sqrt(1 - vVector.z * vVector.z);
    
    // Pick a random point on a circle.
    float t = GetRandomMinMax( -PI, PI );

    // Compute matching X and Y for our Z.
    vVector.x = (float)cosf(t) * radius;
    vVector.y = (float)sinf(t) * radius;

	return vVector;
}

}
