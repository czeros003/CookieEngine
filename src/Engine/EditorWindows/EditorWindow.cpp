#include "Engine/EditorWindows/EditorWindow.h"

#include <imgui/imgui.h>

#include "Engine/Camera/CCameraController.h"

namespace CEngine
{
	EditorWindow::EditorWindow(const char *windowName, bool bMoveCameraWhileHovered) :
	windowName(windowName),
	bMoveCameraWhileHovered(bMoveCameraWhileHovered)
	{
		initStyle();
	}

	EditorWindow::EditorWindow(const char *windowName) :
	windowName(windowName),
	bMoveCameraWhileHovered(false)
	{
		initStyle();
	}

	void EditorWindow::initStyle()
	{
		
	}
	
	void EditorWindow::startUiRender()
	{
		// if (ImGui::IsWindowHovered())
		// {
		// 	ImGui::PopStyleColor(1);
		// 	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::HSV(0.0f, 0.74f, 0.17f, 1.0f));
		// }
		// else
		// {
		// 	ImGui::PopStyleColor(1);
		// 	ImGui::PushStyleColor(ImGuiCol_WindowBg, (ImVec4)ImColor::HSV(0.0f, 0.74f, 0.17f, 0.0f));
		// }
		
	}
}
