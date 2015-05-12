#ifndef _LAYER2D_H_
#define _LAYER2D_H_

#include "EngineObject.h"
#include "SceneObject_ABC.h"

#include <list>
#include <string>
#include <memory>

class Layer2D : public EngineObject
{
public :
    Layer2D();
    Layer2D( bool visible, unsigned int zorder, float x, float y, std::string name );
    void Update();
    float X() const;
    float Y() const;
    const std::string& GetName() const;
    unsigned int GetZOrder() const;
    bool IsVisible() const;
    const std::list< std::shared_ptr< SceneObject_ABC > > GetSceneObjects() const;

    void SetVisible( bool visibility );
    void InsertSceneObject( const std::shared_ptr< SceneObject_ABC >& object );

private:
    bool isVisible_;
    unsigned int zOrder_;
    float x_;
    float y_;
    std::string name_;
    std::list< std::shared_ptr< SceneObject_ABC > > sceneObjects_;
};

#endif // Layer2D.h