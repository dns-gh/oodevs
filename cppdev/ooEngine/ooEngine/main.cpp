#include "ResourceManager.h"
#include "SceneManager2D.h"
#include "InputManager.h"
#include "ListenerTest.h"
#include "ooEngine.h"

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
    std::unique_ptr< ooEngine > engine( new ooEngine() );
    try
    {
        engine->GetResourceManager()->LoadFromXMLFile( tools::GetModulePath() + std::string( "../../data/graphic/template.xml" ) );

        // Default image in the back
        std::shared_ptr< SDLRenderObject> back( new SDLRenderObject() );
        back->SetResourceObject( engine->GetResourceManager()->FindResourceByID( 3 ) );
        engine->GetRenderManager2D()->InsertSceneObject( back, "defaultLayer" );
        back->SetVisible( true );     

        // Sprites test
        std::shared_ptr< SDLSpriteObject > spriteTest( new SDLSpriteObject( 48, 8, 6, 125 ) );
        spriteTest->SetPosition( 300, 200 );
        spriteTest->SetVisible( true );
        spriteTest->SetResourceObject( engine->GetResourceManager()->FindResourceByID( 2 ) );
        spriteTest->GoToFrame( 0 );
        //spriteTest->SetColorKeying( true, 0, 0, 0 );
        Vector2D pos( 390,290 );
        Circle cc( pos, 90 );
        spriteTest->SetCircleCollisionShape( cc );
        CollisionFilter filter( CollisionCategory::Groupe2, CollisionCategory::Groupe1, 0 );
        spriteTest->SetCollisionFilter( filter );
        engine->GetRenderManager2D()->AttachDebugCircle( &*spriteTest );
        engine->GetRenderManager2D()->InsertSceneObject( spriteTest, "defaultLayer" );

        // Sprites test
        std::shared_ptr< SDLSpriteObject > spriteTest2( new SDLSpriteObject( 48, 8, 6, 125 ) );
        spriteTest2->SetPosition( 120, 200 );
        spriteTest2->SetVisible( true );
        spriteTest2->SetResourceObject( engine->GetResourceManager()->FindResourceByID( 2 ) );
        spriteTest2->SetColorKeying( true, 0, 0, 0 );
        spriteTest2->GoToFrame( 0 );
        cc.center_.x_ = 210;
        spriteTest2->SetCircleCollisionShape( cc );
        CollisionFilter filter2( CollisionCategory::Groupe1, CollisionCategory::Groupe2, 0 );
        spriteTest2->SetCollisionFilter( filter2 );
        engine->GetRenderManager2D()->AttachDebugCircle( &*spriteTest2 );
        engine->GetRenderManager2D()->InsertSceneObject( spriteTest2, "defaultLayer" );

        // Debug boxes
        /*SDL_Rect rect;
        rect.x = 120;
        rect.y = 200;
        rect.w = rect.h = 180;
        engine->GetRenderManager2D()->AttachDrawingDebugBox( &rect, &*spriteTest );
        engine->GetRenderManager2D()->AttachDrawingDebugBox( &rect, &*spriteTest2 );
        */

        // Test listener
        /*sceneManager->AddListener( new TestListener() );
        sceneManager->AddTimer( 2000 );
        */
        engine->GetSceneManager2D()->LoadFromXMLFile( tools::GetModulePath() + std::string( "../../data/scene/sceneTest.xml" ) );

        auto inputManager = engine->GetInputManager();
        inputManager->Bind( Actions::Quit, SDL_SCANCODE_ESCAPE );
        inputManager->Bind( Actions::Resume, SDL_SCANCODE_R );
        inputManager->Bind( Actions::Pause, SDL_SCANCODE_P );

        inputManager->Bind( Actions::RightMove, SDL_SCANCODE_RIGHT );
        inputManager->Bind( Actions::LeftMove, SDL_SCANCODE_LEFT );
        inputManager->Bind( Actions::UpMove, SDL_SCANCODE_UP );
        inputManager->Bind( Actions::DownMove, SDL_SCANCODE_DOWN );

        bool quit = false;
        inputManager->Register( Actions::Quit, [&](){
            engine->Stop();
        } );

        inputManager->Register( Actions::Resume, [&](){
            spriteTest->Resume();
            spriteTest2->Resume();
        } );

        inputManager->Register( Actions::Pause, [&](){
            spriteTest->Pause();
            spriteTest2->Pause();
        } );

        Geometry2D::Vector2D dirX(1,0), dirY(0,1);
        inputManager->Register( Actions::RightMove , [&](){
            spriteTest->Move( dirX * 0.1f );
        } );
        inputManager->Register( Actions::LeftMove , [&](){
            spriteTest->Move( -dirX * 0.1f );
        } );
        inputManager->Register( Actions::UpMove , [&](){
            spriteTest->Move( -dirY * 0.1f );
        } );
        inputManager->Register( Actions::DownMove , [&](){
            spriteTest->Move( dirY * 0.1f );
        } );

        engine->Run();
    }
    catch( cException& e )
    {
        MessageBox( NULL, e.what(), "", MB_OK );
        engine->GetLogger()->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_FATALERROR, e.what( ) );
    }

    return 1;
}