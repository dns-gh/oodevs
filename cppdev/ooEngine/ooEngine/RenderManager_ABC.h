#ifndef _RENDERMANAGER_ABC_H
#define _RENDERMANAGER_ABC_H

#include "Resource.h"

class RenderManager_ABC
{
public:
    virtual void Initialize( unsigned int width, unsigned int height, bool fullscreen, char* windowTitle) = 0;
    virtual void Free() = 0;
    virtual bool Update() = 0;
    virtual void toggleFullScreen() = 0;
    virtual Resource* CreateRenderResource() = 0;
    virtual void RenderAllObjects() = 0;

protected:
    virtual ~RenderManager_ABC(){};

};

#endif // RenderManager_ABC.h