#include "Engine/EditorWindows/EditorWindow.h"

#include <imgui/imgui.h>

namespace CEngine
{
	CEngine::EditorWindow::EditorWindow(const char *windowName, bool bMoveCameraWhileHovered) :
	windowName(windowName),
	bMoveCameraWhileHovered(bMoveCameraWhileHovered)
	{
		
	}

	CEngine::EditorWindow::EditorWindow(const char *windowName) :
	windowName(windowName),
	bMoveCameraWhileHovered(false)
	{
		
	}

	void EditorWindow::init()
	{
		
	}

	
	void EditorWindow::startUiRender()
	{
		ImGuiStyle& style = ImGui::GetStyle();

		ImGui::Begin(windowName);
		if (ImGui::IsWindowHovered())
		{
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.3f, 0.6f, 0.5f);
		}
		else
		{
			style.Colors[ImGuiCol_WindowBg] = ImVec4(0.38f, 0.36f, 0.36f, 0.5f);
		}

		ImGui::End();
	}
}
