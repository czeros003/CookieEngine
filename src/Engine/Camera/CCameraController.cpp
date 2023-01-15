#include "Engine/Camera/CCameraController.h"

#include <OgreCamera.h>
#include <OgreWindow.h>
#include <imgui/imgui.h>

namespace CEngine
{
	CCameraController::CCameraController(Demo::GraphicsSystem* graphicsSystem, bool useSceneNode) :
        mUseSceneNode(useSceneNode),
        mSpeedMofifier(false),
        mCameraYaw(0),
        mCameraPitch(0),
        mCameraBaseSpeed(10),
        mCameraSpeedBoost(5),
        mGraphicsSystem(graphicsSystem)
    {
        memset(mWASD, 0, sizeof(mWASD));
        memset(mSlideUpDown, 0, sizeof(mSlideUpDown));
        bMovableCamera = true;
    }

    //-----------------------------------------------------------------------------------
    void CEngine::CCameraController::update(float timeSinceLast)
    {
        if (!bMovableCamera)
        {
            return;
        }

        Ogre::Camera* camera = mGraphicsSystem->getCamera();

        if (mCameraYaw != 0.0f || mCameraPitch != 0.0f)
        {
            if (mUseSceneNode)
            {
                Ogre::Node* cameraNode = camera->getParentNode();

                // Update now as yaw needs the derived orientation.
                cameraNode->_getFullTransformUpdated();
                cameraNode->yaw(Ogre::Radian(mCameraYaw), Ogre::Node::TS_WORLD);
                cameraNode->pitch(Ogre::Radian(mCameraPitch));
            }
            else
            {
                camera->yaw(Ogre::Radian(mCameraYaw));
                camera->pitch(Ogre::Radian(mCameraPitch));
            }

            mCameraYaw = 0.0f;
            mCameraPitch = 0.0f;
        }

        int camMovementZ = mWASD[2] - mWASD[0];
        int camMovementX = mWASD[3] - mWASD[1];
        int slideUpDown = mSlideUpDown[0] - mSlideUpDown[1];

        if (camMovementZ || camMovementX || slideUpDown)
        {
            Ogre::Vector3 camMovementDir((Ogre::Real)camMovementX, (Ogre::Real)slideUpDown,
                (Ogre::Real)camMovementZ);
            camMovementDir.normalise();
            camMovementDir *=
                timeSinceLast * mCameraBaseSpeed * (1 + mSpeedMofifier * mCameraSpeedBoost);

            if (mUseSceneNode)
            {
                Ogre::Node* cameraNode = camera->getParentNode();
                cameraNode->translate(camMovementDir, Ogre::Node::TS_LOCAL);
            }
            else
            {
                camera->moveRelative(camMovementDir);
            }
        }
    }
    //-----------------------------------------------------------------------------------
    bool CEngine::CCameraController::keyPressed(const SDL_KeyboardEvent& arg)
    {
        if (arg.keysym.scancode == SDL_SCANCODE_LSHIFT)
            mSpeedMofifier = true;

        if (arg.keysym.scancode == SDL_SCANCODE_W)
            mWASD[0] = true;
        else if (arg.keysym.scancode == SDL_SCANCODE_A)
            mWASD[1] = true;
        else if (arg.keysym.scancode == SDL_SCANCODE_S)
            mWASD[2] = true;
        else if (arg.keysym.scancode == SDL_SCANCODE_D)
            mWASD[3] = true;
        else if (arg.keysym.scancode == SDL_SCANCODE_E)
            mSlideUpDown[0] = true;
        else if (arg.keysym.scancode == SDL_SCANCODE_Q)
            mSlideUpDown[1] = true;
        else
            return false;

        return true;
    }
    //-----------------------------------------------------------------------------------
    bool CEngine::CCameraController::keyReleased(const SDL_KeyboardEvent& arg)
    {
        if (arg.keysym.scancode == SDL_SCANCODE_LSHIFT)
            mSpeedMofifier = false;

        if (arg.keysym.scancode == SDL_SCANCODE_W)
            mWASD[0] = false;
        else if (arg.keysym.scancode == SDL_SCANCODE_A)
            mWASD[1] = false;
        else if (arg.keysym.scancode == SDL_SCANCODE_S)
            mWASD[2] = false;
        else if (arg.keysym.scancode == SDL_SCANCODE_D)
            mWASD[3] = false;
        else if (arg.keysym.scancode == SDL_SCANCODE_E)
            mSlideUpDown[0] = false;
        else if (arg.keysym.scancode == SDL_SCANCODE_Q)
            mSlideUpDown[1] = false;
        else
            return false;

        return true;
    }
    //-----------------------------------------------------------------------------------
    void CEngine::CCameraController::mouseMoved(const SDL_Event& arg)
    {
        if (!bMovableCamera)
        {
            return;
        }

        ImGuiIO& io = ImGui::GetIO();
        if (io.MouseDown[1])
        {
            float width = static_cast<float>(mGraphicsSystem->getRenderWindow()->getWidth());
            float height = static_cast<float>(mGraphicsSystem->getRenderWindow()->getHeight());

            mCameraYaw += float(-arg.motion.xrel) / width;
            mCameraPitch += float(-arg.motion.yrel) / height;
        }

    }
}