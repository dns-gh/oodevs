#include "SDL2DRenderManager.h"
#include "SDLRenderResource.h"
#include "ErrorLogManager.h"
#include "tools.h"

#include "SDL_hints.h"
#include "SDL_error.h"
#include "SDL_image.h"

#include <memory>

SDL2DRenderManager::SDL2DRenderManager( LogTools& logger )
    : logger_( logger )
{
    // NOTHING
}


SDL2DRenderManager::~SDL2DRenderManager()
{
    Clear();
    SDL_DestroyRenderer( renderer_ );
    SDL_DestroyWindow( renderWindow_ );
    SDL_Quit();
}

void SDL2DRenderManager::Initialize( unsigned int width, unsigned int height, bool fullScreen, char* windowTitle )
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        OOTHROW( 1, "Error when trying to initialize the SDL lib : " + std::string( SDL_GetError( ) ) );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D : initialization of the render manager" );

    renderWindow_ = SDL_CreateWindow( windowTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D : creation of the window" );

    if( !renderWindow_ )
        OOTHROW( 1, "Error when trying to create a window : " + std::string( SDL_GetError() ) );

    renderer_ = SDL_CreateRenderer( renderWindow_, -1, 
                                    SDL_RENDERER_ACCELERATED ); // Use of SDL_CreateWindowAndRenderer possible
    if( !renderer_ )
        OOTHROW( 1, "Error when trying to create a renderer : " + std::string( SDL_GetError( ) ) );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D : creation of the renderer" );

    // Get some info about the renderer
    /* SDL_RendererInfo info;
    SDL_GetRendererInfo( renderer_, &info );*/

    // Set the rendered default draw color to white
    const int color = 255;
    const int alpha = 0;
    SDL_SetRenderDrawColor( renderer_, color, color, color, alpha );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: set render draw default color: " << "[" << color << "," << color << "," << color << "," << alpha << "]" );

    //Initialize PNG loading (useful ?)
    /*int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
        OOTHROW( 1, "Error when trying initialize the SDL Image lib : " + std::string( IMG_GetError( ) ) );
        */
    //SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "linear" );
    SDL_RenderSetLogicalSize( renderer_, width, height );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: set render logical size: " << width << "," << height );
}

void SDL2DRenderManager::Clear()
{
    for( auto it = renderObjects_.begin(); it != renderObjects_.end(); ++it )
        OODELETE( *it );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: clear all render objects" );
}

bool SDL2DRenderManager::Update()
{
    if( !renderer_ )
        OOTHROW( 1, "No rendering context to drawn in" );

    SDL_RenderClear( renderer_ );
    RenderAllObjects();
    RenderScene();
    SDL_RenderPresent( renderer_ );
    return true;
}

std::shared_ptr< Resource_ABC > SDL2DRenderManager::CreateRenderResource() const
{
    std::shared_ptr< Resource_ABC > sharedResource( new SDLRenderResource( *this ) );
    return sharedResource;
}

void SDL2DRenderManager::RenderAllObjects()
{
    for( auto it = renderObjects_.begin(); it != renderObjects_.end(); ++it )
    {
        if( ( *it )->IsVisible() )
        {
            if( !( *it )->GetRenderResource() )
                continue;
            auto sdlResource = std::dynamic_pointer_cast< SDLRenderResource_ABC >( ( *it )->GetRenderResource() );
            if( !sdlResource->GetTexture() )
                continue;
            // Update of the SDLRenderObject. In case of a sprite object, it sets the image to the correct one if need be.
            ( *it )->Update();
            RenderAtPosition( **it, ( *it )->X(), ( *it )->Y() );
        }
    }
}

void SDL2DRenderManager::SetSceneManager2D( std::shared_ptr< SceneManager2D >& manager )
{
    sceneManager_ = manager;
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: set 2D scene manager" );
}

void SDL2DRenderManager::RenderScene()
{
    if( sceneManager_ )
    {
        for( auto it = sceneManager_->GetLayers().begin(); it != sceneManager_->GetLayers().end(); ++it )
        {
            auto layer = *it;
            if( !layer->IsVisible() )
                continue;

            auto& objects = layer->GetSceneObjects();
            for( auto objectIt = objects.begin(); objectIt != objects.end(); ++objectIt )
            {
                auto object = *objectIt;
                if( !object->IsVisible() )
                    continue;
                object->Update();
                RenderAtPosition( *object, layer->X() + object->X(), layer->Y() + object->Y() );
            }
        }
    }
}

void SDL2DRenderManager::RenderAtPosition( const SceneObject& object, const float& x, const float& y )
{
    SDL_Rect src;
    RenderGeometry::RenderRect rect = object.GetRenderRect();
    memcpy( &src, &rect, sizeof( rect ) );
    SDL_Rect dst;
    dst.x = static_cast< int >( x );
    dst.y = static_cast< int >( y );
    dst.w = src.w;
    dst.h = src.h;
    auto sdlResource = std::dynamic_pointer_cast< SDLRenderResource_ABC >( object.GetRenderResource() );
    SDL_RenderCopy( renderer_, sdlResource->GetTexture(), &src, &dst );
}

SDL_Renderer* SDL2DRenderManager::GetRenderer() const
{
    return renderer_;
}

void SDL2DRenderManager::InsertRenderObject( SceneObject* object )
{
    if( object )
        renderObjects_.push_back( dynamic_cast< SDLRenderObject* >( object ) );
}