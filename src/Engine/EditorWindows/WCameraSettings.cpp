#include "Engine/EditorWindows/WCameraSettings.h"

namespace CEngine
{
    WCameraSettings::WCameraSettings(CCameraController* mCameraController): EditorWindow(windowName),
        bMoveCameraWhileHovered(false),
        windowName("Camera Settings"),
        mCameraController(mCameraController)
    {
    }

    WCameraSettings::WCameraSettings(const char* windowName,
        bool bMoveCameraWhileHovered, CCameraController* mCameraController): EditorWindow(windowName, bMoveCameraWhileHovered),
        bMoveCameraWhileHovered(bMoveCameraWhileHovered),
        windowName(windowName),
        mCameraController(mCameraController)
    {
    }

    WCameraSettings::WCameraSettings(const char* windowName, CCameraController* mCameraController):
        EditorWindow(windowName), bMoveCameraWhileHovered(false),
        windowName(windowName),
        mCameraController(mCameraController)
    {
    }

    WCameraSettings::~WCameraSettings()
    {
        delete mCameraController;
        mCameraController = 0;
    }

    void WCameraSettings::startUiRender()
    {
        EditorWindow::startUiRender();
        
        ImGui::Begin("Camera Settings");
        ImGui::Text("Basic Main Viewport Camera Settings.");
        ImGui::Checkbox("Enable Camera Movement While In UI", &bMoveCameraWhileHovered);

        // mCameraController->bMovableCamera = bMoveCameraWhileHovered || !ImGui::IsWindowHovered();

        ImGui::Text("Camera Speed");
        ImGui::SameLine();
        ImGui::SliderFloat("##speed", &mCameraController->mCameraBaseSpeed, 1.0f, 100.0f, "%1.0f");
        

        ImGui::Text("Camera Boost");
        ImGui::SameLine();
        ImGui::SliderFloat("##boost", &mCameraController->mCameraSpeedBoost, 5.0f, 50.0f, "%1.0f");

        ImGui::End();
    }
}
