#include "Engine/EditorWindows/WCameraSettings.h"

namespace CEngine
{
    WCameraSettings::WCameraSettings(CCameraController* mCameraController): EditorWindow(windowName),
        windowName("Camera Settings"),
        mCameraController(mCameraController)
    {
    }

    WCameraSettings::WCameraSettings(const char* windowName,
        bool bMoveCameraWhileHovered, CCameraController* mCameraController): EditorWindow(windowName, bMoveCameraWhileHovered),
        windowName(windowName),
        bMoveCameraWhileHovered(bMoveCameraWhileHovered),
        mCameraController(mCameraController)
    {
    }

    WCameraSettings::WCameraSettings(const char* windowName, CCameraController* mCameraController): EditorWindow(windowName),
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

        static bool bCameraMovable = true;

        ImGui::Begin(windowName);
        ImGui::Text("Basic Main Viewport Camera Settings.");
        ImGui::Checkbox("Enable Camera Movement While In UI", &bCameraMovable);

        mCameraController->bMovableCamera = bCameraMovable || !ImGui::IsWindowHovered();

        ImGui::Text("Camera Speed");
        ImGui::SameLine();
        ImGui::SliderFloat("##speed", &mCameraController->mCameraBaseSpeed, 1.0f, 100.0f, "%1.0f");
        

        ImGui::Text("Camera Boost");
        ImGui::SameLine();
        ImGui::SliderFloat("##boost", &mCameraController->mCameraSpeedBoost, 5.0f, 50.0f, "%1.0f");

        ImGui::End();
    }
}
