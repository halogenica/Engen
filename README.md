#Engen Game Engine
=================

##About the engine:
-----------------
Engen is an extremely small (~2000 line) C++ 3D game engine specifically designed for cross platform rapid prototyping. It is built on top of the Cinder creative coding framework (libcinder.org), and targets Windows, Mac OSX and iOS. It specifically provides a rough organization of the required components to get a game working: render and physics data, input, application window, sound, UI, and statistics. The objective is to make writing game code straight forward and simple while maintaining low-level control of the engine for subtle tweaking or performance optimizations (as opposed to scripting languages). It is intended as a guideline, but encourages modification of any of the components for fast iteration. The organization of code is simply what has worked best for me when making games. It is built on entirely open source technologies which permit use in commercial products (note that on windows OpenAL should be distributed as a dynamic lib for commercial projects to avoid code contamination).

###Features include:
*	3D physics with Bullet
*	3D positional audio with OpenAL
*	Gamepad support for arbitrary controllers (mappings for PS3 and 360)
*	Decoupled timestep for Draw/Update in main loop
*	Input objects supporting any number of players with unique button mappings
*	Expandable component model for objects (Render, Physics, Animation, AI, etc.)
*	Minimalist user interface for options
*	Statistics to display FPS, number of triangles, physics bodies, audio sources, etc.
*	Load arbitrary 3D models and file types
*	Shader integration (currently not available in iOS but will soon be fixed in Cinder)
*	Collision callbacks
*	Font rendering
*	Resource and static libs generate a single executable

I've made 3 games with some version of this engine, each created in only 48 hours for Ludum Dare.
*	*Zero Zen* - http://www.ludumdare.com/compo/ludum-dare-26/?action=preview&uid=12262
*	*Aggrogate* - http://www.ludumdare.com/compo/ludum-dare-28/?action=preview&uid=12262
*	*Cull Shot* - http://www.ludumdare.com/compo/ludum-dare-29/?action=preview&uid=12262

To suppliment some "missing" components in Cinder, I have created a few Cinder plugins called "blocks" which are also available on my github repo. They are required for correctly building this engine.

Engen by Michael Romero
http://halogenica.net


##System requirements:
--------------------
* VS2012 on Windows 7+
* Xcode 5 on Mac OSX 10.7+


##To build from source:
---------------------
Built using VS2012 on Win, Xcode5 on Mac

All project paths are relative, root can be whatever you want. My root (~) is C:/Code/..

###Directory trees:
* ~/games/Engen/..  (requires all of the below to be built first)
* ~/libraries/cinder_0.8.6_dev/.. (github.com/cinder/Cinder/tree/dev, requies boost to build)
* ~/libraries/cinder_0.8.6_dev/boost/.. (sourceforge.net/projects/boost/files/boost/1.53.0/)
* ~/libraries/cinder_0.8.6_dev/blocks/Bullet/.. (github.com/halogenica/Cinder-Bullet)
* ~/libraries/cinder_0.8.6_dev/blocks/Gamepad/..  (github.com/halogenica/Cinder-Gamepad)
* ~/libraries/cinder_0.8.6_dev/blocks/OpenAL/.. (github.com/halogenica/Cinder-OpenAL)
* ~/libraries/cinder_0.8.6_dev/blocks/SimpleGUI/..  (github.com/halogenica/Cinder-SimpleGUI)


##About the example project:
--------------------------
An example game is included in this source. It is a simple demonstration of how to organize game logic in the engine. Two players represented by spheres can be controlled independently using 2 gamepads or WASD and arrows on the keyboard. Three cubes exist in the scene, one of which is red. The game is over when one of the players fires a projectile which collides with the red cube. It demonstrates how to create independent players, load models and textures, render arbitrary fonts for dynamic text, perform 3D physics simulation with collision callbacks, play music and sound effects, utilize game controller and keyboard input, and rough organization of game logic.

###Controls:
-	*Up, Down, Left, Right* - Move Player 1
-	*WASD* - Move Player 2
-	*Space* - Shoot (both players)
-	*F1* - Toggle GUI
-	*F2* - Toggle Statistics (Debug only)
-	*F3* - Toggle Wireframe (Debug only)
-	*F4* - Advance Game State (Debug only)
-	*F* - Fullscreen
-	*ESC* - Exit

###Gamepad Controls:
-	*Left Analog* - Move Player
-	*A button (360) / X button (PS3)* - Shoot

###File Structure:
-	*App/App* - Setup, shutdown and main loop. Inherits from Cinder's AppNative with implicit calls to draw() and update().
-	*App/Input* - Per-player input handling of both keyboard/mouse and gamepad events.
-	*Blocks/...* - Cinder block interfaces (Cinder plugins).
-	*Game/Enemy* - The large cubes in the demo, inherit from ObjectData with some metadata and collision function.
-	*Game/Player* - The large spheres in the demo with a collection of bullets, and a definition of the bullets with metadata.
-	*Game/PlayerCam* - Wrapper for independent cammeras per player, but only a single camera used in this demo.
-	*Game/World* - Responsible for setting up game-specific structures (camera, players, enemies, etc). Only a single instance.
-	*ObjectData/ObjectData* - Root object of component based model; integrates RenderData and PhysicsData, and any other component data you may wish to add (AIData, AnimationData, etc). For example, physics can change position of an object, which render needs to know about, so ObjectData reconciles this information.
-	*ObjectData/PhysicsData* - Bullet physics wrapper for physics bounding box and collision information.
- 	*ObjectData/RenderData* - 3D mesh, color and texture information. Does not implement a material system yet. 
-	*ObjectData/SpriteSheet* - Static or animated sprite textures, work in progress.
-	*Particles/LineParticle* - Inherits from Particle with different emitter behavior; not used in demo.
-	*Particles/Particle* - Interface for polymorphic particles, not used in demo.
-	*Particles/SquareParticle* - Inherits from Particle with different emitter behavior, not used in demo.
-	*System/Common* - Common includes, macros, definitions, globals, constants and helper functions
-	*System/ObjectManager* - Manage system options such as toggling fullscreen, gravity, music, debug info, etc. Handles overlay and user interface. Also handles game screens, however this should probably move to World object. There is only a single ObjectManager, and it is globally accessible for modifying options in the engine from anywhere (such as toggling fullscreen with press of a button)
-	*System/Resources* - These are all content files used within the game. Adding files here will compile them into final executable binary.
-	*Utils/Stats* - Track and display stats such as FPS, triangle count, physics bodies, audio sources, current game state, etc.


##TODO:
-----
*	Clarify division between ObjectManager and World
*	More intelligent spritesheets and animation
*	Autodetect or create physics bounding box for arbitrary model
*	Proper 3D animation support with skinning
*	Rumble support for controllers
*	Memory tracking revealed leaks in the Cinder library on windows
