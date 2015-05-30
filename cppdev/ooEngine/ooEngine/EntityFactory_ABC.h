#ifndef _ENTITY_FACTORY_ABC
#define _ENTITY_FACTORY_ABC

#include "SceneObject.h"
#include <memory>

class EntityFactory_ABC
{
public:
    virtual ~EntityFactory_ABC(){};
    virtual std::shared_ptr< SceneObject > CreateRenderObject() const = 0;
    virtual std::shared_ptr< SceneObject > CreateSpriteObject( unsigned imgNum, unsigned imgPRows, unsigned imgPCol, unsigned long timeBImg ) const = 0;
};

#endif // EntityFactory_ABC.h
