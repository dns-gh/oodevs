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
    Layer2D( std::string );
    void Update();
    std::string GetName() const;
    unsigned int GetZOrder() const;

private:
    bool isVisible_;
    unsigned int zOrder_;
    float x_;
    float y_;
    std::string name_;
    std::list< std::shared_ptr< SceneObject_ABC > > sceneObjects_;
};

#endif // Layer2D.h