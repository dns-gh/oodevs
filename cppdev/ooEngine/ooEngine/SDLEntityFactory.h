#ifndef _SDLENTITY_FACTORY
#define _SDLENTITY_FACTORY

#include "EntityFactory_ABC.h"

class SDLEntityFactory : public EntityFactory_ABC
{
public:
    virtual std::shared_ptr< SceneObject > CreateRenderObject() const;
    virtual std::shared_ptr< SceneObject > CreateSpriteObject( unsigned imgNum, unsigned imgPRows, unsigned imgPCol, unsigned long timeBImg ) const;
};

#endif // SDLEntityFactory.h
