#include "SDL2DRenderManager.h"
#include "ErrorLogManager.h"
#include "SDL_hints.h"
#include "SDL_error.h"

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

    renderer_ = SDL_CreateRenderer( renderWindow_, -1, 0 ); // Use of SDL_CreateWindowAndRenderer possible
    if( !renderer_ )
        OOTHROW( 1, "Error when trying to create a renderer : " + std::string( SDL_GetError( ) ) );

    // Get some info about the renderer
    /* SDL_RendererInfo info;
    SDL_GetRendererInfo( renderer_, &info );*/

    // Set the rendered default draw color to white
    SDL_SetRenderDrawColor( renderer_, 255, 255, 255, 0);

    SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "linear" );
    SDL_RenderSetLogicalSize( renderer_, width, height );
}

bool SDL2DRenderManager::Update()
{
    if( !renderer_ )
        return false;

    SDL_RenderPresent( renderer_ );
    return true;
}