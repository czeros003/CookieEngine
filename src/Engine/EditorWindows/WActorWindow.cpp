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
        ImGui::Columns(2);

        static int item_current_idx = 0;
        if (ImGui::BeginListBox("Actors list"))
        {
            for (int n = 0; n < actors_map.size(); n++)
            {
                const bool is_selected = (item_current_idx == n);
                if (ImGui::Selectable(actors_map.at(n)->name.c_str(), is_selected))
                    item_current_idx = n;

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                {
                    iterator = n;
                    ImGui::SetItemDefaultFocus();
                }
                    
            }
            ImGui::EndListBox();
        }
        
        if (ImGui::Button("+ Add Actor"))
        {
            bPopUpAdd = true;
        }

        ImGui::NextColumn();
        ImGui::Text("Actor info:");
        ImGui::Spacing();
        ImGui::Separator();
        
        
        if (actors_map.size() != 0)
        {
            ImGui::Text("Name: ");
            ImGui::SameLine();
            ImGui::Text(actors_map.at(iterator)->name.c_str());

            ImGui::Text("Mesh Name: ");
            ImGui::SameLine();
            ImGui::Text(actors_map.at(iterator)->meshName.c_str());

            ImGui::Text("Texture: ");
            ImGui::SameLine();
            ImGui::Text(actors_map.at(iterator)->textureName.c_str());

            ImGui::Separator();
            ImGui::Text("Transform X Y Z");
            ImGui::Spacing();
            ImGui::Text("Location:");
            
            if (ImGui::SliderFloat3("##Location X-Y-Z", locations, -100, 100, "%1.0f"))
            {
                actors_map.at(iterator)->SetPosition(locations[0], locations[1], locations[2]);
            }

            ImGui::Text("Rotation:");
            if (ImGui::SliderFloat3("##Rotation X-Y-Z", rotations, -1, 1, "%1.0f"))
            {
                actors_map.at(iterator)->SetRotation(locations[0], locations[1], locations[2]);
            }

            ImGui::Text("Scale:");
            if (ImGui::SliderFloat3("##Scale X-Y-Z", scales, -100, 100, "%1.0f"))
            {
                actors_map.at(iterator)->SetRotation(scales[0], scales[1], scales[2]);
            }
        }
        else
        {
            ImGui::Text("Name: None");
            ImGui::Text("Mesh Name: None");
            ImGui::Text("Texture: None");
            ImGui::Spacing();
            ImGui::Text("None actor selected !");
        }

        
        ImGui::Columns(1);
        
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
