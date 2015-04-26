
#include "tools.h"
#include "ErrorLogManager.h"
#include "ResourceManager.h"
#include "SDL2DRenderManager.h"

int main(int, char**)
{
    ErrorLogManager* errorManager = ErrorLogManager::GetInstance();
    errorManager->CreateLogFile( tools::GetModulePath() + std::string( "logError.txt" ) );
     
    try
    {
        OOTHROW( 1, "Test error message" );
    }
    catch( cException& e )
    {
        MessageBox( NULL, e.what(), "", MB_OK );
        errorManager->LogException( e );
        errorManager->Flush();
    }

    ResourceManager* resourceManager = new ResourceManager();
    delete resourceManager;
    SDL2DRenderManager* sdlRenderManager = SDL2DRenderManager::GetInstance();
    sdlRenderManager->Initialize();
    SDL_Delay( 4000 );

    return 1;
}