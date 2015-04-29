#ifndef _SDL2DRENDERMANAGER_H
#define _SDL2DRENDERMANAGER_H

//#include "RenderManager.h"
#include "SingletonTemplate.h"
#include "Resource.h"
#include "RenderManager_ABC.h"

#include "tinyxml2.h"
#include "SDL.h"
#include "SDLRenderObject.h"
#include "SDL_video.h"
#include "SDL_render.h"

#include <sstream>
#include <list>

class SDL2DRenderManager : public EngineObject
                         , public RenderManager_ABC
                         , public SingletonTemplate< SDL2DRenderManager >
{
    friend SDL2DRenderManager* SingletonTemplate< SDL2DRenderManager >::GetInstance( );
    friend void SingletonTemplate< SDL2DRenderManager >::Kill( );

public:
    virtual void Initialize( unsigned int width = 800, unsigned int height = 600, bool fullscreen = false, char* windowTitle = 0 );
    virtual void Free(){};
    virtual bool Update();
    virtual void toggleFullScreen(){};
    virtual Resource* CreateRenderResource(); //LoadResourceFromXML ? Is there any more data in an xml file that could be useful in that class ?
    virtual void RenderAllObjects(){};
    virtual void PushBackRenderObject( EngineObject* object );

    // Helpers
    SDL_Renderer* GetRenderer() const;

private:

    SDL2DRenderManager( const SDL2DRenderManager& ){};
    SDL2DRenderManager& operator=( const SDL2DRenderManager& ){};
    SDL2DRenderManager(){};
    virtual ~SDL2DRenderManager();

private:
    SDL_Window* renderWindow_;
    SDL_Renderer *renderer_;
    //std::stringstream videoInfos_;
    std::list< EngineObject* > renderObjects_;
};

#endif // SDL2DRenderManager.h