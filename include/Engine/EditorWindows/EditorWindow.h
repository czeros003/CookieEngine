#pragma once
namespace CEngine
{
	class EditorWindow
	{
		//todo style is Hovered logic AND camera movement
	public:
		EditorWindow(const char* windowName, bool bMoveCameraWhileHovered);
		EditorWindow(const char* windowName);

	private:
		const char* windowName;
		bool bMoveCameraWhileHovered;
		void initStyle();

	protected:
		virtual ~EditorWindow() = default;
		virtual void startUiRender();
	};
	
}

