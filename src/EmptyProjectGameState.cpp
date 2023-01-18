
#include "EmptyProjectGameState.h"

#include <iostream>
#include <OgreMesh.h>
#include <Engine/Camera/CCameraController.h>

#include "GraphicsSystem.h"
#include "ImguiManager.h"

#include "OgreItem.h"
#include "OgreSceneManager.h"

#include "OgreMesh2.h"
#include "OgreMeshManager.h"
#include "OgreMeshManager2.h"

#include "OgreHlmsPbs.h"
#include "OgreRoot.h"
#include "../Dependencies/imgui/backends/imgui_impl_opengl3.h"

#include "SdlInputHandler.h"
#include "Engine/EditorWindows/EditorWindow.h"
#include "Engine/EditorWindows/WCameraSettings.h"

using namespace Demo;

namespace Demo
{
    class ImguiFrameListener : public Ogre::FrameListener {
        bool frameRenderingQueued(const Ogre::FrameEvent& evt) {
            ImguiManager::getSingletonPtr()->render();
            return true;
        }
    };
    

    OgreNextImguiGameState::OgreNextImguiGameState(const Ogre::String& helpDescription) :
        TutorialGameState(helpDescription)
    {
        
    }
    //-----------------------------------------------------------------------------------
    /*                                  Global initalizers                              */

    //-----------------------------------------------------------------------------------
    void OgreNextImguiGameState::createScene01(void)
    {
        {
            // HdrUtils::init(mGraphicsSystem->getRenderWindow()->getSampleDescription().getColourSamples());

            sceneManager = mGraphicsSystem->getSceneManager();

            const float armsLength = 2.5f;

            Ogre::v1::MeshPtr planeMeshV1 = Ogre::v1::MeshManager::getSingleton().createPlane(
                "Plane v1", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME,
                Ogre::Plane(Ogre::Vector3::UNIT_Y, 1.0f), 50.0f, 50.0f, 1, 1, true, 1, 4.0f, 4.0f,
                Ogre::Vector3::UNIT_Z, Ogre::v1::HardwareBuffer::HBU_STATIC,
                Ogre::v1::HardwareBuffer::HBU_STATIC);

            Ogre::MeshPtr planeMesh = Ogre::MeshManager::getSingleton().createByImportingV1(
                "Plane", Ogre::ResourceGroupManager::DEFAULT_RESOURCE_GROUP_NAME, planeMeshV1.get(), true,
                true, true);

            {
                Ogre::Item* item = sceneManager->createItem(planeMesh, Ogre::SCENE_DYNAMIC);
                item->setDatablock("Marble");
                Ogre::SceneNode* sceneNode = sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)
                    ->createChildSceneNode(Ogre::SCENE_DYNAMIC);
                sceneNode->setPosition(0, -1, 0);
                sceneNode->attachObject(item);

                // // Change the addressing mode of the roughness map to wrap via code.
                // // Detail maps default to wrap, but the rest to clamp.
                // assert(dynamic_cast<Ogre::HlmsPbsDatablock*>(item->getSubItem(0)->getDatablock()));
                // Ogre::HlmsPbsDatablock* datablock =
                //     static_cast<Ogre::HlmsPbsDatablock*>(item->getSubItem(0)->getDatablock());
                // // Make a hard copy of the sampler block
                // Ogre::HlmsSamplerblock samplerblock(*datablock->getSamplerblock(Ogre::PBSM_ROUGHNESS));
                // samplerblock.mU = Ogre::TAM_WRAP;
                // samplerblock.mV = Ogre::TAM_WRAP;
                // samplerblock.mW = Ogre::TAM_WRAP;
                // // Set the new samplerblock. The Hlms system will
                // // automatically create the API block if necessary
                // datablock->setSamplerblock(Ogre::PBSM_ROUGHNESS, samplerblock);
            }

            for (int i = 0; i < 4; ++i)
            {
                for (int j = 0; j < 4; ++j)
                {
                    Ogre::String meshName;

                    if (i == j)
                        meshName = "Sphere1000.mesh";
                    else
                        meshName = "Cube_d.mesh";

                    Ogre::Item* item = sceneManager->createItem(
                        meshName, Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                        Ogre::SCENE_DYNAMIC);
                    if (i % 2 == 0)
                        item->setDatablock("Rocks");
                    else
                        item->setDatablock("Marble");

                    item->setVisibilityFlags(0x000000001);

                    const size_t idx = static_cast<size_t>(i * 4 + j);

                    mSceneNode[idx] = sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)
                        ->createChildSceneNode(Ogre::SCENE_DYNAMIC);

                    mSceneNode[idx]->setPosition((Ogre::Real(i) - 1.5f) * armsLength, 2.0f,
                        (Ogre::Real(j) - 1.5f) * armsLength);
                    mSceneNode[idx]->setScale(0.65f, 0.65f, 0.65f);

                    mSceneNode[idx]->roll(Ogre::Radian((Ogre::Real)idx));

                    mSceneNode[idx]->attachObject(item);
                }
            }

            Ogre::SceneNode* mainRootNode = sceneManager->getRootSceneNode();

            mainLight = sceneManager->createLight();
            Ogre::SceneNode* mainLightNode = mainRootNode->createChildSceneNode();

            // initLight();
            mainLightNode->attachObject(mainLight);
            mainLight->setPowerScale(97.0f);
            mainLight->setType(Ogre::Light::LT_DIRECTIONAL);
            mainLight->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());
            mainLight->setDirection( Ogre::Vector3( 0, -1, 0 ).normalisedCopy() );
            mainLight->setDirection( Ogre::Vector3( 0, -1, -0.5 ).normalisedCopy() );

            mLightNodes[0] = mainLightNode;

            // sceneManager->setAmbientLight(Ogre::ColourValue(0.3f, 0.5f, 0.7f) * 0.1f * 0.75f * 60.0f,
            //     Ogre::ColourValue(0.6f, 0.45f, 0.3f) * 0.065f * 0.75f * 60.0f,
            //     -mainLight->getDirection() + Ogre::Vector3::UNIT_Y * 0.2f);

            mainLight = sceneManager->createLight();
            mainLightNode = mainRootNode->createChildSceneNode();
            mainLightNode->attachObject(mainLight);
            mainLight->setDiffuseColour(0.8f, 0.4f, 0.2f);  // Warm
            mainLight->setSpecularColour(0.8f, 0.4f, 0.2f);
            mainLight->setType(Ogre::Light::LT_SPOTLIGHT);
            mainLightNode->setPosition(-10.0f, 10.0f, 10.0f);
            mainLight->setDirection(Ogre::Vector3(1, -1, -1).normalisedCopy());
            mainLight->setAttenuationBasedOnRadius(10.0f, 0.01f);
            //
            mLightNodes[1] = mainLightNode;
            
            mainLight = sceneManager->createLight();
            mainLightNode = mainRootNode->createChildSceneNode();
            mainLightNode->attachObject(mainLight);
            mainLight->setDiffuseColour(0.2f, 0.4f, 0.8f);  // Cold
            mainLight->setSpecularColour(0.2f, 0.4f, 0.8f);
            mainLight->setType(Ogre::Light::LT_SPOTLIGHT);
            mainLightNode->setPosition(10.0f, 10.0f, -10.0f);
            mainLight->setDirection(Ogre::Vector3(-1, -1, 1).normalisedCopy());
            mainLight->setAttenuationBasedOnRadius(10.0f, 0.01f);
            
            mLightNodes[2] = mainLightNode;
            /** Camera
                @note
                    Base camera controller.
                @remarks
                    Enable/Disable camera movement by bCameraMovable
            */
            // mCameraController = new CameraController(mGraphicsSystem, false);
            
            mCameraController = new CEngine::CCameraController(mGraphicsSystem, false);
            
            // EditorStyleWindow = new CEngine::EditorWindow("Sample");
            CameraSettings = std::make_shared<CEngine::WCameraSettings>("Camera Settings" ,mCameraController);
            MainMenuBar = std::make_shared<CEngine::WMainMenuBar>("Main Menu Bar");
        }

        //Register imgui for rendering
        // SetImGuiStyle();
        Ogre::Root::getSingleton().addFrameListener(new ImguiFrameListener());
        //initialise with your target workspace.
        ImguiManager::getSingleton().init(mGraphicsSystem->getCompositorWorkspace());

        SetImGuiStyle();
        
        //Setup imgui key codes for SDL2
        {
            ImGuiIO& io = ImGui::GetIO();
            
            io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;
            io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;
            io.BackendPlatformName = "imgui_impl_ogre_next_sdl";
            /*                          Docking                         */
            io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

            // Keyboard mapping. ImGui will use those indices to peek into the io.KeysDown[] array.
            io.KeyMap[ImGuiKey_Tab] = SDL_SCANCODE_TAB;
            io.KeyMap[ImGuiKey_LeftArrow] = SDL_SCANCODE_LEFT;
            io.KeyMap[ImGuiKey_RightArrow] = SDL_SCANCODE_RIGHT;
            io.KeyMap[ImGuiKey_UpArrow] = SDL_SCANCODE_UP;
            io.KeyMap[ImGuiKey_DownArrow] = SDL_SCANCODE_DOWN;
            io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;
            io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
            io.KeyMap[ImGuiKey_Home] = SDL_SCANCODE_HOME;
            io.KeyMap[ImGuiKey_End] = SDL_SCANCODE_END;
            io.KeyMap[ImGuiKey_Insert] = SDL_SCANCODE_INSERT;
            io.KeyMap[ImGuiKey_Delete] = SDL_SCANCODE_DELETE;
            io.KeyMap[ImGuiKey_Backspace] = SDL_SCANCODE_BACKSPACE;
            io.KeyMap[ImGuiKey_Space] = SDL_SCANCODE_SPACE;
            io.KeyMap[ImGuiKey_Enter] = SDL_SCANCODE_RETURN;
            io.KeyMap[ImGuiKey_Escape] = SDL_SCANCODE_ESCAPE;
            io.KeyMap[ImGuiKey_C] = SDL_SCANCODE_C;
            io.KeyMap[ImGuiKey_V] = SDL_SCANCODE_V;
            io.KeyMap[ImGuiKey_X] = SDL_SCANCODE_X;
            io.KeyMap[ImGuiKey_Y] = SDL_SCANCODE_Y;
            io.KeyMap[ImGuiKey_Z] = SDL_SCANCODE_Z;
            //movable
            io.KeyMap[ImGuiKey_A] = SDL_SCANCODE_A;
            io.KeyMap[ImGuiKey_W] = SDL_SCANCODE_W;
            io.KeyMap[ImGuiKey_D] = SDL_SCANCODE_D;
            io.KeyMap[ImGuiKey_S] = SDL_SCANCODE_S;
            io.KeyMap[ImGuiKey_Q] = SDL_SCANCODE_Q;
            io.KeyMap[ImGuiKey_E] = SDL_SCANCODE_E;
            io.KeyMap[ImGuiKey_LeftShift] = SDL_SCANCODE_LSHIFT; //boost
            io.KeyMap[ImGuiKey_PageDown] = SDL_SCANCODE_PAGEDOWN;
            io.KeyMap[ImGuiKey_PageUp] = SDL_SCANCODE_PAGEUP;

        }

        TutorialGameState::createScene01();
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImguiGameState::destroyScene()
    {
        ImguiManager::getSingleton().shutdown();
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImguiGameState::update(float timeSinceLast)
    {
        static bool tried = false;
        if (!tried)
        {
            SdlInputHandler* inputHandler = mGraphicsSystem->getInputHandler();
            inputHandler->setGrabMousePointer(false);
            inputHandler->setMouseVisible(true);
            inputHandler->setMouseRelative(false);
            tried = true;
        }

        //Calculate the frame delta time using SDL's helper functions.
        static Uint64 g_Time = 0;
        static Uint64 frequency = SDL_GetPerformanceFrequency();
        Uint64 current_time = SDL_GetPerformanceCounter();
        float deltaTime = g_Time > 0 ? (float)((double)(current_time - g_Time) / frequency) : (float)(1.0f / 60.0f);
        g_Time = current_time;
        
        ImguiManager::getSingletonPtr()->newFrame(deltaTime);

        //Begin issuing imgui draw calls.
        //Don't do this in the frameRenderingQueued callback,
        //as if any of your logic alters Ogre state you will break things.
        bool show_demo_window = true;
        ImGui::ShowDemoWindow(&show_demo_window);

        /*HERE UI IMPLEMENTATION*/
        LightSetting();
        CameraSettings->startUiRender();
        ImGui::ShowStyleEditor();
        ImGui::ShowDebugLogWindow();
        MainMenuBar->startUiRender();
        mCameraController->bMovableCamera = CameraSettings->bMoveCameraWhileHovered || (!ImGui::IsWindowHovered() && !ImGui::IsAnyItemHovered());
        
        TutorialGameState::update(timeSinceLast);
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImguiGameState::generateDebugText(float timeSinceLast, Ogre::String& outText)
    {
        TutorialGameState::generateDebugText(timeSinceLast, outText);
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImguiGameState::mouseMoved(const SDL_Event& arg)
    {
        ImGuiIO& io = ImGui::GetIO();
        if (arg.type == SDL_MOUSEMOTION)
        {
            Ogre::Vector2 mousePos(arg.motion.x, arg.motion.y);
            io.MousePos = ImVec2(mousePos.x, mousePos.y);
        }

        TutorialGameState::mouseMoved(arg);
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImguiGameState::mousePressed(const SDL_MouseButtonEvent& arg, Ogre::uint8 id)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[arg.button == SDL_BUTTON_LEFT ? 0 : 1] = true;
        
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImguiGameState::mouseReleased(const SDL_MouseButtonEvent& arg, Ogre::uint8 id)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[arg.button == SDL_BUTTON_LEFT ? 0 : 1] = false;
        
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImguiGameState::textInput(const SDL_TextInputEvent& arg)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharactersUTF8(arg.text);
    }
    void OgreNextImguiGameState::_keyEvent(const SDL_KeyboardEvent& arg, bool keyPressed)
    {
        ImGuiIO& io = ImGui::GetIO();

        int key = arg.keysym.scancode;
        IM_ASSERT(key >= 0 && key < IM_ARRAYSIZE(io.KeysDown));

        io.KeyShift = ((SDL_GetModState() & KMOD_SHIFT) != 0);
        io.KeyCtrl = ((SDL_GetModState() & KMOD_CTRL) != 0);
        io.KeyAlt = ((SDL_GetModState() & KMOD_ALT) != 0);
        io.KeySuper = ((SDL_GetModState() & KMOD_GUI) != 0);

        io.KeysDown[key] = keyPressed;
    }

    //-----------------------------------------------------------------------------------
    void OgreNextImguiGameState::keyPressed(const SDL_KeyboardEvent& arg)
    {
        _keyEvent(arg, true);

        TutorialGameState::keyPressed(arg);
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImguiGameState::keyReleased(const SDL_KeyboardEvent& arg)
    {
        _keyEvent(arg, false);

        TutorialGameState::keyReleased(arg);
    }
    //-----------------------------------------------------------------------------------

    void OgreNextImguiGameState::LightSetting()
    {
        //global illumination?
        
        ImGui::Begin("Light Settings");
        ImGuiStyle& style = ImGui::GetStyle();
        
        // if (ImGui::IsWindowHovered())
        // {
        //     style.Colors[ImGuiCol_WindowBg] = ImVec4(0.2f, 0.3f, 0.6f, 0.5f);
        //     style.Colors[ImGuiCol_Border] = ImVec4(0.00f, 0.00f, 0.00f, 0.39f);
        //     style.Colors[ImGuiCol_BorderShadow] = ImVec4(1.00f, 1.00f, 1.00f, 0.10f);
        //     style.Colors[ImGuiCol_FrameBg] = ImVec4(1.00f, 1.00f, 1.00f, 0.94f);
        //     style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.26f, 0.59f, 0.98f, 0.40f);
        // }
        // else
        // {
        //     style.Colors[ImGuiCol_WindowBg] = ImVec4(0.38f, 0.36f, 0.36f, 0.5f);
        //     style.Colors[ImGuiCol_Border] = ImVec4(0.38f, 0.38f, 0.38f, 0.1f);
        //     style.Colors[ImGuiCol_BorderShadow] = ImVec4(0.14f, 014.f, 0.14f, 0.1f);
        //     style.Colors[ImGuiCol_FrameBg] = ImVec4(0.f, 0.f, 0.f, 0.1f);
        // }

        ImGui::Text("Global Ambient Setting");
        ImGui::Text("Color");
        static float upperHemisphereColor[4] = { 0.3f, 0.5f, 0.7f, 0.7f };
        static float lowerHemisphereColor[4] = { 0.6f, 0.45f, 0.3f, 0.5f };

        ImGui::ColorEdit4("Upper Color", upperHemisphereColor);
        ImGui::ColorEdit4("Lower Color", lowerHemisphereColor);

        ImGui::Text("Direction");
        static float lightDirectionf[3] = { mainLight->getDirection().x, mainLight->getDirection().y, mainLight->getDirection().z};
        ImGui::SliderFloat3("Light Direction", lightDirectionf, -360.0f, 360.0f, "%1.0f");
        Ogre::Vector3 vec3f = { lightDirectionf[0], lightDirectionf[1], lightDirectionf[2] };

        sceneManager->setAmbientLight(Ogre::ColourValue(upperHemisphereColor[0], upperHemisphereColor[1], upperHemisphereColor[2], upperHemisphereColor[3])
            * 0.1f * 0.75f * 60.0f,
            Ogre::ColourValue(lowerHemisphereColor[0], lowerHemisphereColor[1], lowerHemisphereColor[2], lowerHemisphereColor[3])
            * 0.065f * 0.75f * 60.0f,
            -vec3f);

        ImGui::End();
        
    }

    inline void OgreNextImguiGameState::SetImGuiStyle()
    {
        /*
         * Style
         */
        
        ImGuiStyle& style = ImGui::GetStyle();
        //Rounding
        const float rounding = 9.f;
        style.WindowRounding = rounding;
        style.ChildRounding = rounding;
        style.FrameRounding = rounding;
        style.PopupRounding = rounding;
        style.ScrollbarRounding = rounding;
        style.GrabRounding = rounding;
        style.LogSliderDeadzone = rounding;
        style.TabRounding = rounding;
        style.FrameBorderSize = 1.f;
        style.WindowBorderSize = 0.f;
        style.PopupBorderSize = 0.f;

        //Colors
        ImVec4* colors = style.Colors;
        colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
        colors[ImGuiCol_TextDisabled]           = ImVec4(1.00f, 1.00f, 1.00f, 0.50f);
        colors[ImGuiCol_WindowBg]               = ImVec4(0.18f, 0.18f, 0.18f, 1.00f);
        colors[ImGuiCol_ChildBg]                = ImVec4(0.98f, 0.96f, 0.96f, 0.10f);
        colors[ImGuiCol_PopupBg]                = ImVec4(0.25f, 0.18f, 0.24f, 0.94f);
        colors[ImGuiCol_Border]                 = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_BorderShadow]           = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]                = ImVec4(0.09f, 0.09f, 0.09f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.00f, 0.00f, 0.00f, 0.77f);
        colors[ImGuiCol_FrameBgActive]          = ImVec4(0.02f, 0.02f, 0.02f, 1.00f);
        colors[ImGuiCol_TitleBg]                = ImVec4(0.04f, 0.04f, 0.04f, 1.00f);
        colors[ImGuiCol_TitleBgActive]          = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
        colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
        colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.31f, 0.31f, 0.31f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.41f, 0.41f, 0.41f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.51f, 0.51f, 0.51f, 1.00f);
        colors[ImGuiCol_CheckMark]              = ImVec4(0.98f, 0.97f, 0.44f, 1.00f);
        colors[ImGuiCol_SliderGrab]             = ImVec4(0.96f, 0.69f, 0.38f, 0.50f);
        colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.43f, 0.27f, 0.08f, 1.00f);
        colors[ImGuiCol_Button]                 = ImVec4(0.25f, 0.18f, 0.24f, 1.00f);
        colors[ImGuiCol_ButtonHovered]          = ImVec4(0.53f, 0.30f, 0.36f, 1.00f);
        colors[ImGuiCol_ButtonActive]           = ImVec4(0.53f, 0.30f, 0.36f, 0.54f);
        colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.33f);
        colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.51f);
        colors[ImGuiCol_HeaderActive]           = ImVec4(0.00f, 0.00f, 0.00f, 0.72f);
        colors[ImGuiCol_Separator]              = ImVec4(0.43f, 0.43f, 0.50f, 0.50f);
        colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive]        = ImVec4(0.10f, 0.40f, 0.75f, 1.00f);
        colors[ImGuiCol_ResizeGrip]             = ImVec4(0.00f, 0.00f, 0.00f, 0.20f);
        colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.00f, 0.00f, 0.00f, 0.67f);
        colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.72f, 0.72f, 0.72f, 0.94f);
        colors[ImGuiCol_Tab]                    = ImVec4(0.04f, 0.04f, 0.04f, 0.80f);
        colors[ImGuiCol_TabHovered]             = ImVec4(0.42f, 0.42f, 0.42f, 1.00f);
        colors[ImGuiCol_TabActive]              = ImVec4(0.42f, 0.42f, 0.42f, 0.50f);
        colors[ImGuiCol_TabUnfocused]           = ImVec4(1.00f, 1.00f, 1.00f, 0.04f);
        colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_DockingPreview]         = ImVec4(0.47f, 0.25f, 0.00f, 0.34f);
        colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.20f, 0.20f, 0.20f, 1.00f);
        colors[ImGuiCol_PlotLines]              = ImVec4(0.61f, 0.61f, 0.61f, 1.00f);
        colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.43f, 0.35f, 1.00f);
        colors[ImGuiCol_PlotHistogram]          = ImVec4(0.90f, 0.70f, 0.00f, 1.00f);
        colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.60f, 0.00f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]       = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.26f, 0.59f, 0.98f, 0.35f);
        colors[ImGuiCol_DragDropTarget]         = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]           = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
    }
    
}
