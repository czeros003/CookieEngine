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
		// ImGui::ShowStyleEditor();
		// if (ImGui::IsWindowHovered())
		// {
		// 	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
		// }
		// else
		// {
		// 	ImGui::GetStyle().Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.3f, 0.6f, 0.0f);
		// }
		
	}
}
