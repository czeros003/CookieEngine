
#include "EmptyProjectGameState.h"
#include "GraphicsSystem.h"

#include "Compositor/OgreCompositorManager2.h"
#include "OgreCamera.h"
#include "OgreConfigFile.h"
#include "OgreRoot.h"
#include "OgreSceneManager.h"
#include "OgreWindow.h"

// Declares WinMain / main
#include "MainEntryPointHelper.h"
#include "System/MainEntryPoints.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX
#    include <errno.h>
#    include <pwd.h>
#    include <sys/stat.h>
#    include <sys/types.h>
#    include <unistd.h>
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#    include "shlobj.h"
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#    include <Foundation/Foundation.h>
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#    include "OSX/macUtils.h"
#endif
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
#    include "iOS/macUtils.h"
#endif
#include "../Dependencies/imgui/imgui.h"
#include "../Dependencies/imgui/backends/imgui_impl_sdl.h"
#include "../Dependencies/imgui/backends/imgui_impl_opengl3.h"
#include "EmptyProject.h"

#include <OgreArchiveManager.h>
#include <OgreHlmsManager.h>
#include <OgreHlmsPbs.h>
#include <OgreHlmsUnlit.h>

#include "../Dependencies/imgui/backends/imgui_impl_opengl3_loader.h"

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
INT WINAPI WinMainApp( HINSTANCE hInst, HINSTANCE hPrevInstance, LPSTR strCmdLine, INT nCmdShow )
#else
int mainApp( int argc, const char *argv[] )
#endif
{
    return Demo::MainEntryPoints::mainAppSingleThreaded( DEMO_MAIN_ENTRY_PARAMS );
}

namespace Demo
{

    class CookieEngineSystem final : public GraphicsSystem
    {
        Ogre::CompositorWorkspace* setupCompositor() override
        {
            Ogre::CompositorManager2* compositorManager = mRoot->getCompositorManager2();
            //add
            Ogre::RenderSystem *renderSystem = mRoot->getRenderSystem();
            const Ogre::RenderSystemCapabilities *caps = renderSystem->getCapabilities();
            //add

            Ogre::String workspaceName("CookieEngineWorkspace");
            if (!compositorManager->hasWorkspaceDefinition(workspaceName)) {
                compositorManager->createBasicWorkspaceDef(workspaceName, Ogre::ColourValue(0.2f, 0.4f, 0.6f, 1.f), Ogre::IdString());
            }
            
            // if( mRenderWindow->isMultisample() &&
            //     caps->hasCapability( Ogre::RSC_EXPLICIT_FSAA_RESOLVE ) )
            //         workspaceName = "HdrWorkspaceMsaa";

            return compositorManager->addWorkspace( mSceneManager, mRenderWindow->getTexture(), mCamera, workspaceName, true );
        }

        void registerHlms() override
        {
            Ogre::ConfigFile cf;
            cf.load(mResourcePath + "resources2.cfg");

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
            Ogre::String rootHlmsFolder = Ogre::macBundlePath() + '/' +
                cf.getSetting("DoNotUseAsResource", "Hlms", "");
#else
            Ogre::String rootHlmsFolder = mResourcePath + cf.getSetting("DoNotUseAsResource", "Hlms", "");
#endif

            if (rootHlmsFolder.empty())
                rootHlmsFolder = "./";
            else if (*(rootHlmsFolder.end() - 1) != '/')
                rootHlmsFolder += "/";

            //At this point rootHlmsFolder should be a valid path to the Hlms data folder

            Ogre::HlmsUnlit* hlmsUnlit = 0;
            Ogre::HlmsPbs* hlmsPbs = 0;

            //For retrieval of the paths to the different folders needed
            Ogre::String mainFolderPath;
            Ogre::StringVector libraryFoldersPaths;
            Ogre::StringVector::const_iterator libraryFolderPathIt;
            Ogre::StringVector::const_iterator libraryFolderPathEn;

            Ogre::ArchiveManager& archiveManager = Ogre::ArchiveManager::getSingleton();

            {
                //Create & Register HlmsUnlit
                //Get the path to all the subdirectories used by HlmsUnlit
                Ogre::HlmsUnlit::getDefaultPaths(mainFolderPath, libraryFoldersPaths);
                Ogre::Archive* archiveUnlit = archiveManager.load(rootHlmsFolder + mainFolderPath,
                    "FileSystem", true);
                Ogre::ArchiveVec archiveUnlitLibraryFolders;
                libraryFolderPathIt = libraryFoldersPaths.begin();
                libraryFolderPathEn = libraryFoldersPaths.end();
                while (libraryFolderPathIt != libraryFolderPathEn)
                {
                    Ogre::Archive* archiveLibrary =
                        archiveManager.load(rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true);
                    archiveUnlitLibraryFolders.push_back(archiveLibrary);
                    ++libraryFolderPathIt;
                }

                //Create and register the unlit Hlms
                hlmsUnlit = OGRE_NEW Ogre::HlmsUnlit(archiveUnlit, &archiveUnlitLibraryFolders);
                Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsUnlit);
            }

            {
                //Create & Register HlmsPbs
                //Do the same for HlmsPbs:
                Ogre::HlmsPbs::getDefaultPaths(mainFolderPath, libraryFoldersPaths);
                Ogre::Archive* archivePbs = archiveManager.load(rootHlmsFolder + mainFolderPath,
                    "FileSystem", true);

                //Get the library archive(s)
                Ogre::ArchiveVec archivePbsLibraryFolders;
                libraryFolderPathIt = libraryFoldersPaths.begin();
                libraryFolderPathEn = libraryFoldersPaths.end();
                while (libraryFolderPathIt != libraryFolderPathEn)
                {
                    Ogre::Archive* archiveLibrary =
                        archiveManager.load(rootHlmsFolder + *libraryFolderPathIt, "FileSystem", true);
                    archivePbsLibraryFolders.push_back(archiveLibrary);
                    ++libraryFolderPathIt;
                }

                //Create and register
                hlmsPbs = OGRE_NEW Ogre::HlmsPbs(archivePbs, &archivePbsLibraryFolders);
                Ogre::Root::getSingleton().getHlmsManager()->registerHlms(hlmsPbs);
            }


            Ogre::RenderSystem* renderSystem = mRoot->getRenderSystem();
            if (renderSystem->getName() == "Direct3D11 Rendering Subsystem")
            {
                //Set lower limits 512kb instead of the default 4MB per Hlms in D3D 11.0
                //and below to avoid saturating AMD's discard limit (8MB) or
                //saturate the PCIE bus in some low end machines.
                bool supportsNoOverwriteOnTextureBuffers;
                renderSystem->getCustomAttribute("MapNoOverwriteOnDynamicBufferSRV",
                    &supportsNoOverwriteOnTextureBuffers);

                if (!supportsNoOverwriteOnTextureBuffers)
                {
                    hlmsPbs->setTextureBufferDefaultSize(512 * 1024);
                    hlmsUnlit->setTextureBufferDefaultSize(512 * 1024);
                }
            }

#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            //Now setup is complete enough to allow permanent addition of the /Data/ path.
            //Ideally this would have been there from the beginning,
            //but this is difficult with reading in specific resources2.cfg files.
            mResourcePath += "/Data/";
#endif
        }

        void setupResources() override
        {
            Ogre::String dataPath;
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            dataPath = mResourcePath + "/Data/";
#else
            dataPath = "../Data/";
#endif

            // Ogre::CompositorPassColibriGuiProvider *compoProvider =
            //         OGRE_NEW Ogre::CompositorPassColibriGuiProvider( colibriManager );
            // Ogre::CompositorManager2 *compositorManager = mRoot->getCompositorManager2();
            // compositorManager->setCompositorPassProvider( compoProvider );

            GraphicsSystem::setupResources();

            Ogre::ConfigFile cf;
            cf.load(mResourcePath + "resources2.cfg");
            // std::cout << mResourcePath << "path xd";

            Ogre::String originalDataFolder = cf.getSetting("DoNotUseAsResource", "Hlms", "");

            if (originalDataFolder.empty())
                originalDataFolder = "./";
            else if (*(originalDataFolder.end() - 1) != '/')
                originalDataFolder += "/";

            // originalDataFolder += "2.0/scripts/materials/PbsMaterials";
            //
            // addResourceLocation(originalDataFolder, getMediaReadArchiveType(), "General");
            const char *c_locations[11] = {
                "2.0/scripts/materials/Tutorial_SMAA",
                "2.0/scripts/materials/Tutorial_SMAA/GLSL",
                "2.0/scripts/materials/Tutorial_SMAA/HLSL",
                "2.0/scripts/materials/Tutorial_SMAA/Metal",
                "2.0/scripts/materials/Tutorial_SMAA/Vulkan",
                "2.0/scripts/materials/HDR",
                "2.0/scripts/materials/HDR/GLSL",
                "2.0/scripts/materials/HDR/HLSL",
                "2.0/scripts/materials/HDR/Metal",
                "2.0/scripts/materials/PbsMaterials",
                "2.0/scripts/materials/HDR_SMAA",
            };
            
            for( size_t i = 0; i < 11; ++i )
            {
                Ogre::String dataFolder = originalDataFolder + c_locations[i];
                addResourceLocation( dataFolder, getMediaReadArchiveType(), "General" );
            }
        }

    public:
        CookieEngineSystem(GameState* gameState) : GraphicsSystem(gameState)
        {
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            //mResourcePath = Ogre::macBundlePath() + "/Contents/Resources/Data/";
            //This can't be /Data/ for some of the early setup.
            //This is changed after that setup.
            mResourcePath = Ogre::macBundlePath() + "/Contents/Resources/";
#else
            mResourcePath = "../Data/";
#endif
            mAlwaysAskForConfig = false;

            //It's recommended that you set this path to:
            //    %APPDATA%/ColibriGui/ on Windows
            //    ~/.config/ColibriGui/ on Linux
            //    macCachePath() + "/ColibriGui/" (NSCachesDirectory) on Apple -> Important because
            //    on iOS your app could be rejected from App Store when they see iCloud
            //    trying to backup your Ogre.log & ogre.cfg auto-generated without user
            //    intervention. Also convenient because these settings will be deleted
            //    if the user removes cached data from the app, so the settings will be
            //    reset.
            //  Obviously you can replace "ColibriGui" by your app's name.
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            mWriteAccessFolder = +"/";
            TCHAR path[MAX_PATH];
            if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_APPDATA, NULL,
                SHGFP_TYPE_CURRENT, path) != S_OK))
            {
                //Need to convert to OEM codepage so that fstream can
                //use it properly on international systems.
#if defined(_UNICODE) || defined(UNICODE)
                int size_needed = WideCharToMultiByte(CP_OEMCP, 0, path, (int)wcslen(path),
                    NULL, 0, NULL, NULL);
                mWriteAccessFolder = std::string(size_needed, 0);
                WideCharToMultiByte(CP_OEMCP, 0, path, (int)wcslen(path),
                    &mWriteAccessFolder[0], size_needed, NULL, NULL);
#else
                TCHAR oemPath[MAX_PATH];
                CharToOem(path, oemPath);
                mWriteAccessFolder = std::string(oemPath);
#endif
                mWriteAccessFolder += "/CookieEngine/";

                //Attempt to create directory where config files go
                if (!CreateDirectoryA(mWriteAccessFolder.c_str(), NULL) &&
                    GetLastError() != ERROR_ALREADY_EXISTS)
                {
                    //Couldn't create directory (no write access?),
                    //fall back to current working dir
                    mWriteAccessFolder = "";
                }
            }
#elif OGRE_PLATFORM == OGRE_PLATFORM_LINUX
            const char* homeDir = getenv("HOME");
            if (homeDir == 0)
                homeDir = getpwuid(getuid())->pw_dir;
            mWriteAccessFolder = homeDir;
            mWriteAccessFolder += "/.config";
            int result = mkdir(mWriteAccessFolder.c_str(), S_IRWXU | S_IRWXG);
            int errorReason = errno;

            //Create "~/.config"
            if (result && errorReason != EEXIST)
            {
                printf("Error. Failing to create path '%s'. Do you have access rights?",
                    mWriteAccessFolder.c_str());
                mWriteAccessFolder = "";
            }
            else
            {
                //Create "~/.config/ColibriGui"
                mWriteAccessFolder += "/CookieEngine/";
                result = mkdir(mWriteAccessFolder.c_str(), S_IRWXU | S_IRWXG);
                errorReason = errno;

                if (result && errorReason != EEXIST)
                {
                    printf("Error. Failing to create path '%s'. Do you have access rights?",
                        mWriteAccessFolder.c_str());
                    mWriteAccessFolder = "";
                }
            }
#elif OGRE_PLATFORM == OGRE_PLATFORM_APPLE || OGRE_PLATFORM == OGRE_PLATFORM_APPLE_IOS
            mWriteAccessFolder = "/tmp/imguiOgre/";
            //Create "pathToCache/ColibriGui"
            int result = mkdir(mWriteAccessFolder.c_str(), S_IRWXU | S_IRWXG);
            int errorReason = errno;

            if (result && errorReason != EEXIST)
            {
                printf("Error. Failing to create path '%s'. Do you have access rights?",
                    mWriteAccessFolder.c_str());
                mWriteAccessFolder = "";
            }
#endif
        }
            };

    void MainEntryPoints::createSystems(GameState** outGraphicsGameState,
        GraphicsSystem** outGraphicsSystem,
        GameState** outLogicGameState,
        LogicSystem** outLogicSystem)
    {
        OgreNextImGuiGameState* gfxGameState = new OgreNextImGuiGameState(
            "Cookie Engine");

        GraphicsSystem* graphicsSystem = new CookieEngineSystem(gfxGameState);

        gfxGameState->_notifyGraphicsSystem(graphicsSystem);

        *outGraphicsGameState = gfxGameState;
        *outGraphicsSystem = graphicsSystem;
    }

    void MainEntryPoints::destroySystems(GameState* graphicsGameState,
        GraphicsSystem* graphicsSystem,
        GameState* logicGameState,
        LogicSystem* logicSystem)
    {
        delete graphicsSystem;
        delete graphicsGameState;
    }

    const char* MainEntryPoints::getWindowTitle()
    {
        return "Cookie Engine";
    }
}
