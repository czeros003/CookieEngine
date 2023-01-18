#pragma once
#include "EditorWindow.h"

namespace CEngine
{
    class WMainMenuBar : EditorWindow
    {
    public:
        WMainMenuBar(const char* windowName, bool bMoveCameraWhileHovered);
        explicit WMainMenuBar(const char* windowName);

        void startUiRender() override;
        ~WMainMenuBar() override;
    
    private:
        const char* windowName;
        bool bExitPopUp;
        void ExitWindowPopUp();
    protected:
    };
}

