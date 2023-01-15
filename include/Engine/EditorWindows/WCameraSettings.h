#pragma once
#include "EditorWindow.h"
#include <imgui/imgui.h>

#include "Engine/Camera/CCameraController.h"

namespace CEngine
{
    class WCameraSettings : EditorWindow
    {
    public:
        WCameraSettings(CCameraController* mCameraController);
        WCameraSettings(const char* windowName, bool bMoveCameraWhileHovered, CCameraController* mCameraController);
        
        WCameraSettings(const char* windowName, CCameraController* mCameraController);
        
        ~WCameraSettings() override;

        void startUiRender() override;

    private:
        const char* windowName;
        bool bMoveCameraWhileHovered;
        CCameraController* mCameraController;
        
    };
}

