#pragma once

#include "System/Common.h"
#include "App/Input.h"
#include "Utils/Stats.h"

namespace gen
{
// protos
class ObjectData;
class World;

class ObjectManager
{
public:
    ObjectManager();
    ~ObjectManager();
    void Update(float dt);
    void Draw();
    void UpdateSoundTimer(float dt);    // this is its own function because the timing is very sensitive
    void Start();                       // transition from Ready to Playing state.
    void Restart();
    void Resize();
    void GameOver();
    void FullscreenToggled();
    void MusicToggled();

    GameState       m_gameState;
    bool            m_gravity;
    bool            m_displayStats;
    bool            m_displayGui;
    bool            m_fullscreen;
    bool            m_wireframe;
    bool            m_playMusic;

    std::deque<ObjectData*> m_objects;
    World*          m_pWorld;

    ci::gl::Fbo     m_fbo;
    ci::gl::Fbo::Format m_fboFormat;

    float           m_masterVolume;
    float           m_playtime;
    float           m_creditsTimer;
    float           m_gameoverTimer;
    float           m_loopPlaytime;
    float           m_bgTrackLength;

    ci::gl::Texture m_titleTex;
    ci::gl::Texture m_rulesTex;
    ci::gl::Texture m_gameoverTex;
    ci::gl::Texture m_creditsTex;

    ci::gl::GlslProg    m_passthroughShader;

    ci::TextBox     m_text;

private:
    void CreateScreenTextures();
    void DrawRules();
    void DrawGameOver();
    void DrawTitle();
    void DrawCredits();
};
}
