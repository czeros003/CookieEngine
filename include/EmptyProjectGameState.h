

#ifndef _Demo_EmptyProjectGameState_H_
#define _Demo_EmptyProjectGameState_H_

#include <OgreFrameListener.h>
#include <OgreSceneManager.h>

#include "TutorialGameState.h"
#include "GraphicsSystem.h"
#include "OgrePrerequisites.h"
#include "Engine/EditorWindows/EditorWindow.h"
#include "Engine/EditorWindows/WCameraSettings.h"

#include "Engine/GameState/CookieBaseState.h"

namespace Demo
{
    class OgreNextImguiGameState : public CEngine::TutorialGameState, public Ogre::FrameListener
    {
        virtual void generateDebugText(float timeSinceLast, Ogre::String& outText);

        Ogre::SceneNode* mSceneNode[16]{};

        Ogre::SceneNode* mLightNodes[3]{};

        Ogre::Light* mainLight;

        Ogre::SceneManager* sceneManager;

    public:
        OgreNextImguiGameState(const Ogre::String& helpDescription);

        virtual void createScene01(void);
        virtual void destroyScene();

        virtual void update(float timeSinceLast);

        virtual void mouseMoved(const SDL_Event& arg);
        virtual void mousePressed(const SDL_MouseButtonEvent& arg, Ogre::uint8 id);
        virtual void mouseReleased(const SDL_MouseButtonEvent& arg, Ogre::uint8 id);
        virtual void textInput(const SDL_TextInputEvent& arg);
        virtual void keyPressed(const SDL_KeyboardEvent& arg);
        virtual void keyReleased(const SDL_KeyboardEvent& arg);
        
        void LightSetting();

    private:
        void _keyEvent(const SDL_KeyboardEvent& arg, bool keyPressed);
        void SetImGuiStyle(bool bStyleDark, float alpha);
        
        CEngine::EditorWindow* ew;
        CEngine::WCameraSettings* ew1;
    };

}

#endif
