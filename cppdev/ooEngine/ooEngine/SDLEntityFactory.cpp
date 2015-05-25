#include "SDLEntityFactory.h"
#include "SDLSpriteObject.h"

SDLEntityFactory::SDLEntityFactory( LogTools& logger )
    : logger_( logger )
{
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "SDL 2D: creation of the entity factory (id:" << GetID() << ")" );
}

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