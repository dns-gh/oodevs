#include "ooEngine.h"

#include "EntityFactory_ABC.h"
#include "ErrorLogManager.h"
#include "SceneManager2D.h"
#include "InputManager.h"

#include "SDL2DRenderManager.h"
#include "SDLEntityFactory.h"

#include <memory>

ooEngine::ooEngine() // TODO: put Config class as input
    : running_( false )
    , paused_( false )
{
    // Init of a logger
    logger_ = std::make_shared< LogTools >( tools::GetModulePath( ) );
    logger_->RegisterLog( FILE_INFOS, "ooEngine.log" );

    logger_->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "------------------------------" );
    logger_->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "-- ooEngine: initialization --" );
    logger_->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "------------------------------" );

    // Initialization of the render manager
    renderManager2D_ = std::make_shared< SDL2DRenderManager >( *logger_ );
    renderManager2D_->Initialize( 800, 600, false, "window 1"  );

    resourcesManager_ = std::make_shared< ResourceManager >( *renderManager2D_, *logger_ );

    entityFactory_ = std::make_shared< SDLEntityFactory >( *logger_ );
    auto collisionSolver = std::make_shared< CollisionSolver >( *logger_ );
    sceneManager2D_ = std::make_shared< SceneManager2D >( *resourcesManager_, *collisionSolver, *entityFactory_, *logger_ );
    renderManager2D_->SetSceneManager2D( sceneManager2D_ );

    inputManager_ = std::make_shared< InputManager >( *logger_ );
}

ooEngine::~ooEngine()
{
    // NOTHING
}

std::shared_ptr< LogTools > ooEngine::GetLogger() const
{
    return logger_;
}

std::shared_ptr< RenderManager_ABC > ooEngine::GetRenderManager2D() const
{
    return renderManager2D_;
}

std::shared_ptr< ResourceManager > ooEngine::GetResourceManager() const
{
    return resourcesManager_;
}

std::shared_ptr< SceneManager2D > ooEngine::GetSceneManager2D() const
{
    return sceneManager2D_;
}

std::shared_ptr< InputManager > ooEngine::GetInputManager() const
{
    return inputManager_;
}

void ooEngine::Run()
{
    logger_->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "-----------------------" );
    logger_->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "-- ooEngine: running --" );
    logger_->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "-----------------------" );
    running_ = true;
    MainLoop();
    logger_->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "---------------------" );
    logger_->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "-- ooEngine: stop! --" );
    logger_->OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "---------------------" );
}

void ooEngine::Pause()
{
    paused_ = true;
}

void ooEngine::Resume()
{
    paused_ = false;
}

void ooEngine::Stop()
{
    running_ = false;
}

void ooEngine::MainLoop()
{
    while( running_ )
    {
        if( paused_ )
            continue;
        if( sceneManager2D_ )
            sceneManager2D_->Update();
        if( renderManager2D_ )
            renderManager2D_->Update();
        if( inputManager_ )
            inputManager_->Update();
    }
}