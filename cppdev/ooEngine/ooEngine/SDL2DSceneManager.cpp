#include "SDL2DSceneManager.h"
#include "SceneListener_ABC.h"

std::shared_ptr< Layer2D > SDL2DSceneManager::CreateLayer( std::string name )
{
    std::shared_ptr< Layer2D > layer = FindLayer( name );

    if( !layer )
    {
        layer.reset( new Layer2D( name ) );
        layers_.push_back( layer );
    }
    return layer;
}

std::shared_ptr< Layer2D > SDL2DSceneManager::FindLayer( std::string name ) const
{
    for( auto it = layers_.begin(); it != layers_.end(); ++it )
    {
        if( ( *it )->GetName() == name )
            return *it;
    }
    return 0;
}

const std::list< std::shared_ptr< Layer2D > >& SDL2DSceneManager::GetLayers() const
{
    return layers_;
}

void SDL2DSceneManager::RemoveLayer( std::string name )
{
    for( auto it = layers_.begin( ); it != layers_.end( ); ++it )
    {
        if( ( *it )->GetName() == name )
            layers_.remove( *it );
    }
}

void SDL2DSceneManager::SortLayer()
{
    layers_.sort( 
        []( std::shared_ptr< Layer2D > lhs, std::shared_ptr< Layer2D >  rhs )
            {
                return lhs->GetZOrder() < rhs->GetZOrder();
            } );
}


bool SDL2DSceneManager::LoadFromXMLFile( std::string )
{
    return true;
}

unsigned int SDL2DSceneManager::AddTimer( unsigned long interval )
{
    std::shared_ptr< OOTimer > timer = std::make_shared< OOTimer >( interval );
    timers_.push_back( timer );
    timer->Start();
    return timer->GetID();
}

void SDL2DSceneManager::AddListener( SceneListener_ABC* listener )
{
    listeners_.push_back( listener );
}

void SDL2DSceneManager::Update( )
{
    checkTimerExpiration();
}

void SDL2DSceneManager::checkTimerExpiration()
{
    for( auto it = timers_.begin(); it != timers_.end(); ++it )
    {
        ( *it )->Update();
        if( !( *it )->IsExpired() )
            continue;
        for( auto lit = listeners_.begin(); lit != listeners_.end(); ++lit )
        {
            if( ( *lit )->listenFor_ != TIMER_EXPIRED )
                continue;
            ( *lit )->Event( *this, NULL );
            ( *it )->Start();
        }
    }
}