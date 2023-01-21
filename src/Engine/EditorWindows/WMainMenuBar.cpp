#include "Engine/EditorWindows/WMainMenuBar.h"

#include <cstdlib>

namespace CEngine
{
    WMainMenuBar::WMainMenuBar(const char* windowName, bool bRotate)
        : EditorWindow(windowName, bRotate), bRotate(bRotate), windowName(windowName)
    {
        bExitPopUp = false;
    }

    WMainMenuBar::WMainMenuBar(const char* windowName)
        : EditorWindow(windowName), bRotate(true), windowName(windowName)
    {
        bExitPopUp = false;
    }

    WMainMenuBar::~WMainMenuBar() 
    {
        // delete this;
    }

    void WMainMenuBar::startUiRender()
    {
        EditorWindow::startUiRender();

        if (ImGui::BeginMainMenuBar())
        {
            if (ImGui::BeginMenu("File"))
            {
                if (ImGui::MenuItem("New")) {}
                if (ImGui::MenuItem("Open", "Ctrl+O")) {}
                if (ImGui::BeginMenu("Open Recent"))
                {
                    ImGui::MenuItem("fish_hat.c");
                    ImGui::MenuItem("fish_hat.inl");
                    ImGui::MenuItem("fish_hat.h");
                    if (ImGui::BeginMenu("More.."))
                    {
                        ImGui::MenuItem("Hello");
                        ImGui::MenuItem("Sailor");
                        if (ImGui::BeginMenu("Recurse.."))
                        {
                            
                            ImGui::EndMenu();
                        }
                        ImGui::EndMenu();
                    }
                    ImGui::EndMenu();
                }
                if (ImGui::MenuItem("Save", "Ctrl+S")) {}
                if (ImGui::MenuItem("Save As..")) {}

                ImGui::Separator();
                if (ImGui::BeginMenu("Options"))
                {
                    static bool enabled = true;
                    ImGui::MenuItem("Enabled", "", &enabled);
                    ImGui::BeginChild("child", ImVec2(0, 60), true);
                    for (int i = 0; i < 10; i++)
                        ImGui::Text("Scrolling Text %d", i);
                    ImGui::EndChild();
                    static float f = 0.5f;
                    static int n = 0;
                    ImGui::SliderFloat("Value", &f, 0.0f, 1.0f);
                    ImGui::InputFloat("Input", &f, 0.1f);
                    ImGui::Combo("Combo", &n, "Yes\0No\0Maybe\0\0");
                    ImGui::EndMenu();
                }

                
                if (ImGui::BeginMenu("Colors"))
                {
                    float sz = ImGui::GetTextLineHeight();
                    for (int i = 0; i < ImGuiCol_COUNT; i++)
                    {
                        const char* name = ImGui::GetStyleColorName((ImGuiCol)i);
                        ImVec2 p = ImGui::GetCursorScreenPos();
                        ImGui::GetWindowDrawList()->AddRectFilled(p, ImVec2(p.x + sz, p.y + sz), ImGui::GetColorU32((ImGuiCol)i));
                        ImGui::Dummy(ImVec2(sz, sz));
                        ImGui::SameLine();
                        ImGui::MenuItem(name);
                    }
                    ImGui::EndMenu();
                }

                // Here we demonstrate appending again to the "Options" menu (which we already created above)
                // Of course in this demo it is a little bit silly that this function calls BeginMenu("Options") twice.
                // In a real code-base using it would make senses to use this feature from very different code locations.
                if (ImGui::BeginMenu("Options")) // <-- Append!
                {
                    
                    static bool b = true;
                    ImGui::Checkbox("SomeOption", &b);
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Disabled", false)) // Disabled
                {
                    IM_ASSERT(0);
                }
                if (ImGui::MenuItem("Checked", NULL, true)) {}
                
                if (ImGui::MenuItem("Quit", "Alt+F4"))
                {
                    bExitPopUp = true;
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Edit"))
            {
                if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
                if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
                ImGui::Separator();
                if (ImGui::MenuItem("Cut", "CTRL+X")) {}
                if (ImGui::MenuItem("Copy", "CTRL+C")) {}
                if (ImGui::MenuItem("Paste", "CTRL+V")) {}
                ImGui::EndMenu();
            }

            if (ImGui::BeginMenu("Tools"))
            {
                //todo create adding docking option, link other editor ui settings here 
                ImGui::EndMenu();
            }
            
            
            if (ImGui::BeginMenu("Options"))
            {
                ImGui::Checkbox("Rotate objects",&bRotate);
            
                ImGui::EndMenu();
            }
            
            ImGui::EndMainMenuBar();

            //Exit pop up logic
            ExitWindowPopUp();
        }
    }

    void WMainMenuBar::ExitWindowPopUp()
    {
        if(bExitPopUp)
        {
            ImGui::PushStyleColor(ImGuiCol_PopupBg, (ImVec4)ImColor::HSV(0.0f, 0.74f, 0.17f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_MenuBarBg, (ImVec4)ImColor::HSV(0.0f, 0.74f, 0.17f,1.f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0,0,0,1));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, (ImVec4)ImColor::HSV(0.0f, 0.0f, 0.19f,0.5f));
            ImGui::PushStyleColor(ImGuiCol_ModalWindowDimBg, (ImVec4)ImColor::HSV(0.0f, 0.f, 0.f,0.0f));
            
            ImGui::OpenPopup("Exit", ImGuiPopupFlags_MouseButtonLeft);
            this->bExitPopUp = !bExitPopUp;
        }

        bool p_open = true;
        if (ImGui::BeginPopupModal("Exit", &p_open, ImGuiWindowFlags_NoResize + ImGuiWindowFlags_NoMove))
        {
            ImGui::Text("Do you want to exit?");
            ImGui::Spacing();
            ImGui::Text("All data will be lost.");
            ImGui::Separator();
            ImGui::Spacing();
            ImGui::Columns(2);
            if (ImGui::Button("No", ImVec2(ImGui::GetWindowSize().x*0.4f, 0.0f)))
            {
                ImGui::CloseCurrentPopup();
                ImGui::PopStyleColor(5);
            }
            ImGui::NextColumn();
            if (ImGui::Button("Yes", ImVec2(ImGui::GetWindowSize().x*0.4f, 0.0f)))
            {
                exit(0);
            }
            ImGui::Columns(1);
            ImGui::EndPopup();
        }
    }
}
