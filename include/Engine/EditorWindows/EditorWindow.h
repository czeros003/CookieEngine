#pragma once
namespace CEngine
{
	class EditorWindow
	{
		//todo style is Hovered logic AND camera movement
	public:
		EditorWindow(const char* windowName, bool bMoveCameraWhileHovered);
		EditorWindow(const char* windowName);

		// virtual void startUiRender();

	private:
		const char* windowName;
		bool bMoveCameraWhileHovered;
		void init();

	protected:
		virtual ~EditorWindow() = default;
		virtual void startUiRender();
	};
	
}

