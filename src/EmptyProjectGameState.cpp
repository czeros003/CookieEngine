
#include "EmptyProjectGameState.h"

#include <iostream>
#include <OgreMesh.h>
#include <OgreWindow.h>
#include <Engine/Camera/CCameraController.h>

#include "GraphicsSystem.h"
#include "ImguiManager.h"

#include "OgreItem.h"
#include "OgreSceneManager.h"

#include "OgreMeshManager.h"
#include "OgreMeshManager2.h"

#include "OgreHlmsPbs.h"
#include "OgreRoot.h"

#include "SdlInputHandler.h"
#include "Engine/EditorWindows/EditorWindow.h"
#include "Engine/EditorWindows/WCameraSettings.h"
#include "Utils/HdrUtils.h"

#include "OgreHlmsPbsDatablock.h"
#include "OgreHlmsSamplerblock.h"

#include "OgreHlmsManager.h"
#include "OgreTextureFilters.h"
#include "OgreTextureGpuManager.h"

using namespace Demo;

namespace Demo
{
    class ImguiFrameListener : public Ogre::FrameListener {
        bool frameRenderingQueued(const Ogre::FrameEvent& evt) {
            ImguiManager::getSingletonPtr()->render();
            return true;
        }
    };


    OgreNextImGuiGameState::OgreNextImGuiGameState(const Ogre::String& helpDescription) :
        TutorialGameState(helpDescription),
#ifdef OGRE_BUILD_COMPONENT_ATMOSPHERE
        mAtmosphere( 0 ),
#endif
        mAnimateObjects(true),
        mCurrentPreset( std::numeric_limits<Ogre::uint32>::max() ),
        mExposure( 0.0f ),
        mMinAutoExposure( -2.5f ),
        mMaxAutoExposure( 2.5f ),
        mBloomFullThreshold( 5.0f ),
        mainLight(nullptr),
        sceneManager(nullptr),
        EditorStyleWindow(nullptr)
    {
    }

    void OgreNextImGuiGameState::switchPreset(int direction)
    {
        struct Preset
        {
            const char *name{};
            Ogre::ColourValue skyColour;
            Ogre::ColourValue ambUpperHemisphere;
            Ogre::ColourValue ambLowerHemisphere;
            float lightPower[3]{};
            float exposure{};
            float minAutoExposure{};
            float maxAutoExposure{};
            float bloomThreshold{};
            float envmapScale{};
        };

        const Preset c_presets[] = {
            {
                "Bright, sunny day",
                Ogre::ColourValue( 0.2f, 0.4f, 0.6f ) * 60.0f,  // Sky
                Ogre::ColourValue( 0.3f, 0.50f, 0.7f ) * 4.5f,
                Ogre::ColourValue( 0.6f, 0.45f, 0.3f ) * 2.925f,
                {
                    97.0f,      // Sun power
                    1.5f, 1.5f  // Lights
                },
                0.0f,   // Exposure
                -1.0f,  // Exposure
                2.5f,   // Exposure
                5.0f,   // Bloom
                16.0f   // Env. map scale
            },
            {
                "Average, slightly hazy day",
                Ogre::ColourValue( 0.2f, 0.4f, 0.6f ) * 32.0f,  // Sky
                Ogre::ColourValue( 0.3f, 0.50f, 0.7f ) * 3.15f,
                Ogre::ColourValue( 0.6f, 0.45f, 0.3f ) * 2.0475f,
                {
                    48.0f,      // Sun power
                    1.5f, 1.5f  // Lights
                },
                0.0f,   // Exposure
                -2.0f,  // Exposure
                2.5f,   // Exposure
                5.0f,   // Bloom
                8.0f    // Env. map scale
            },
            {
                "Heavy overcast day",
                Ogre::ColourValue( 0.4f, 0.4f, 0.4f ) * 4.5f,  // Sky
                Ogre::ColourValue( 0.5f, 0.5f, 0.5f ) * 0.4f,
                Ogre::ColourValue( 0.5f, 0.5f, 0.5f ) * 0.365625f,
                {
                    6.0625f,    // Sun power
                    1.5f, 1.5f  // Lights
                },
                0.0f,   // Exposure
                -2.5f,  // Exposure
                1.0f,   // Exposure
                5.0f,   // Bloom
                0.5f    // Env. map scale
            },
            {
                "Gibbous moon night",
                Ogre::ColourValue( 0.27f, 0.3f, 0.6f ) * 0.01831072f,  // Sky
                Ogre::ColourValue( 0.5f, 0.5f, 0.50f ) * 0.003f,
                Ogre::ColourValue( 0.4f, 0.5f, 0.65f ) * 0.00274222f,
                {
                    0.0009251f,  // Sun power
                    1.5f, 1.5f   // Lights
                },
                0.65f,            // Exposure
                -2.5f,            // Exposure
                3.0f,             // Exposure
                5.0f,             // Bloom
                0.0152587890625f  // Env. map scale
            },
            {
                "Gibbous moon night w/ powerful spotlights",
                Ogre::ColourValue( 0.27f, 0.3f, 0.6f ) * 0.01831072f,  // Sky
                Ogre::ColourValue( 0.5f, 0.5f, 0.50f ) * 0.003f,
                Ogre::ColourValue( 0.4f, 0.5f, 0.65f ) * 0.00274222f,
                {
                    0.0009251f,  // Sun power
                    6.5f, 6.5f   // Lights
                },
                0.65f,            // Exposure
                -2.5f,            // Exposure
                3.0f,             // Exposure
                5.0f,             // Bloom
                0.0152587890625f  // Env. map scale
            },
            {
                "JJ Abrams style",
                Ogre::ColourValue( 0.2f, 0.4f, 0.6f ) * 6.0f,  // Sky
                Ogre::ColourValue( 0.3f, 0.50f, 0.7f ) * 0.1125f,
                Ogre::ColourValue( 0.6f, 0.45f, 0.3f ) * 0.073125f,
                {
                    4.0f,           // Sun power
                    17.05f, 17.05f  // Lights
                },
                0.5f,  // Exposure
                1.0f,  // Exposure
                2.5f,  // Exposure
                3.0f,  // Bloom
                1.0f,  // Env. map scale
            },
        };

        {
            const Ogre::uint32 numPresets = sizeof( c_presets ) / sizeof( c_presets[0] );

            if( direction >= 0 )
                mCurrentPreset = ( mCurrentPreset + 1 ) % numPresets;
            else
                mCurrentPreset = ( mCurrentPreset + numPresets - 1 ) % numPresets;
        }

        const Preset &preset = c_presets[mCurrentPreset];

        mPresetName = preset.name;
        mExposure = preset.exposure;
        mMinAutoExposure = preset.minAutoExposure;
        mMaxAutoExposure = preset.maxAutoExposure;
        mBloomFullThreshold = preset.bloomThreshold;

        HdrUtils::setSkyColour( preset.skyColour, 1.0f, mGraphicsSystem->getCompositorWorkspace() );
        HdrUtils::setExposure( mExposure, mMinAutoExposure, mMaxAutoExposure );
        HdrUtils::setBloomThreshold( std::max( mBloomFullThreshold - 2.0f, 0.0f ),
                                     std::max( mBloomFullThreshold, 0.01f ) );
        for( int i = 0; i < 3; ++i )
        {
            assert( dynamic_cast<Ogre::Light *>( mLightNodes[i]->getAttachedObject( 0 ) ) );
            Ogre::Light *light = static_cast<Ogre::Light *>( mLightNodes[i]->getAttachedObject( 0 ) );
            light->setPowerScale( preset.lightPower[i] );
        }

        Ogre::SceneManager* sceneManager = mGraphicsSystem->getSceneManager();  // NOLINT(clang-diagnostic-shadow)
        sceneManager->setAmbientLight( preset.ambLowerHemisphere, preset.ambUpperHemisphere,
                                       sceneManager->getAmbientLightHemisphereDir(),
                                       preset.envmapScale );

#ifdef OGRE_BUILD_COMPONENT_ATMOSPHERE
        if( !mAtmosphere )
        {
            OGRE_ASSERT_HIGH( dynamic_cast<Ogre::Light *>( mLightNodes[0]->getAttachedObject( 0u ) ) );
            mGraphicsSystem->createAtmosphere(
                static_cast<Ogre::Light *>( mLightNodes[0]->getAttachedObject( 0u ) ) );
            OGRE_ASSERT_HIGH( dynamic_cast<Ogre::AtmosphereNpr *>( sceneManager->getAtmosphere() ) );
            mAtmosphere = static_cast<Ogre::AtmosphereNpr *>( sceneManager->getAtmosphere() );
        }

        Ogre::AtmosphereNpr::Preset atmoPreset;
        atmoPreset.sunPower = preset.lightPower[0];
        atmoPreset.skyPower = preset.skyColour.toVector3().collapseMax() * 2.0f;
        atmoPreset.skyColour = preset.skyColour.toVector3() / atmoPreset.skyPower;
        atmoPreset.linkedLightPower = preset.lightPower[0];
        atmoPreset.envmapScale = preset.envmapScale;

        // The colour we pass to Atmosphere is too greenish when displayed. Correct it a bit.
        atmoPreset.skyColour *= Ogre::Vector3( 1.0f, 0.9f, 1.0f );

        atmoPreset.linkedSceneAmbientUpperPower *= 50.0f;
        atmoPreset.linkedSceneAmbientLowerPower *= 50.0f;
        if( mCurrentPreset == 2u )
        {
            // Heavy overcast day
            atmoPreset.densityCoeff = 0.25f;
            atmoPreset.densityDiffusion = 0.25f;
            atmoPreset.linkedSceneAmbientUpperPower *= 0.5f;
            atmoPreset.fogDensity = 0.025f;
        }
        else if( mCurrentPreset == 3u || mCurrentPreset == 4u )
        {
            // Gibbous moon night series. Night always requires quite the parameter changes
            atmoPreset.sunPower = 0.25f;
            atmoPreset.skyPower = 0.07f;
            atmoPreset.densityCoeff = 0.08f;
            atmoPreset.linkedSceneAmbientUpperPower *= 0.0025f;
            atmoPreset.linkedSceneAmbientLowerPower *= 0.0025f;
        }
        else if( mCurrentPreset == 5u )
        {
            atmoPreset.densityCoeff = 0.38f;
            atmoPreset.linkedSceneAmbientUpperPower *= 0.5f;
            atmoPreset.linkedSceneAmbientLowerPower *= 0.02f;
        }
        else
        {
            atmoPreset.linkedSceneAmbientUpperPower *= 8.0f;
            atmoPreset.linkedSceneAmbientLowerPower *= 2.0f;
        }
        mAtmosphere->setPreset( atmoPreset );
#endif
    }
    
    //-----------------------------------------------------------------------------------
    /*                                  Global initalizers                              */

    //-----------------------------------------------------------------------------------
    void OgreNextImGuiGameState::createScene01()
    {
        {
            HdrUtils::init(mGraphicsSystem->getRenderWindow()->getSampleDescription().getColourSamples());

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

                // Change the addressing mode of the roughness map to wrap via code.
                // Detail maps default to wrap, but the rest to clamp.
                if (true)
                {
                    assert( dynamic_cast<Ogre::HlmsPbsDatablock *>( item->getSubItem( 0 )->getDatablock() ) );
                    Ogre::HlmsPbsDatablock *datablock =
                        static_cast<Ogre::HlmsPbsDatablock *>( item->getSubItem( 0 )->getDatablock() );
                    // Make a hard copy of the sampler block
                    Ogre::HlmsSamplerblock samplerblock( *datablock->getSamplerblock( Ogre::PBSM_ROUGHNESS ) );
                    samplerblock.mU = Ogre::TAM_WRAP;
                    samplerblock.mV = Ogre::TAM_WRAP;
                    samplerblock.mW = Ogre::TAM_WRAP;
                    // Set the new samplerblock. The Hlms system will
                    // automatically create the API block if necessary
                    datablock->setSamplerblock( Ogre::PBSM_ROUGHNESS, samplerblock );
                }
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

                    const size_t idx = static_cast<size_t>(i * 4 + j);  // NOLINT(bugprone-misplaced-widening-cast)

                    mSceneNode[idx] = sceneManager->getRootSceneNode(Ogre::SCENE_DYNAMIC)
                        ->createChildSceneNode(Ogre::SCENE_DYNAMIC);

                    mSceneNode[idx]->setPosition((Ogre::Real(i) - 1.5f) * armsLength, 2.0f,
                        (Ogre::Real(j) - 1.5f) * armsLength);
                    mSceneNode[idx]->setScale(0.65f, 0.65f, 0.65f);

                    mSceneNode[idx]->roll(Ogre::Radian((Ogre::Real)idx));

                    mSceneNode[idx]->attachObject(item);
                }
            }

            if (true)
            {
                {
                    size_t numItems = 0;
                    Ogre::HlmsManager *hlmsManager = mGraphicsSystem->getRoot()->getHlmsManager();
            
                    assert( dynamic_cast<Ogre::HlmsPbs *>( hlmsManager->getHlms( Ogre::HLMS_PBS ) ) );
            
                    Ogre::HlmsPbs *hlmsPbs =
                        static_cast<Ogre::HlmsPbs *>( hlmsManager->getHlms( Ogre::HLMS_PBS ) );
            
                    const int numX = 8;
                    const int numZ = 8;
            
                    const float armsLengthSphere = 1.0f;
                    const float startX = ( numX - 1 ) / 2.0f;
                    const float startZ = ( numZ - 1 ) / 2.0f;
            
                    Ogre::Root *root = mGraphicsSystem->getRoot();
                    Ogre::TextureGpuManager *textureMgr = root->getRenderSystem()->getTextureGpuManager();
            
                    for( int x = 0; x < numX; ++x )
                    {
                        for( int z = 0; z < numZ; ++z )
                        {
                            Ogre::String datablockName = "Test" + Ogre::StringConverter::toString( numItems++ );
                            Ogre::HlmsPbsDatablock *datablock = static_cast<Ogre::HlmsPbsDatablock *>(
                                hlmsPbs->createDatablock( datablockName, datablockName, Ogre::HlmsMacroblock(),
                                                          Ogre::HlmsBlendblock(), Ogre::HlmsParamVec() ) );
            
                            Ogre::TextureGpu *texture = textureMgr->createOrRetrieveTexture(
                                "SaintPetersBasilica.dds", Ogre::GpuPageOutStrategy::Discard,
                                Ogre::TextureFlags::PrefersLoadingFromFileAsSRGB, Ogre::TextureTypes::TypeCube,
                                Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                                Ogre::TextureFilter::TypeGenerateDefaultMipmaps );
            
                            datablock->setTexture( Ogre::PBSM_REFLECTION, texture );
                            datablock->setDiffuse( Ogre::Vector3( 0.0f, 1.0f, 0.0f ) );
            
                            datablock->setRoughness(
                                std::max( 0.02f, float( x ) / std::max( 1.0f, (float)( numX - 1 ) ) ) );
                            datablock->setFresnel(
                                Ogre::Vector3( float( z ) / std::max( 1.0f, (float)( numZ - 1 ) ) ), false );
            
                            Ogre::Item *item = sceneManager->createItem(
                                "Sphere1000.mesh", Ogre::ResourceGroupManager::AUTODETECT_RESOURCE_GROUP_NAME,
                                Ogre::SCENE_DYNAMIC );
                            item->setDatablock( datablock );
                            item->setVisibilityFlags( 0x000000002 );
            
                            Ogre::SceneNode *sceneNode = sceneManager->getRootSceneNode( Ogre::SCENE_DYNAMIC )
                                                                     ->createChildSceneNode( Ogre::SCENE_DYNAMIC );
                            sceneNode->setPosition(
                                Ogre::Vector3( armsLengthSphere * Ogre::Real( x ) - startX, 1.0f,
                                               armsLengthSphere * Ogre::Real( z ) - startZ ) );
                            sceneNode->attachObject( item );
                        }
                    }
                }
            }

            Ogre::SceneNode* mainRootNode = sceneManager->getRootSceneNode();


            //Create light
            mainLight = sceneManager->createLight();
            Ogre::SceneNode* mainLightNode = mainRootNode->createChildSceneNode();

            // initLight();
            mainLightNode->attachObject(mainLight);
            mainLight->setPowerScale(97.0f);
            mainLight->setType(Ogre::Light::LT_DIRECTIONAL);
            mainLight->setDirection(Ogre::Vector3(-1, -1, -1).normalisedCopy());
            // mainLight->setDirection( Ogre::Vector3( 0, -1, 0 ).normalisedCopy() );
            // mainLight->setDirection( Ogre::Vector3( 0, -1, -0.5 ).normalisedCopy() );

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
            switchPreset(3);
            
            // EditorStyleWindow = new CEngine::EditorWindow("Sample");
            CameraSettings = std::make_shared<CEngine::WCameraSettings>("Camera Settings" ,mCameraController);
            MainMenuBar = std::make_shared<CEngine::WMainMenuBar>("Main Menu Bar");
        }

        //Register imgui for rendering
        
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
    void OgreNextImGuiGameState::destroyScene()
    {
        ImguiManager::getSingleton().shutdown();
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImGuiGameState::update(float timeSinceLast)
    {

        if( mAnimateObjects )
        {
            for( int i = 0; i < 16; ++i )
                mSceneNode[i]->yaw( Ogre::Radian( timeSinceLast * float( i ) * 0.125f ) );
        }

        TutorialGameState::update( timeSinceLast );
        
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
    void OgreNextImGuiGameState::generateDebugText(float timeSinceLast, Ogre::String& outText)
    {
        TutorialGameState::generateDebugText(timeSinceLast, outText);

        if( mDisplayHelpMode == 2 )
        {
            outText += "\nHold SHIFT to decrease values\n[SPACE] Preset: ";
            outText += mPresetName;
            outText += "\nF5 Exposure = ";
            outText += Ogre::StringConverter::toString( mExposure );
            outText += "\nF6 Min Auto Exposure = ";
            outText += Ogre::StringConverter::toString( mMinAutoExposure );
            outText += "\nF7 Max Auto Exposure = ";
            outText += Ogre::StringConverter::toString( mMaxAutoExposure );
            outText += "\nF8 Bloom Threshold = ";
            outText += Ogre::StringConverter::toString( mBloomFullThreshold );
        }
        else if( mDisplayHelpMode == 1 )
        {
            Ogre::uint32 visibilityMask = mGraphicsSystem->getSceneManager()->getVisibilityMask();

            outText += "\nPress F2 to toggle animation. ";
            outText += mAnimateObjects ? "[On]" : "[Off]";
            outText += "\nPress F3 to show/hide animated objects. ";
            outText += ( visibilityMask & 0x000000001 ) ? "[On]" : "[Off]";
            outText += "\nPress F4 to show/hide palette of spheres. ";
            outText += ( visibilityMask & 0x000000002 ) ? "[On]" : "[Off]";
        }
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImGuiGameState::mouseMoved(const SDL_Event& arg)
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
    void OgreNextImGuiGameState::mousePressed(const SDL_MouseButtonEvent& arg, Ogre::uint8 id)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[arg.button == SDL_BUTTON_LEFT ? 0 : 1] = true;
        
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImGuiGameState::mouseReleased(const SDL_MouseButtonEvent& arg, Ogre::uint8 id)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.MouseDown[arg.button == SDL_BUTTON_LEFT ? 0 : 1] = false;
        
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImGuiGameState::textInput(const SDL_TextInputEvent& arg)
    {
        ImGuiIO& io = ImGui::GetIO();
        io.AddInputCharactersUTF8(arg.text);
    }
    void OgreNextImGuiGameState::_keyEvent(const SDL_KeyboardEvent& arg, bool keyPressed)
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
    void OgreNextImGuiGameState::keyPressed(const SDL_KeyboardEvent& arg)
    {
        _keyEvent(arg, true);

        TutorialGameState::keyPressed(arg);
    }
    //-----------------------------------------------------------------------------------
    void OgreNextImGuiGameState::keyReleased(const SDL_KeyboardEvent& arg)
    {
        _keyEvent(arg, false);

        TutorialGameState::keyReleased(arg);
    }
    //-----------------------------------------------------------------------------------

    void OgreNextImGuiGameState::LightSetting()
    {
        ImGui::Begin("Global Light Settings");
        
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


        static int currentItem = 1;
        const char* lightPresets[6] = {"Bright, sunny day", "Average, slightly hazy day", "Heavy overcast day", "Gibbous moon night", "Gibbous moon night w/ powerful spotlights", "JJ Abrams style"};
        const char* combo_preview_value = lightPresets[currentItem];
        // if (ImGui::ListBox("Preset", &currentItem, lightPresets, IM_ARRAYSIZE(lightPresets), 1))
        // {
        //     switchPreset(currentItem);
        //     ImGui::EndListBox();
        // }

        if (ImGui::BeginCombo("Preset", combo_preview_value, 0))
        {
            for (int n = 0; n < IM_ARRAYSIZE(lightPresets); n++)
            {
                const bool is_selected = (currentItem == n);
                if (ImGui::Selectable(lightPresets[n], is_selected))
                {
                    currentItem = n;
                    switchPreset(currentItem);
                }

                // Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
                if (is_selected)
                {
                    ImGui::SetItemDefaultFocus();
                    // switchPreset(currentItem);
                }
            }
            ImGui::EndCombo();
        }
        
        ImGui::End();
        
    }

    inline void OgreNextImGuiGameState::SetImGuiStyle()
    {
        /* Style */
        
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
