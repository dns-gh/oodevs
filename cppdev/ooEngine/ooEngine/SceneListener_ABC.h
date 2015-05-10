#ifndef _SCENELISTENER_ABC_H
#define _SCENELISTENER_ABC_H

class RenderManager_ABC;

enum SCENE_EVENT_TYPE
{
    NOTHING = 0,
    TIMER_EXPIRED = 1,
};

class SceneListener_ABC
{
public:
    SceneListener_ABC()
    {
        listenFor_ = NOTHING;
    }
    virtual ~SceneListener_ABC(){}
    SCENE_EVENT_TYPE listenFor_;
    virtual void Event( RenderManager_ABC* manager, void* customData ) const = 0;
};



#endif // SceneListener_ABC.h