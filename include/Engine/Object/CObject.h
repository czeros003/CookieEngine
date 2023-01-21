#pragma once
#include <OgreMemoryTracker.h>
#include <OgrePrerequisites.h>

namespace CEngine
{
    //This is base class of object projection in game engine.
    //It maintains to hold base data, only derive from it or make child copy
    class CObject
    {
    public:
        explicit CObject(Ogre::String objectName);
        
        Ogre::String objectName;
    private:
        
    
    };
}
