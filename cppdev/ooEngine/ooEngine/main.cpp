
#include "tools.h"
#include "ErrorLogManager.h"
#include "LogTools.h"
#include "ResourceManager.h"
#include "SDL2DRenderManager.h"
#include "SDLSpriteObject.h"

#include <memory>

int main(int, char**)
{
    auto logger = std::make_shared< LogTools >( tools::GetModulePath( ) );
    bool isRegistered = logger->RegisterLog( FILE_INFOS, "infos.txt" );
    if( !isRegistered )
        return 0;
    logger->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, " Test : " << 1 << std::endl << " Test " << 2 );

    try
    {
        // Initialization of managers
        auto resourceManager = std::make_shared< ResourceManager>();
        resourceManager->LoadFromXMLFile( tools::GetModulePath() + std::string( "../../data/graphic/template.xml" ) );
        auto sdlRenderManager = SDL2DRenderManager::GetInstance();
        sdlRenderManager->Initialize();

        // Default image in the back
        SDLRenderObject* back = new SDLRenderObject();
        back->SetResourceObject( resourceManager->FindResourceByID( 3 ) );
        sdlRenderManager->InsertRenderObject( back );
        back->SetVisible( true );

        // Sprites test
        SDLSpriteObject* spriteTest = new SDLSpriteObject( 48, 8, 6, 125 );
        spriteTest->SetPosition( 300, 200 );
        spriteTest->SetResourceObject( resourceManager->FindResourceByID( 2 ) );
        //spriteTest->SetColorKeying( true, 0, 0, 0 );
        sdlRenderManager->InsertRenderObject( spriteTest );

        // Sprites test
        SDLSpriteObject* spriteTest2 = new SDLSpriteObject( 48, 8, 6, 125 );
        spriteTest2->SetPosition( 120, 200 );
        spriteTest2->SetResourceObject( resourceManager->FindResourceByID( 2 ) );
        spriteTest2->SetColorKeying( true, 0, 0, 0 );
        sdlRenderManager->InsertRenderObject( spriteTest2 );

        bool quit = false;
        while( !quit )
        {
            if( !SDL2DRenderManager::GetInstance()->Update() )
                quit = true;

            SDL_Event event;
            while( SDL_PollEvent( &event ) != 0 )
            {
                switch( event.type )
                {
                    case SDL_QUIT:
                        quit = true;
                    case SDL_KEYDOWN:
                    {
                        if( event.key.keysym.sym == SDLK_p )
                        {
                            spriteTest->Pause( );
                            spriteTest2->Pause( );
                        }
                        if( event.key.keysym.sym == SDLK_r ) // resume or play
                        {
                            spriteTest->Resume( );
                            spriteTest2->Resume( );
                        }
                        if( event.key.keysym.sym == SDLK_ESCAPE )
                            quit = true;
                    }
                }
            }
        }
    }
    catch( cException& e )
    {
        MessageBox( NULL, e.what(), "", MB_OK );
        logger->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_FATALERROR, e.what( ) );
    }

    return 1;
}