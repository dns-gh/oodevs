#ifndef _SDLENTITY_FACTORY
#define _SDLENTITY_FACTORY

#include "EntityFactory_ABC.h"
#include "LogTools.h"

class SDLEntityFactory : public EntityFactory_ABC
{
public:
    SDLEntityFactory( LogTools& logger );
    virtual std::shared_ptr< SceneObject > CreateRenderObject() const;
    virtual std::shared_ptr< SceneObject > CreateSpriteObject( unsigned imgNum, unsigned imgPRows, unsigned imgPCol, unsigned long timeBImg ) const;

private:
    LogTools& logger_;
};

#endif // SDLEntityFactory.h
