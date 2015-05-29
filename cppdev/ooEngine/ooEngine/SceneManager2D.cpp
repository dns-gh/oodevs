#include "SceneManager2D.h"
#include "SceneListener_ABC.h"
#include "ErrorLogManager.h"

using namespace tinyxml2;

SceneManager2D::SceneManager2D( const ResourceManager& resourceManager, const CollisionSolver& solver, const EntityFactory_ABC& entityFactory, LogTools& logger )
    : resourceManager_( resourceManager )
    , collisionSolver_( solver )
    , entityFactory_( entityFactory )
    , logger_( logger )
{
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Scene manager 2D: creation" );
}

SceneManager2D::~SceneManager2D()
{
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Scene manager: clear layers, timers, listeners & destruction" );
}

std::shared_ptr< Layer2D > SceneManager2D::CreateLayer( std::string name )
{
    std::shared_ptr< Layer2D > layer = FindLayer( name );
    if( !layer )
    {
        layer.reset( new Layer2D( true, 0, 0, 0, name ) );
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

bool SceneManager2D::LoadFromXMLFile( std::string filename )
{
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Scene manager 2D: loading data from xml file: " << filename );
    tinyxml2::XMLDocument doc;
    if( doc.LoadFile( filename.c_str() ) != XMLError::XML_SUCCESS )
    {
        logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_ERROR, "Scene manager 2D: error in loading the file..." );
        return false;
    }

    XMLNode* tree = doc.FirstChildElement( "scene" );
    if( !tree )
    {
        logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_ERROR, "Scene manager 2D: error in loading the file... <scene> node dosen't exists" );
        return false;
    }

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
            elem->QueryBoolAttribute( "visible", &visible );
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
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Scene manager 2D: loaded successfully" );
    return true;
}

void SceneManager2D::AddLayerObject( std::shared_ptr< Layer2D >& layer, tinyxml2::XMLElement* elem )
{
    if( elem->Value() != std::string( "object" ) )
        return;
    std::shared_ptr< SceneObject > object = std::make_shared< SceneObject >( this ) ;

    bool sprite = false;
    if( elem->Attribute( "sprite" ) )
        elem->QueryBoolAttribute( "sprite", &sprite );

    if( sprite )
    {
        int num, rows, cols, time;
        if( elem->Attribute( "sprite_num" ) && elem->Attribute( "sprite_rows" )
            && elem->Attribute( "sprite_cols" ) && elem->Attribute( "sprite_time" ) )
        {
            num = std::atoi( elem->Attribute( "sprite_num" ) );
            rows = std::atoi( elem->Attribute( "sprite_rows" ) );
            cols = std::atoi( elem->Attribute( "sprite_cols" ) );
            time = std::atoi( elem->Attribute( "sprite_time" ) );
        }
        else
            OOTHROW( 2, "Missing data for the sprite object to be loaded" );
        object = entityFactory_.CreateSpriteObject( num, rows, cols, time );
    }
    else
        object = entityFactory_.CreateRenderObject();

    if( !elem->Attribute( "id" ) )
        OOTHROW( 2, "No id provided for the loading of the object in the layer" );
    
    auto resource = resourceManager_.FindResourceByID( std::atoi( elem->Attribute( "id" ) ) );
    if( !resource )
        OOTHROW( 2, "Error when searching a resource from its id" );
    object->SetResourceObject( resource );
    
    if( elem->Attribute( "x" ) && elem->Attribute( "y" ) )
        object->SetPosition( static_cast< float >( std::atof( elem->Attribute( "x" ) ) ), static_cast< float >( std::atof( elem->Attribute( "y" ) ) ) );

    bool colorkey = false;
    if( elem->Attribute( "colorkey" ) )
        elem->QueryBoolAttribute( "colorkey", &colorkey );

    bool visible = false;
    if( elem->Attribute( "visible" ) )
        elem->QueryBoolAttribute( "visible", &visible );
    object->SetVisible( visible );
    
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
    std::shared_ptr< Timer > timer = std::make_shared< Timer >( interval );
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

bool SceneManager2D::CheckCollisions( const SceneObject& object, const Circle& circle ) const
{
    for( auto& layer : layers_ )
    {
        for( auto& it : layer->GetSceneObjects() )
        {              
            if( !it->GetFilter().Filters( object.GetFilter() ) )
                continue;

            if( object.GetID() != it->GetID() && collisionSolver_.CollisionBetween( circle, it->GetCircleCollisionShape() ) )
                return true;
        }
    }
    return false;
}

void SceneManager2D::Log( const std::string& msg ) const
{
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, msg.c_str() );
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