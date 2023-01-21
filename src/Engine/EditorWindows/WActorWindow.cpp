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
            
            static char buf1[32];
            static char buf2[32];
            static char buf3[32];
            ImGui::InputText("Name", buf1, IM_ARRAYSIZE(buf1));
            ImGui::Text(buf1);
            ImGui::InputText("Mesh", buf2, IM_ARRAYSIZE(buf2));
            ImGui::InputText("Material", buf3, IM_ARRAYSIZE(buf3));

            if (ImGui::Button("Create"))
            {
                actors_map.insert({iterator+=iterator + 1, std::make_shared<CActor>("Test Actor", "Sphere1000.mesh", "Marble", mSceneNode)});
                actors_map.at(iterator)->CreateActor(sceneManager);
                
                // for (auto actor : actors_vec)
                // {
                //     actor->CreateActor();
                // }
                
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
