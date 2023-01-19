#include "Engine/Object/CObject.h"

namespace CEngine
{
    CObject::CObject(Ogre::String name) : objectName(std::move(name))
    {
    }
}
