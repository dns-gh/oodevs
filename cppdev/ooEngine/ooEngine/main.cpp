#include "ResourceManager.h"
#include "SceneManager2D.h"
#include "InputManager.h"
#include "ListenerTest.h"

#include "tools.h"
#include "ErrorLogManager.h"
#include "LogTools.h"

#include "SDL_Rect.h"
#include "SDL2DRenderManager.h"
#include "SDLEntityFactory.h"
#include "SDLSpriteObject.h"

#include <memory>

enum Actions {
    Quit = 0,
    Resume = 1,
    Pause,
    RightMove = 10,
    LeftMove,
    UpMove,
    DownMove,
};

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
        auto sdlRenderManager = std::make_shared< SDL2DRenderManager >( *logger );
        sdlRenderManager->Initialize( 800, 600, false, "window 1"  );

        /*std::shared_ptr< RenderManager_ABC > sdlRenderManager2 = std::make_shared< SDL2DRenderManager >( *logger );
        sdlRenderManager2->Initialize( 800, 600, false, "window 2" );
        */

        auto resourceManager = std::make_shared< ResourceManager >( *sdlRenderManager, *logger );
        resourceManager->LoadFromXMLFile( tools::GetModulePath() + std::string( "../../data/graphic/template.xml" ) );

        auto entityFactory = std::make_shared< SDLEntityFactory >( *logger );
        auto collisionSolver = std::make_shared< CollisionSolver >( *logger );

        auto sceneManager = std::make_shared< SceneManager2D >( *resourceManager, *collisionSolver, *entityFactory, *logger );
        sdlRenderManager->SetSceneManager2D( sceneManager );

        // Default image in the back
        std::shared_ptr< SDLRenderObject> back( new SDLRenderObject() );
        back->SetResourceObject( resourceManager->FindResourceByID( 3 ) );
        sdlRenderManager->InsertSceneObject( back, "defaultLayer" );
        back->SetVisible( true );     

        // Sprites test
        std::shared_ptr< SDLSpriteObject > spriteTest( new SDLSpriteObject( 48, 8, 6, 125 ) );
        spriteTest->SetPosition( 300, 200 );
        spriteTest->SetVisible( true );
        spriteTest->SetResourceObject( resourceManager->FindResourceByID( 2 ) );
        spriteTest->GoToFrame( 0 );
        //spriteTest->SetColorKeying( true, 0, 0, 0 );
        Vector2D pos( 390,290 );
        Circle cc( pos, 90 );
        spriteTest->SetCollisionCircle( cc );
        sdlRenderManager->DrawDebugCircle( &*spriteTest );
        sdlRenderManager->InsertSceneObject( spriteTest, "defaultLayer" );

        // Sprites test
        std::shared_ptr< SDLSpriteObject > spriteTest2( new SDLSpriteObject( 48, 8, 6, 125 ) );
        spriteTest2->SetPosition( 120, 200 );
        spriteTest2->SetVisible( true );
        spriteTest2->SetResourceObject( resourceManager->FindResourceByID( 2 ) );
        spriteTest2->SetColorKeying( true, 0, 0, 0 );
        spriteTest2->GoToFrame( 0 );
        cc.center_.x_ = 210;
        spriteTest2->SetCollisionCircle( cc );
        sdlRenderManager->DrawDebugCircle( &*spriteTest2 );
        sdlRenderManager->InsertSceneObject( spriteTest2, "defaultLayer" );

        // Debug boxes
        SDL_Rect rect;
        rect.x = 120;
        rect.y = 200;
        rect.w = rect.h = 180;
        sdlRenderManager->AttachDrawingDebugBox( &rect, &*spriteTest );
        sdlRenderManager->AttachDrawingDebugBox( &rect, &*spriteTest2 );

        // Test listener
        /*sceneManager->AddListener( new TestListener() );
        sceneManager->AddTimer( 2000 );
        */
        sceneManager->LoadFromXMLFile( tools::GetModulePath() + std::string( "../../data/scene/sceneTest.xml" ) );

        // Test input manager
        logger->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Input Manager: test" );
        auto inputManager = std::make_shared< InputManager >( *logger );
        /*inputManager->Bind( 0, SDL_SCANCODE_KP_0 );
        inputManager->Bind( 0, SDL_SCANCODE_KP_1 );
        inputManager->Bind( 1, SDL_SCANCODE_KP_1 );
        inputManager->UnBind( 0, SDL_SCANCODE_KP_0 );
        inputManager->UnBind( 0, SDL_SCANCODE_KP_1 );
        */
        inputManager->Bind( Actions::Quit, SDL_SCANCODE_ESCAPE );
        inputManager->Bind( Actions::Resume, SDL_SCANCODE_R );
        inputManager->Bind( Actions::Pause, SDL_SCANCODE_P );

        inputManager->Bind( Actions::RightMove, SDL_SCANCODE_RIGHT );
        inputManager->Bind( Actions::LeftMove, SDL_SCANCODE_LEFT );
        inputManager->Bind( Actions::UpMove, SDL_SCANCODE_UP );
        inputManager->Bind( Actions::DownMove, SDL_SCANCODE_DOWN );

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

            if( inputManager->PerformAction( Actions::Quit ) )
                quit = true;

            if( inputManager->PerformAction( Actions::Resume ) ) // resume or play
            {
                spriteTest->Resume();
                spriteTest2->Resume();
            }
            if( inputManager->PerformAction( Actions::Pause ) )
            {
                spriteTest->Pause();
                spriteTest2->Pause();
            }

            Geometry2D::Vector2D dirX(1,0), dirY(0,1);
            if( inputManager->PerformAction( Actions::RightMove ) )
                spriteTest->Move( dirX * 0.1f );
            if( inputManager->PerformAction( Actions::LeftMove ) )
                spriteTest->Move( -dirX * 0.1f );
            if( inputManager->PerformAction( Actions::UpMove ) )
                spriteTest->Move( -dirY * 0.1f );
            if( inputManager->PerformAction( Actions::DownMove ) )
                spriteTest->Move( dirY * 0.1f );

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