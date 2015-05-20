#include "SDLEntityFactory.h"
#include "SDLSpriteObject.h"

std::shared_ptr< SceneObject > SDLEntityFactory::CreateRenderObject() const
{
    std::shared_ptr< SceneObject > object( new SDLRenderObject() );
    return object;
}

std::shared_ptr< SceneObject > SDLEntityFactory::CreateSpriteObject( unsigned imgNum, unsigned imgPRows, unsigned imgPCol, unsigned long timeBImg ) const
{
    std::shared_ptr< SceneObject > object( new SDLSpriteObject( imgNum, imgPRows, imgPCol, timeBImg ) );
    return object;
}