#pragma once
#include <OgreMemoryTracker.h>
#include <OgreSceneNode.h>

#include "EditorWindow.h"
#include "Engine/Object/Actor/CActor.h"
#include <map>

namespace CEngine
{
    class WActorWindow : EditorWindow
    {
    public:
        WActorWindow(const char* windowName, Ogre::SceneNode* mSceneNode, Ogre::SceneManager* sceneManager);
        
        explicit WActorWindow(const char* windowName);
        
        void startUiRender() override;

        ~WActorWindow() override;
    protected:
    private:
        const char* windowName;
        Ogre::SceneNode* mSceneNode; //max 30 actors, temporary no need for bigger values -> just create another window instead
        Ogre::SceneManager* sceneManager;
        

        void DeleteActor(int sceneNodeIndex);
        void AddActor();
        
        std::string actorName;
        std::string meshName;
        std::string textureName;
        bool bPopUpAdd;
        int iterator;

        std::map<int, std::shared_ptr<CActor>> actors_map;
        std::vector<std::shared_ptr<CActor>> actors_vec;

        float locations[3];
        float rotations[3];
        float scales[3];
        
    };
}

