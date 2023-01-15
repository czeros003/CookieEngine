#pragma once

#include "OgrePrerequisites.h"

#include "GameState.h"
#include "GraphicsSystem.h"
#include "Engine/Camera/CCameraController.h"

namespace Ogre
{
    namespace v1
    {
        class TextAreaOverlayElement;
    }
}  // namespace Ogre

namespace CEngine
{
    class Demo::GraphicsSystem;
    class CEngine::CCameraController;

    /// Base game state for the tutorials. All it does is show a little text on screen :)
    class TutorialGameState : public Demo::GameState
    {
    protected:
	    Demo::GraphicsSystem* mGraphicsSystem;

        /// Optional, for controlling the camera with WASD and the mouse
        CCameraController* mCameraController;

        Ogre::String mHelpDescription;
        Ogre::uint16 mDisplayHelpMode;
        Ogre::uint16 mNumDisplayHelpModes;

        Ogre::v1::TextAreaOverlayElement* mDebugText;
        Ogre::v1::TextAreaOverlayElement* mDebugTextShadow;

        virtual void createDebugTextOverlay();
        virtual void generateDebugText(float timeSinceLast, Ogre::String& outText);

    public:
        TutorialGameState(const Ogre::String& helpDescription);
        ~TutorialGameState() override;

        void _notifyGraphicsSystem(Demo::GraphicsSystem* graphicsSystem);

        void createScene01() override;

        void update(float timeSinceLast) override;

        void keyPressed(const SDL_KeyboardEvent& arg) override;
        void keyReleased(const SDL_KeyboardEvent& arg) override;

        void mouseMoved(const SDL_Event& arg) override;
    };
}  // namespace Demo
