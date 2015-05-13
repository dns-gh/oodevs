#include "SceneManager2D.h"
#include "SceneListener_ABC.h"
#include "ErrorLogManager.h"

#include "SDLRenderObject.h" // TOREMOVE

using namespace tinyxml2;

SceneManager2D::SceneManager2D( const std::shared_ptr< ResourceManager >& resourceManager )
: resourceManager_( resourceManager )
{
    // NOTHING
}

std::shared_ptr< Layer2D > SceneManager2D::CreateLayer( std::string name )
{
    std::shared_ptr< Layer2D > layer = FindLayer( name );

    if( !layer )
        return 0;
    layer.reset( new Layer2D( false, 0, 0, 0, name ) );
    layers_.push_back( layer );
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

bool SceneManager2D::LoadFromXMLFile( std::string filename )
{
    tinyxml2::XMLDocument doc;
    if( doc.LoadFile( filename.c_str() ) != XMLError::XML_SUCCESS )
        return false;

    XMLNode* tree = doc.FirstChildElement( "scene" );
    if( !tree )
        return false;

    for( XMLNode* child = tree->FirstChild(); child; child = child->NextSibling() )
    {
        XMLElement* elem = child->ToElement( );
        if( !elem )
            continue;

        std::shared_ptr< Layer2D > layer;
        if( !elem->Attribute( "name" ) || !elem->Attribute( "zorder" ) || !elem->Attribute( "x" ) || !elem->Attribute( "y" ) )
            OOTHROW( 2, "Error when loading a layer, no name, zorder, x or y position provided" );
        bool visible = false;
        if( elem->Attribute( "visible" ) )
            visible = elem->Attribute( "visible" ) == "true";
        std::string name = elem->Attribute( "name" );
        unsigned int zorder = std::atoi( elem->Attribute( "zorder" ) );
        float x = static_cast< float >( std::atof( elem->Attribute( "x" ) ) );
        float y = static_cast< float >( std::atof( elem->Attribute( "y" ) ) );
        layer.reset( new Layer2D( visible, zorder, x, y, name ) );
        for( XMLNode* objects = child->FirstChild( ); objects; objects = objects->NextSibling( ) )
        {
            if( std::string( objects->Value( ) ) != "objects" )
                continue;

            for( XMLNode* obj = objects->FirstChild( ); obj; obj = obj->NextSibling( ) )
                AddLayerObject( layer, obj->ToElement() );
        }
        layers_.push_back( layer );
    }
    return true;
}

void SceneManager2D::AddLayerObject( std::shared_ptr< Layer2D >& layer, tinyxml2::XMLElement* elem )
{
    if( elem->Value() != std::string( "object" ) )
        return;
    std::shared_ptr< SceneObject > object( new SDLRenderObject() );

    if( !elem->Attribute( "id" ) )
        OOTHROW( 2, "No id provided for the loading of the object in the layer" );
    
    auto resource = resourceManager_->FindResourceByID( std::atoi( elem->Attribute( "id" ) ) );
    if( !resource )
        OOTHROW( 2, "Error when searching a resource from its id" );
    object->SetResourceObject( resource );
    
    if( elem->Attribute( "x" ) && elem->Attribute( "y" ) )
        object->SetPosition( static_cast< float >( std::atof( elem->Attribute( "x" ) ) ), static_cast< float >( std::atof( elem->Attribute( "y" ) ) ) );

    bool colorkey = false;
    if( elem->Attribute( "colorkey" ) == "true" )
    {
        colorkey = true;
    } // colorkey false by default
        
    if( elem->Attribute( "visible" ) == "true" )
    {
        object->SetVisible( true );
    } // not visible by default
    
    int r = 0, g = 0, b = 0;
    if( elem->Attribute( "r" ) && elem->Attribute( "g" ) && elem->Attribute( "b" ) )
    {
        r = std::atoi( elem->Attribute( "r" ) );
        g = std::atoi( elem->Attribute( "g" ) );
        b = std::atoi( elem->Attribute( "b" ) );
    }
    object->SetColorKeying( colorkey, r, g, b );
    layer->InsertSceneObject( object );
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