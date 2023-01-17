#pragma once
#include <imgui/imgui.h>

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
		
		Demo::GraphicsSystem* mGraphicsSystem;

	public:
	    float mCameraBaseSpeed;
	    float mCameraSpeedBoost;
	    bool bMovableCamera;

	    CCameraController(Demo::GraphicsSystem* graphicsSystem, bool useSceneNode = false);

		void update(float timeSinceLast);
	    bool keyPressed(const SDL_KeyboardEvent& arg);
	    bool keyReleased(const SDL_KeyboardEvent& arg);
	    void mouseMoved(const SDL_Event& arg);
	};

}