#include "SceneManager2D.h"
#include "SceneListener_ABC.h"

std::shared_ptr< Layer2D > SceneManager2D::CreateLayer( std::string name )
{
    std::shared_ptr< Layer2D > layer = FindLayer( name );

    if( !layer )
    {
        layer.reset( new Layer2D( name ) );
        layers_.push_back( layer );
    }
    return layer;
}

std::shared_ptr< Layer2D > SceneManager2D::FindLayer( std::string name ) const
{
    for( auto it = layers_.begin(); it != layers_.end(); ++it )
    {
        if( ( *it )->GetName() == name )
            return *it;
    }
    return 0;
}

const std::list< std::shared_ptr< Layer2D > >& SceneManager2D::GetLayers() const
{
    return layers_;
}

void SceneManager2D::RemoveLayer( std::string name )
{
    for( auto it = layers_.begin( ); it != layers_.end( ); ++it )
    {
        if( ( *it )->GetName() == name )
            layers_.remove( *it );
    }
}

void SceneManager2D::SortLayer()
{
    layers_.sort( 
        []( std::shared_ptr< Layer2D > lhs, std::shared_ptr< Layer2D >  rhs )
            {
                return lhs->GetZOrder() < rhs->GetZOrder();
            } );
}


bool SceneManager2D::LoadFromXMLFile( std::string )
{
    return true;
}

unsigned int SceneManager2D::AddTimer( unsigned long interval )
{
    std::shared_ptr< OOTimer > timer = std::make_shared< OOTimer >( interval );
    timers_.push_back( timer );
    timer->Start();
    return timer->GetID();
}

void SceneManager2D::AddListener( SceneListener_ABC* listener )
{
    listeners_.push_back( listener );
}

void SceneManager2D::Update()
{
    checkTimerExpiration();
}

void SceneManager2D::checkTimerExpiration()
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