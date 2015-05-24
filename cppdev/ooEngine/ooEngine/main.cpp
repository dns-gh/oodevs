
#include "tools.h"
#include "ErrorLogManager.h"
#include "LogTools.h"

#include "SDL2DRenderManager.h"
#include "SDLEntityFactory.h"
#include "SDLSpriteObject.h"
#include "ResourceManager.h"
#include "SceneManager2D.h"
#include "InputManager.h"
#include "ListenerTest.h"

#include <memory>

int main(int, char**)
{
    // init of a logger
    auto logger = std::make_shared< LogTools >( tools::GetModulePath( ) );
    bool isRegistered = logger->RegisterLog( FILE_INFOS, "infos.log" );
    if( !isRegistered )
        return 0;

    try
    {
        // Initialization of managers
        std::shared_ptr< RenderManager_ABC > sdlRenderManager = std::make_shared< SDL2DRenderManager >( *logger );
        sdlRenderManager->Initialize( 800, 600, false, "window 1"  );

        /*std::shared_ptr< RenderManager_ABC > sdlRenderManager2 = std::make_shared< SDL2DRenderManager >( *logger );
        sdlRenderManager2->Initialize( 800, 600, false, "window 2" );
        */

        auto resourceManager = std::make_shared< ResourceManager >( *sdlRenderManager, *logger );
        resourceManager->LoadFromXMLFile( tools::GetModulePath() + std::string( "../../data/graphic/template.xml" ) );

        auto entityFactory = std::make_shared< SDLEntityFactory >( *logger );

        std::shared_ptr< SceneManager2D > sceneManager = std::make_shared< SceneManager2D >( *resourceManager, *entityFactory, *logger );
        sdlRenderManager->SetSceneManager2D( sceneManager );

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

        // Test listener
        sceneManager->AddListener( new TestListener() );
        sceneManager->AddTimer( 2000 );
        sceneManager->LoadFromXMLFile( tools::GetModulePath() + std::string( "../../data/scene/sceneTest.xml" ) );

        // Test input manager
        logger->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Input Manager: test" );
        auto inputManager = std::make_shared< InputManager >( *logger );
        inputManager->Bind( 0, SDL_SCANCODE_KP_0 );
        inputManager->Bind( 0, SDL_SCANCODE_KP_1 );
        inputManager->Bind( 1, SDL_SCANCODE_KP_1 );
        inputManager->UnBind( 0, SDL_SCANCODE_KP_0 );
        inputManager->UnBind( 0, SDL_SCANCODE_KP_1 );
        inputManager->Bind( 2, SDL_SCANCODE_ESCAPE );
        inputManager->Bind( 3, SDL_SCANCODE_R );
        inputManager->Bind( 4, SDL_SCANCODE_P );

        bool quit = false;
        while( !quit )
        {
            if( sceneManager )
                sceneManager->Update();
            if( sdlRenderManager )
                if( !sdlRenderManager->Update() )
                    quit = true;
            if( inputManager )
                inputManager->Update();

            if( inputManager->PerformAction( 0 ) )
                logger->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Action 0" );
            if( inputManager->PerformAction( 1 ) )
                logger->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Action 1" );
            if( inputManager->PerformAction( 2 ) )
                quit = true;

            if( inputManager->PerformAction( 3 ) ) // resume or play
            {
                spriteTest->Resume();
                spriteTest2->Resume();
            }
            if( inputManager->PerformAction( 4 ) )
            {
                spriteTest->Pause();
                spriteTest2->Pause();
            }

            SDL_Event event;
            while( SDL_PollEvent( &event ) != 0 )
            {
                switch( event.type )
                {
                    case SDL_QUIT:
                        quit = true;
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