#pragma once
#include "CameraController.h"
#include "GraphicsSystem.h"

namespace CEngine
{
	class CCameraController 
	{
	private:
		bool mUseSceneNode;

	    bool  mSpeedMofifier;
	    bool  mWASD[4]{};
	    bool  mSlideUpDown[2]{};
	    float mCameraYaw;
	    float mCameraPitch;

	public:
	    float mCameraBaseSpeed;
	    float mCameraSpeedBoost;
	    bool bMovableCamera;

	private:
	    Demo::GraphicsSystem* mGraphicsSystem;

	public:
	    CCameraController(Demo::GraphicsSystem* graphicsSystem, bool useSceneNode = false);

		void update(float timeSinceLast);

	    /// Returns true if we've handled the event
	    bool keyPressed(const SDL_KeyboardEvent& arg);
	    /// Returns true if we've handled the event
	    bool keyReleased(const SDL_KeyboardEvent& arg);

	    void mouseMoved(const SDL_Event& arg);
	};

}