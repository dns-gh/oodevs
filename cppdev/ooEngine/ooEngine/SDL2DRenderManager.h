#ifndef _SDL2DRENDERMANAGER_H
#define _SDL2DRENDERMANAGER_H

//#include "RenderManager.h"
#include "SingletonTemplate.h"
#include "Resource.h"
#include "SDL.h"
#include "SDLRenderObject.h"
#include "tinyxml2.h"
#include "SDL_video.h"
#include "SDL_render.h"
#include <sstream>
#include <list>

class SDL2DRenderManager : public EngineObject
                         , public SingletonTemplate< SDL2DRenderManager > // : public RenderManager
{
    friend SDL2DRenderManager* SingletonTemplate< SDL2DRenderManager >::GetInstance( );
    friend void SingletonTemplate< SDL2DRenderManager >::Kill( );

public:
    void Initialize( unsigned int width = 800, unsigned int height = 600, bool fullscreen = false, char* windowTitle = 0 );
    void Free();
    bool Update();
    void toggleFullScreen();
    Resource* LoadResourceFromXML( tinyxml2::XMLElement* element );
    void RenderAllObjects();
private:

    SDL2DRenderManager( const SDL2DRenderManager& ){};
    SDL2DRenderManager& operator=( const SDL2DRenderManager& ){};
    SDL2DRenderManager(){};
    virtual ~SDL2DRenderManager();

private:
    SDL_Window* renderWindow_;
    SDL_Renderer *renderer_;
    //std::stringstream videoInfos_;
    std::list< SDLRenderObject* > renderObjects_;
};

#endif // SDL2DRenderManager.h