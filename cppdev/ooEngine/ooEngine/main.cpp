
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
        ResourceManager* resourceManager = new ResourceManager();
        delete resourceManager;
        SDL2DRenderManager* sdlRenderManager = SDL2DRenderManager::GetInstance();
        sdlRenderManager->Initialize();

        SDLRenderResource* imageTest = new SDLRenderResource();
        imageTest->Initialize( 0, 0, tools::GetModulePath() + std::string( "/../../data/graphic/bluesky.jpg" ) );
        imageTest->Load();

        //While application is running
        bool quit = false;
        //Event handler
        SDL_Event e;
        while( !quit )
        {
            //Handle events on queue
            while( SDL_PollEvent( &e ) != 0 )
            {
                //User requests quit
                if( e.type == SDL_QUIT )
                    quit = true;
            }
            sdlRenderManager->Update();
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