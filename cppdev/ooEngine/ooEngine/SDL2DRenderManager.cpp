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
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: creation of a render manager (id:" << GetID() << ")" );
}


SDL2DRenderManager::~SDL2DRenderManager()
{
    Clear();
    SDL_DestroyRenderer( renderer_ );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: destroy renderer" );
    SDL_DestroyWindow( renderWindow_ );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: destroy window" );
    SDL_Quit();
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: sdl quit" );
}

void SDL2DRenderManager::Initialize( unsigned int width, unsigned int height, bool fullScreen, char* windowTitle )
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        OOTHROW( 1, "Error when trying to initialize the SDL lib : " + std::string( SDL_GetError( ) ) );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: initialization of the render manager" );

    renderWindow_ = SDL_CreateWindow( windowTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: creation of the window: " << windowTitle );

    if( !renderWindow_ )
        OOTHROW( 1, "Error when trying to create a window : " + std::string( SDL_GetError() ) );

    renderer_ = SDL_CreateRenderer( renderWindow_, -1, 
                                    SDL_RENDERER_ACCELERATED ); // Use of SDL_CreateWindowAndRenderer possible
    if( !renderer_ )
        OOTHROW( 1, "Error when trying to create a renderer : " + std::string( SDL_GetError( ) ) );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: creation of the renderer" );

    // Get some info about the renderer
    /* SDL_RendererInfo info;
    SDL_GetRendererInfo( renderer_, &info );*/

    // Set the rendered default draw color to grey
    const int color = 188;
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
    // NOTHING
}

bool SDL2DRenderManager::Update()
{
    if( !renderer_ )
        OOTHROW( 1, "No rendering context to drawn in" );

    SDL_RenderClear( renderer_ );
    RenderAllObjects();
    DrawDebugBoxes();
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
    RenderScene();
}

void SDL2DRenderManager::SetSceneManager2D( std::shared_ptr< SceneManager2D >& manager )
{
    sceneManager_ = manager;
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: set the 2D scene manager to the sdl render manager" );
    // Create a default layer
    if( sceneManager_ )
        sceneManager_->CreateLayer( "defaultLayer" );
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
    Geometry2D::RenderRect rect = object.GetRenderRect( );
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

void SDL2DRenderManager::InsertSceneObject( const std::shared_ptr< SceneObject>& object, std::string layerName /* "" */ )
{
    if( sceneManager_ )
    {
        object->SetSceneManager2D( &*sceneManager_ );
        auto layer = sceneManager_->FindLayer( layerName );
        if( layer )
        {        
            if( object )
                layer->InsertSceneObject( object );
            return;
        }
        logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_WARNING, "SDL Render Manager: no layer found of the name: " << layerName );
    }
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_WARNING, "SDL Render Manager: scene manager not set for the sdl renderer" );
}

void SDL2DRenderManager::AttachDrawingDebugBox( SDL_Rect* rect, SceneObject* object /* = 0 */ )
{
    auto pair = std::make_pair( rect, object );
    debugRects_.push_back( pair );
}

void SDL2DRenderManager::DrawDebugCircle( SceneObject* object )
{
    if( object )
        debugCircles_.push_back( object );
}

void SDL2DRenderManager::DrawCircle( const Circle& circle )
{
    int sampling = static_cast< int >( circle.radius_ );
    int center = static_cast< int >( circle.center_.x_ );
    SDL_Point* pointsUp = new SDL_Point[ 2 * sampling + 1 ];
    SDL_Point* pointsDown = new SDL_Point[ 2 * sampling + 1 ];
    for( int i = 0; i < 2 * sampling + 1; ++i )
    {
        int posx = i - sampling + center;
        pointsUp[ i ].x = posx;
        pointsDown[ i ].x = posx;
        pointsUp[ i ].y =  static_cast< int >( circle.center_.y_ + sqrt( pow( circle.radius_, 2.0 ) - pow( posx - center, 2.0 ) ) );
        pointsDown[ i ].y = static_cast< int >( circle.center_.y_ - sqrt( pow( circle.radius_, 2.0 ) - pow( posx - center, 2.0 ) ) );
    }
    SDL_RenderDrawLines( renderer_, pointsUp, 2 * sampling + 1 );
    SDL_RenderDrawLines( renderer_, pointsDown, 2 * sampling + 1 );
    delete pointsUp;
    delete pointsDown;
}

void SDL2DRenderManager::DrawDebugBoxes()
{
    for( auto it : debugRects_ )
    {
        if( it.second )
        {
            it.first->x = static_cast< int >( it.second->X() );
            it.first->y = static_cast< int >( it.second->Y() );
        }
        SDL_RenderDrawRect( renderer_, it.first );
    }

    for( auto it : debugCircles_ )
    {
        DrawCircle( it->GetCircleCollisionShape() );
    }
}