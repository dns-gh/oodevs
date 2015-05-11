#ifndef _TestListener_h
#define _TestListener_h

#include "SceneListener_ABC.h"

class TestListener : public SceneListener_ABC
{
public:
    TestListener()
    {
        listenFor_ = TIMER_EXPIRED;
    }

    virtual void Event( const SceneManager2D_ABC& manager, void* customData ) const
    {
        auto layer = manager.FindLayer( "LayerTest" );
        if( layer )
            layer->SetVisible( !layer->IsVisible() );
    }
};

#endif