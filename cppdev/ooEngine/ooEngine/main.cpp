
#include "tools.h"
#include "ErrorLogManager.h"
#include "ResourceManager.h"
#include "SDL2DRenderManager.h"

int main(int, char**)
{
    ErrorLogManager* errorManager = ErrorLogManager::GetInstance();
    errorManager->CreateLogFile( tools::GetModulePath() + std::string( "/logError.txt" ) );
     
    try
    {
        auto resourceManager = new ResourceManager();
        auto sdlRenderManager = SDL2DRenderManager::GetInstance();
        sdlRenderManager->Initialize();

        // Test
        /*auto imageTest = new SDLRenderResource();
        imageTest->Initialize( 0, 0, tools::GetModulePath() + std::string( "/../../data/graphic/bluesky.jpg" ) );
        imageTest->Load();
        imageTest->RenderTextureAtPos( 10, 10 );
        imageTest->RenderTextureAtPos( 100, 100 );
        imageTest->UnLoad();
        */

        bool quit = false;
        while( !quit )
        {
            if( !SDL2DRenderManager::GetInstance()->Update() )
                quit = true;
        }
    }
    catch( cException& e )
    {
        MessageBox( NULL, e.what(), "", MB_OK );
        errorManager->LogException( e );
        errorManager->Flush();
    }

    return 1;
}