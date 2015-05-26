#ifndef _SDL2DRENDERMANAGER_H
#define _SDL2DRENDERMANAGER_H

//#include "RenderManager.h"
#include "Geometry2D.h"
#include "SingletonTemplate.h"
#include "Resource_ABC.h"
#include "RenderManager_ABC.h"
#include "SceneManager2D.h"
#include "LogTools.h"

#include "SDL.h"
#include "SDLRenderObject.h"
#include "SDL_video.h"
#include "SDL_render.h"

#include <sstream>
#include <vector>
#include <list>
#include <utility>

class SDL2DRenderManager : public EngineObject
                         , public RenderManager_ABC
{
public:
    SDL2DRenderManager( LogTools& logger );
    virtual ~SDL2DRenderManager();

    virtual void Initialize( unsigned int width = 800, unsigned int height = 600, bool fullscreen = false, char* windowTitle = 0 );
    virtual void Clear();
    virtual bool Update();
    virtual void toggleFullScreen(){};
    virtual std::shared_ptr< Resource_ABC > CreateRenderResource() const; //LoadResourceFromXML ? Is there any more data in an xml file that could be useful in that class ?
    virtual void RenderAllObjects();
    virtual void SetSceneManager2D( std::shared_ptr< SceneManager2D >& manager );
    
    void RenderAtPosition( const SceneObject& object, const float& x, const float& y );
    SDL_Renderer* GetRenderer() const;
    // Insert a scene object into the default layer
    virtual void InsertSceneObject( const std::shared_ptr< SceneObject>& object, std::string layerName = "" );

    // Debug helpers
    void DrawCircle( const Circle& circle );
    void AttachDrawingDebugBox( SDL_Rect* rect, SceneObject* object = nullptr );
    void AttachDrawingDebugCircle( Geometry2D::Circle* circle, SceneObject* object = nullptr );

private:
    void RenderScene();
    void DrawDebugBoxes();

private:
    SDL_Window* renderWindow_;
    SDL_Renderer* renderer_;
    //std::stringstream videoInfos_;
    std::shared_ptr< SceneManager2D > sceneManager_;
    LogTools& logger_;

    // debug infos
    std::vector< std::pair< SDL_Rect*, SceneObject* > > debugRects_;
    std::vector< std::pair< Geometry2D::Circle*, SceneObject* > > debugCircles_;
};

#endif // SDL2DRenderManager.h