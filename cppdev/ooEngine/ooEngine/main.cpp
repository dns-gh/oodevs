
#include "tools.h"
#include "ErrorLogManager.h"
#include "ResourceManager.h"
#include "SDL2DRenderManager.h"
#include "SDLSpriteObject.h"

int main(int, char**)
{
    ErrorLogManager* errorManager = ErrorLogManager::GetInstance();
    errorManager->CreateLogFile( tools::GetModulePath() + std::string( "/logError.txt" ) );
     
    try
    {
        // Initialization of managers
        auto resourceManager = new ResourceManager();
        resourceManager->LoadFromXMLFile( tools::GetModulePath() + std::string( "/../../data/graphic/template.xml" ) );
        auto sdlRenderManager = SDL2DRenderManager::GetInstance();
        sdlRenderManager->Initialize();

        // Sprites test
        SDLSpriteObject* spriteTest = new SDLSpriteObject( 48, 8, 6, 125 );
        spriteTest->SetPosition( 300, 200 );
        spriteTest->SetResourceObject( resourceManager->FindResourceByID( 1 ) );
        sdlRenderManager->InsertRenderObject( spriteTest );
        spriteTest->Play( 0 );

        // Rendering test
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