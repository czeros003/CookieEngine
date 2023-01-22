#include "Engine/EditorWindows/WActorWindow.h"

namespace CEngine
{
    WActorWindow::WActorWindow(const char* windowName, Ogre::SceneNode* mSceneNode, Ogre::SceneManager* sceneManager):
        EditorWindow(windowName),
        windowName(windowName),
        mSceneNode(mSceneNode),
        sceneManager(sceneManager),
        bPopUpAdd(false),
        iterator(0)
    {
    }

    void WActorWindow::startUiRender()
    {
        EditorWindow::startUiRender();

        ImGui::Begin(windowName);
        
        if (ImGui::BeginListBox("Actors list"))
        {
            for (int n = 0; n < actors_map.size(); n++)
            {
                const bool is_selected = (iterator == n);
                if (ImGui::Selectable(actors_map.at(n)->name.c_str(), is_selected))
                    iterator = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                    ImGui::SetItemDefaultFocus();
            }
            ImGui::EndListBox();
        }

        
        if (ImGui::Button("+ Add Actor"))
        {
            bPopUpAdd = true;
        }
        ImGui::End();

        if (bPopUpAdd)
        {
            ImGui::OpenPopup("Actor creator", ImGuiPopupFlags_None);
            bPopUpAdd = !bPopUpAdd;
        }
        
        bool p_open = true;
        if (ImGui::BeginPopupModal("Actor creator", &p_open, ImGuiWindowFlags_None))
        {
            ImGui::Text("Actor creator");
            ImGui::Spacing();
            
            static char bufName[32];
            static char bufMeshName[32];
            static char bufTextureName[32];
            ImGui::InputText("Name", bufName, IM_ARRAYSIZE(bufName));
            ImGui::InputText("Mesh", bufMeshName, IM_ARRAYSIZE(bufMeshName));
            ImGui::InputText("Material", bufTextureName, IM_ARRAYSIZE(bufTextureName));

            if (ImGui::Button("Create"))
            {
                actors_map.insert({actors_map.size(), std::make_shared<CActor>(bufName, bufMeshName, bufTextureName, mSceneNode)});
                actors_map.at(actors_map.size()-1)->CreateActor(sceneManager);
                
                ImGui::CloseCurrentPopup();
            }
            
            ImGui::EndPopup();
        }
    }

    WActorWindow::~WActorWindow()
    {
        
    }

    void WActorWindow::DeleteActor(int sceneNodeIndex)
    {
        
    }

    bool operator==(const Ogre::SceneNode& lhs, int rhs);

    void WActorWindow::AddActor()
    {
        actors_vec.push_back(std::make_shared<CActor>(actorName, meshName, textureName, mSceneNode));
    }
}
