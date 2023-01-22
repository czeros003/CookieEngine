#include "Engine/Object/Actor/CActor.h"

#include <OgreItem.h>
#include <OgreSceneManager.h>

namespace CEngine
{
    CActor::CActor(const Ogre::String& name, const Ogre::String& mesh_name, const Ogre::String& texture_name, Ogre::SceneNode* m_scene_node)
        : CObject(name),
          name(name),
          meshName(mesh_name),
          textureName(texture_name),
          mSceneNode(m_scene_node)
    {
        
    }

    void CActor::CreateActor()
    {
    }

    void CActor::CreateActor(Ogre::SceneManager* sceneManager)
    {
        Ogre::Item* item = sceneManager->createItem(meshName, //creating from mesh name
            Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME, Ogre::SCENE_DYNAMIC);

        if (!textureName.empty())
        {
            item->setDatablock(textureName);
        }

        item->setVisibilityFlags(0x000000001);

        mSceneNode = sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)
                        ->createChildSceneNode(Ogre::SCENE_DYNAMIC);
        
        mSceneNode->setPosition(0.f, 0.f, 10.f);
        mSceneNode->setScale(1.0f, 1.0f, 1.0f);
        
        mSceneNode->attachObject(item);
    }

    void CActor::SetPosition(float X, float Y, float Z)
    {
        mSceneNode->setPosition(X, Y, Z);
    }

    void CActor::SetScale(float X, float Y, float Z)
    {
        mSceneNode->setScale(X, Y, Z);
    }

    void CActor::SetRotation(float X, float Y, float Z)
    {
        mSceneNode->translate(X, Y, Z);
    }
}
