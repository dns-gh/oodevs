#ifndef _RENDERMANAGER_ABC_H
#define _RENDERMANAGER_ABC_H

#include "Resource_ABC.h"
#include "SceneObject_ABC.h"

#include <memory>

class SceneManager2D;

class RenderManager_ABC
{
public:
    virtual void Initialize( unsigned int width = 800, unsigned int height = 600, bool fullscreen = false, char* windowTitle = 0 ) = 0;
    virtual void Clear() = 0;
    virtual bool Update() = 0;
    virtual void toggleFullScreen() = 0;
    virtual std::shared_ptr< Resource_ABC > CreateRenderResource() const = 0;
    virtual void RenderAllObjects() = 0;
    virtual void SetSceneManager2D( std::shared_ptr< SceneManager2D >& manager ) = 0;
    virtual void RenderScene() = 0;
    virtual void InsertRenderObject( SceneObject_ABC* object ) = 0;

protected:
    virtual ~RenderManager_ABC(){};

};

#endif // RenderManager_ABC.h