#include "SDL2DRenderManager.h"
#include "SDLRenderResource.h"
#include "ErrorLogManager.h"
#include "SDL_hints.h"
#include "SDL_error.h"
#include "SDL_image.h"

SDL2DRenderManager::~SDL2DRenderManager( )
{
    SDL_DestroyRenderer( renderer_ );
    SDL_DestroyWindow( renderWindow_ );
    SDL_Quit();
}

void SDL2DRenderManager::Initialize( unsigned int width, unsigned int height, bool fullScreen, char* windowTitle )
{
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
        OOTHROW( 1, "Error when trying to initialize the SDL lib : " + std::string( SDL_GetError( ) ) );

    renderWindow_ = SDL_CreateWindow( windowTitle,
        SDL_WINDOWPOS_UNDEFINED,
        SDL_WINDOWPOS_UNDEFINED,
        width, height,
        fullScreen ? SDL_WINDOW_FULLSCREEN : SDL_WINDOW_RESIZABLE );

    if( !renderWindow_ )
        OOTHROW( 1, "Error when trying to create a window : " + std::string( SDL_GetError() ) );

    renderer_ = SDL_CreateRenderer( renderWindow_, -1, 
                                    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC ); // Use of SDL_CreateWindowAndRenderer possible
    if( !renderer_ )
        OOTHROW( 1, "Error when trying to create a renderer : " + std::string( SDL_GetError( ) ) );

    // Get some info about the renderer
    /* SDL_RendererInfo info;
    SDL_GetRendererInfo( renderer_, &info );*/

    // Set the rendered default draw color to white
    SDL_SetRenderDrawColor( renderer_, 255, 255, 255, 0);

    //Initialize PNG loading (useful ?)
    /*int imgFlags = IMG_INIT_PNG;
    if( !( IMG_Init( imgFlags ) & imgFlags ) )
        OOTHROW( 1, "Error when trying initialize the SDL Image lib : " + std::string( IMG_GetError( ) ) );
        */
    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "linear" );
    SDL_RenderSetLogicalSize( renderer_, width, height );
}

bool SDL2DRenderManager::Update()
{
    //Event handler
    SDL_Event event;
    while( SDL_PollEvent( &event ) != 0 )
    {
        switch( event.type )
        {
            case SDL_QUIT:
                return false;
            case SDL_KEYDOWN:
            {
                if( event.key.keysym.sym == SDLK_ESCAPE )
                    return false;
            }
        }
    }

    if( !renderer_ )
        OOTHROW( 1, "No rendering context to drawn in" );

    SDL_RenderClear( renderer_ );
    RenderAllObjects();

    SDL_RenderPresent( renderer_ );
    return true;

}

Resource* SDL2DRenderManager::CreateRenderResource()
{
    return new SDLRenderResource();
}

void SDL2DRenderManager::RenderAllObjects()
{
    for( auto it = renderObjects_.begin(); it != renderObjects_.end(); ++it )
    {
        if( ( *it )->IsVisible() )
        {
            if( !( *it )->GetRenderResource( )->GetTexture( ) )
                continue;
            // Update of the SDLRenderObject. In case of a sprite object, it sets the image to the correct one if need be.
            ( *it )->Update();
            SDL_Rect rect = ( *it )->GetRenderRect();
            rect.x = static_cast< int >( ( *it )->X() );
            rect.y = static_cast< int >( ( *it )->Y() );
            // no use since w and h are already set when GetRenderRect is called.
            //SDL_QueryTexture( ( *it )->GetRenderResource( )->GetTexture( ), NULL, NULL, &rect.w, &rect.h );
            SDL_RenderCopy( renderer_, ( *it )->GetRenderResource()->GetTexture(), NULL, &rect );
        }
    }
}

SDL_Renderer* SDL2DRenderManager::GetRenderer() const
{
    return renderer_;
}

void SDL2DRenderManager::PushBackRenderObject( SDLRenderObject* object )
{
    if( object )
        renderObjects_.push_back( object );
}