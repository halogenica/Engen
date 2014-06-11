#include "System/ObjectManager.h"
#include "App/App.h"
#include "ObjectData/ObjectData.h"
#include "ObjectData/RenderData.h"
#include "ObjectData/PhysicsData.h"
#include "Game/World.h"
#include "Game/PlayerCam.h"

using namespace gen;
using namespace ci;

ObjectManager::ObjectManager()
{
    // Initialize controls
    m_gameState     = TITLE;
    m_gravity       = true;
    m_displayStats  = false;
    m_displayGui    = false;
    m_fullscreen    = false;
    m_wireframe     = false;
    m_playMusic     = true;

    m_masterVolume  = 0.3f;
    m_playtime      = 0;
    m_creditsTimer  = 0;    // counts up
    m_gameoverTimer = 0;    // counts down

    g_pApp->setFullScreen(m_fullscreen);
    g_pBtDynamicsWorld->setGravity(btVector3(0, GRAVITY_CONST * m_gravity, 0));

    // Add World
    m_pWorld = new World();

    // FBO for full-screen post processing
    m_fboFormat.setSamples(16); // 16x MSAA
    m_fbo = gl::Fbo(g_pApp->getWindowWidth()*getWindow()->getContentScale(),
                    g_pApp->getWindowHeight()*getWindow()->getContentScale(),
                    m_fboFormat);
    
    // Load shaders
    m_passthroughShader = gl::GlslProg(ci::app::loadResource(RES_VERT_PASSTHROUGH_SHADER), ci::app::loadResource(RES_FRAG_PASSTHROUGH_SHADER));

    // Calculate BGM length
    // Could set looping in OpenAL sound, however these audio files were designed to
    // be retriggered and overlap before they finish playing 
    float bars = 8.f;
    float beatPerMin = 140.f;
    const float secPerMin = 60.f;
    const float beatPerBar = 4.f;
    m_bgTrackLength = secPerMin / beatPerMin * beatPerBar * bars;
    m_loopPlaytime = m_bgTrackLength;   // trick audio to think it's overdue for play

    // Load font for screens
    m_text.setFont(Font(app::loadResource(RES_GAME_FONT), 80));
    m_text.setColor(ci::Colorf(0.f, 0.f, 0.f));
    m_text.setAlignment(TextBox::CENTER);

    // Load textures
    m_titleTex = gl::Texture(ci::loadImage(ci::app::loadResource(RES_TITLE_TEX)));
    m_creditsTex = gl::Texture(ci::loadImage(ci::app::loadResource(RES_CREDITS_TEX)));
    CreateScreenTextures();

    // Setup GUI
    g_pGui->lightColor = ColorA(1, 0, 0, 1);
    g_pGui->addLabel("OPTIONS:");
    g_pGui->addSeparator();
    g_pGui->addParam("Fullscreen?", &m_fullscreen, m_fullscreen)->registerCallback(this, &ObjectManager::FullscreenToggled);
    g_pGui->addParam("Play Music?", &m_playMusic, m_playMusic)->registerCallback(this, &ObjectManager::MusicToggled);
    g_pGui->addParam("Master volume:", &m_masterVolume, 0.f, 1.f, m_masterVolume);
}

ObjectManager::~ObjectManager()
{
    for (auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        delete(*i);
    }
    delete m_pWorld;
}

void ObjectManager::Start()
{

}

void ObjectManager::Restart()
{
    m_playtime = 0;
    
    m_pWorld->Reset();

    g_pObjectManager->m_gameState = PLAYING;
}

void ObjectManager::Resize()
{
    CreateScreenTextures();
}

void ObjectManager::UpdateSoundTimer(float dt)
{
    m_playtime += dt;

    m_loopPlaytime += dt;

    if (m_bgTrackLength <= m_loopPlaytime)
    {
        m_loopPlaytime = 0;
        if (m_playMusic)
        {
            m_pWorld->PlayBGM(m_gameState);
        }
    }
}

void ObjectManager::GameOver()
{
    m_gameoverTimer = 2.f;    // counts down
    m_gameState = GAME_OVER;
}

void ObjectManager::FullscreenToggled()
{
    g_pApp->setFullScreen(m_fullscreen);
    m_fbo = gl::Fbo(g_pApp->getWindowWidth()*ci::app::getWindow()->getContentScale(),
                    g_pApp->getWindowHeight()*ci::app::getWindow()->getContentScale(),
                    m_fboFormat);
}

void ObjectManager::MusicToggled()
{
    if (m_playMusic)
    {
        m_pWorld->PlayBGM(m_gameState);
    }
    else
    {
        m_pWorld->StopBGM();
    }
}

void ObjectManager::CreateScreenTextures()
{
    std::stringstream displayString;

    displayString << "THESE ARE THE RULES:" << std::endl;
    displayString << "Shoot the red cube!";
    m_text.setSize(Vec2f(g_pApp->getWindowWidth(), TextBox::GROW));
    m_text.setText(displayString.str());
    m_rulesTex = gl::Texture(m_text.render());

    displayString.str("");
    displayString << "GAME OVER";
    m_text.setSize(Vec2f(g_pApp->getWindowWidth(), g_pApp->getWindowHeight()));
    m_text.setText(displayString.str());
    m_gameoverTex = gl::Texture(m_text.render());
}

void ObjectManager::DrawRules()
{
    gl::pushMatrices();
    gl::setMatricesWindow(getWindowSize());

    gl::draw(m_rulesTex, Vec2f(0,0));

    gl::popMatrices();
}

void ObjectManager::DrawGameOver()
{
    gl::color(.7f, .7f, .7f, .3f);
    gl::drawSolidRect(Rectf(0.f, 0.f, g_pApp->getWindowWidth(), g_pApp->getWindowHeight()));
    gl::color(1,1,1,1);

    gl::pushMatrices();
    gl::setMatricesWindow(getWindowSize());

    gl::draw(m_gameoverTex, Vec2f(0,0));

    gl::popMatrices();
}

void ObjectManager::DrawTitle()
{
    // Example of using a texture
    // Preferred method of rendering text is using a text box
    gl::color(.7f, .7f, .7f, .3f);
    gl::drawSolidRect(Rectf(0.f, 0.f, g_pApp->getWindowWidth(), g_pApp->getWindowHeight()));
    gl::color(1,1,1,1);
    gl::draw(m_titleTex, g_pApp->getWindowBounds());
}

void ObjectManager::DrawCredits()
{
    // Example of using a texture
    // Preferred method of rendering text is using a text box
    gl::color(.7f, .7f, .7f, .3f);
    gl::drawSolidRect(Rectf(0.f, 0.f, g_pApp->getWindowWidth(), g_pApp->getWindowHeight()));
    gl::color(1,1,1,1);
    gl::draw(m_creditsTex, g_pApp->getWindowBounds());
}

void ObjectManager::Update(float dt)
{
    // Update assets
    for (auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        (*i)->Update(dt);
    }

    if (m_pWorld)
    {
        m_pWorld->Update(dt);
    }

    if (m_gameState == GAME_OVER)
    {
        m_gameoverTimer -= dt;
    }

    if (m_gameState == CREDITS)
    {
        m_creditsTimer += dt;
        if (m_creditsTimer >= 2)
        {
            g_pApp->quit();
        }
    }

    OpenAL::SetListenerGain(m_masterVolume);
}

void ObjectManager::Draw()
{
    // Render state
#if ! defined(CINDER_GLES)
    m_wireframe ? gl::enableWireframe() : gl::disableWireframe();
#endif

    gl::enableVerticalSync();   // TODO: why can't this be set once at setup when changing FBOs?
    gl::enableAlphaBlending();
    gl::enableDepthRead();
    gl::enable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    gl::clear(ColorA(178/255.f,178/255.f,178/255.f,1.0f));

    m_passthroughShader.bind();
    m_passthroughShader.uniform("resolution", Vec2f(g_pApp->getWindowWidth(), g_pApp->getWindowHeight()));
    m_passthroughShader.uniform("resolutionScale", getWindow()->getContentScale());
    m_passthroughShader.uniform("timer", static_cast<float>(m_playtime));

    gl::pushMatrices();
    gl::setViewport(Area(0, 0, g_pApp->getWindowWidth()*getWindow()->getContentScale(), g_pApp->getWindowHeight()*getWindow()->getContentScale()));
    gl::setMatrices(m_pWorld->m_pCamera->GetCamera());

    for (auto i = m_objects.begin(); i != m_objects.end(); i++)
    {
        (*i)->Draw();
    }

    m_pWorld->Draw();

    gl::popMatrices();

    m_passthroughShader.unbind();

    // Draw game screens
    gl::pushMatrices();

    // overlay draws should ignore depth and blend
    gl::disableDepthRead();
    gl::disableDepthWrite();
    gl::enableAlphaBlending();
    gl::color(1,1,1,1);

    switch (m_gameState)
    {
        case TITLE:
            DrawTitle();
            break;
        case RULES:
            DrawRules();
            break;
        case PLAYING:
            // No special overlay screens while playing
            break;
        case GAME_OVER:
            DrawGameOver();
            break;
        case CREDITS:
            DrawCredits();
            break;
        default:
            break;
    }
    gl::disableAlphaBlending();
    gl::enableDepthRead();
    gl::enableDepthWrite();
    gl::color(ColorA(1,1,1,1));
    gl::popMatrices();


#if ! defined(CINDER_GLES)
    gl::disableWireframe();
#endif

    // Draw statistics
    if (m_displayStats)
    {
        g_pStats->Display();
    }

    // Draw gui
    if (m_displayGui)
    {
        g_pGui->draw();
    }
}
