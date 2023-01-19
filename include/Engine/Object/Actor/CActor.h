#pragma once
#include "Engine/Object/CObject.h"


namespace CEngine
{
    class CActor : CObject
    {
    public:
        explicit CActor(const Ogre::String& objectName);

        CActor(const Ogre::String& name, const Ogre::String& mesh_name, const Ogre::String& texture_name, Ogre::SceneNode* m_scene_node);


        void CreateActor();
        void CreateActor(Ogre::SceneManager* sceneManager);
        
        void SetPosition(float X, float Y, float Z);
        void SetScale(float X, float Y, float Z);
        void SetRotation(float X, float Y, float Z);
        
    
    private:
        Ogre::String meshName;
        Ogre::String textureName;
        Ogre::SceneNode* mSceneNode;
        // int sceneIndex;
    
    };
}
