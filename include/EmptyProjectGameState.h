

#ifndef _Demo_EmptyProjectGameState_H_
#define _Demo_EmptyProjectGameState_H_

#include <OgreAtmosphereNpr.h>
#include <OgreFrameListener.h>
#include <OgreSceneManager.h>

#include "TutorialGameState.h"
#include "GraphicsSystem.h"
#include "OgrePrerequisites.h"
#include "Engine/EditorWindows/EditorWindow.h"
#include "Engine/EditorWindows/WActorWindow.h"
#include "Engine/EditorWindows/WCameraSettings.h"
#include "Engine/EditorWindows/WMainMenuBar.h"

#include "Engine/GameState/CookieBaseState.h"
#include "Engine/Object/Actor/CActor.h"

namespace Demo
{
    class OgreNextImGuiGameState : public CEngine::TutorialGameState, public Ogre::FrameListener
    {
        Ogre::SceneNode* mSceneNode[17]{}; //before 16
        
        Ogre::SceneNode* mLightNodes[3]{};

#ifdef OGRE_BUILD_COMPONENT_ATMOSPHERE
        Ogre::AtmosphereNpr *mAtmosphere;
#endif
        //manipulate globals
        bool mAnimateObjects;
        int mCurrentPreset;
        Ogre::String mPresetName;
        float mExposure;
        float mMinAutoExposure;
        float mMaxAutoExposure;
        float mBloomFullThreshold;
        
        void switchPreset( int direction = 1 );
        void generateDebugText(float timeSinceLast, Ogre::String& outText) override;

        Ogre::Light* mainLight;
        
        Ogre::SceneManager* sceneManager;
        
    protected:
    
    public:
        OgreNextImGuiGameState(const Ogre::String& helpDescription);
        void createScene01() override;
        void destroyScene() override;
        void update(float timeSinceLast) override;
        void mouseMoved(const SDL_Event& arg) override;
        void mousePressed(const SDL_MouseButtonEvent& arg, Ogre::uint8 id) override;
        void mouseReleased(const SDL_MouseButtonEvent& arg, Ogre::uint8 id) override;
        void textInput(const SDL_TextInputEvent& arg) override;
        void keyPressed(const SDL_KeyboardEvent& arg) override;
        void keyReleased(const SDL_KeyboardEvent& arg) override;


    private:
        void _keyEvent(const SDL_KeyboardEvent& arg, bool keyPressed);
        
        static void SetImGuiStyle();
        void LightSetting();
        std::shared_ptr<CEngine::WCameraSettings> CameraSettings;
        std::shared_ptr<CEngine::WMainMenuBar> MainMenuBar;
        std::shared_ptr<CEngine::WActorWindow> ActorsWindowManager;
        
        std::shared_ptr<CEngine::CActor> testCActor;
        const char* lightPresets[6];

        SdlInputHandler* inputHandler;
        
    };

}

#endif
