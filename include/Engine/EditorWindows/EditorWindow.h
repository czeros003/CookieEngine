#pragma once
namespace CEngine
{
	class EditorWindow
	{
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

