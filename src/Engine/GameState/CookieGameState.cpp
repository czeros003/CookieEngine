#include "OgreCommon/TutorialGameState.h"
#include "Engine/GameState/CookieBaseState.h"
#include "GraphicsSystem.h"

#include "OgreSceneManager.h"

#include "OgreOverlay.h"
#include "OgreOverlayContainer.h"
#include "OgreOverlayManager.h"
#include "OgreTextAreaOverlayElement.h"

#include "OgreFrameStats.h"
#include "OgreRoot.h"

#include "OgreGpuProgramManager.h"
#include "OgreHlms.h"
#include "OgreHlmsCompute.h"
#include "OgreHlmsManager.h"

namespace CEngine
{
    TutorialGameState::TutorialGameState(const Ogre::String& helpDescription) :
        mGraphicsSystem(0),
        mCameraController(0),
        mHelpDescription(helpDescription),
        mDisplayHelpMode(1),
        mNumDisplayHelpModes(2),
        mDebugText(0)
    {
    }
    //-----------------------------------------------------------------------------------
    TutorialGameState::~TutorialGameState()
    {
        delete mCameraController;
        mCameraController = 0;
    }
    //-----------------------------------------------------------------------------------
    void TutorialGameState::_notifyGraphicsSystem(Demo::GraphicsSystem* graphicsSystem)
    {
        mGraphicsSystem = graphicsSystem;
    }
    //-----------------------------------------------------------------------------------
    void TutorialGameState::createScene01() { createDebugTextOverlay(); }
    //-----------------------------------------------------------------------------------
    void TutorialGameState::createDebugTextOverlay()
    {
        Ogre::v1::OverlayManager& overlayManager = Ogre::v1::OverlayManager::getSingleton();
        Ogre::v1::Overlay* overlay = overlayManager.create("DebugText");

        Ogre::v1::OverlayContainer* panel = static_cast<Ogre::v1::OverlayContainer*>(
            overlayManager.createOverlayElement("Panel", "DebugPanel"));
        mDebugText = static_cast<Ogre::v1::TextAreaOverlayElement*>(
            overlayManager.createOverlayElement("TextArea", "DebugText"));
        mDebugText->setFontName("DebugFont");
        mDebugText->setCharHeight(0.025f);

        mDebugTextShadow = static_cast<Ogre::v1::TextAreaOverlayElement*>(
            overlayManager.createOverlayElement("TextArea", "0DebugTextShadow"));
        mDebugTextShadow->setFontName("DebugFont");
        mDebugTextShadow->setCharHeight(0.025f);
        mDebugTextShadow->setColour(Ogre::ColourValue::Black);
        mDebugTextShadow->setPosition(0.002f, 0.002f);

        panel->addChild(mDebugTextShadow);
        panel->addChild(mDebugText);
        overlay->add2D(panel);
        overlay->show();
    }
    //-----------------------------------------------------------------------------------
    void TutorialGameState::generateDebugText(float timeSinceLast, Ogre::String& outText)
    {
        
    }
    //-----------------------------------------------------------------------------------
    void TutorialGameState::update(float timeSinceLast)
    {
        if (mDisplayHelpMode != 0)
        {
            // Show FPS
            Ogre::String finalText;
            generateDebugText(timeSinceLast, finalText);
            mDebugText->setCaption(finalText);
            mDebugTextShadow->setCaption(finalText);
        }

        if (mCameraController)
            mCameraController->update(timeSinceLast);
    }
    //-----------------------------------------------------------------------------------
    void TutorialGameState::keyPressed(const SDL_KeyboardEvent& arg)
    {
        bool handledEvent = false;

        if (mCameraController)
            handledEvent = mCameraController->keyPressed(arg);

        if (!handledEvent)
            GameState::keyPressed(arg);
    }
    //-----------------------------------------------------------------------------------
    void TutorialGameState::keyReleased(const SDL_KeyboardEvent& arg)
    {
        if (arg.keysym.scancode == SDL_SCANCODE_F1 &&
            (arg.keysym.mod & ~(KMOD_NUM | KMOD_CAPS)) == 0)
        {
            mDisplayHelpMode = (mDisplayHelpMode + 1) % mNumDisplayHelpModes;

            Ogre::String finalText;
            generateDebugText(0, finalText);
            mDebugText->setCaption(finalText);
            mDebugTextShadow->setCaption(finalText);
        }
        else if (arg.keysym.scancode == SDL_SCANCODE_F1 &&
            (arg.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)))
        {
            // Hot reload of PBS shaders. We need to clear the microcode cache
            // to prevent using old compiled versions.
            Ogre::Root* root = mGraphicsSystem->getRoot();
            Ogre::HlmsManager* hlmsManager = root->getHlmsManager();

            Ogre::Hlms* hlms = hlmsManager->getHlms(Ogre::HLMS_PBS);
            Ogre::GpuProgramManager::getSingleton().clearMicrocodeCache();
            hlms->reloadFrom(hlms->getDataFolder());
        }
        else if (arg.keysym.scancode == SDL_SCANCODE_F2 &&
            (arg.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)))
        {
            // Hot reload of Unlit shaders.
            Ogre::Root* root = mGraphicsSystem->getRoot();
            Ogre::HlmsManager* hlmsManager = root->getHlmsManager();

            Ogre::Hlms* hlms = hlmsManager->getHlms(Ogre::HLMS_UNLIT);
            Ogre::GpuProgramManager::getSingleton().clearMicrocodeCache();
            hlms->reloadFrom(hlms->getDataFolder());
        }
        else if (arg.keysym.scancode == SDL_SCANCODE_F3 &&
            (arg.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)))
        {
            // Hot reload of Compute shaders.
            Ogre::Root* root = mGraphicsSystem->getRoot();
            Ogre::HlmsManager* hlmsManager = root->getHlmsManager();

            Ogre::Hlms* hlms = hlmsManager->getComputeHlms();
            Ogre::GpuProgramManager::getSingleton().clearMicrocodeCache();
            hlms->reloadFrom(hlms->getDataFolder());
        }
        else if (arg.keysym.scancode == SDL_SCANCODE_F5 &&
            (arg.keysym.mod & (KMOD_LCTRL | KMOD_RCTRL)))
        {
            // Force device reelection
            Ogre::Root* root = mGraphicsSystem->getRoot();
            root->getRenderSystem()->validateDevice(true);
        }
        else
        {
            bool handledEvent = false;

            if (mCameraController)
                handledEvent = mCameraController->keyReleased(arg);

            if (!handledEvent)
                GameState::keyReleased(arg);
        }
    }
    //-----------------------------------------------------------------------------------
    void TutorialGameState::mouseMoved(const SDL_Event& arg)
    {
        if (mCameraController)
            mCameraController->mouseMoved(arg);

        GameState::mouseMoved(arg);
    }
}  // namespace Demo
