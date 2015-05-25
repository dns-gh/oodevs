#include "ResourceManager.h"
#include "tinyxml2.h"

using namespace tinyxml2;

// TODO : add of a memory budget to allow caching of resources from multiple scopes

ResourceManager::ResourceManager( const RenderManager_ABC& renderManager, LogTools& logger )
    : renderManager_( renderManager )
    , logger_( logger )
{
    currentScope_ = 0;
    resourceCount_ = 0;
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Resource Manager: creation (id:" << GetID() << ")" );
}

ResourceManager::~ResourceManager()
{
    Clear();
    currentScope_ = 0;
    resourceCount_ = 0;
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Resource Manager: clear resources for each scope & destruction" );
}

bool ResourceManager::LoadFromXMLFile( const std::string& filename )
{
    tinyxml2::XMLDocument doc;
    if( doc.LoadFile( filename.c_str( ) ) == XMLError::XML_SUCCESS )
    {
        XMLNode* tree = doc.FirstChildElement( "resources" );
        if( tree )
        {
            for( XMLNode* child = tree->FirstChild( ); child; child = child->NextSibling( ) )
            {
                if( std::string( child->Value() ) != "resource" )
                    continue;
                XMLElement* elem = child->ToElement();
                if( elem )
                {
                    std::shared_ptr< Resource_ABC > resource;
                    std::string attValue = elem->Attribute( "type" );
                    RESOURCE_TYPE type;
                    if( attValue == "graphic" )
                    {
                        // Create a resource by the graphic manager
                        resource = renderManager_.CreateRenderResource();
                        type = RESOURCE_GRAPHIC;
                    }
                    else if( attValue == "text" )
                    {
                        // load of the resource by the config manager
                        // resource = ConfigManager->LoadResourceFromXML( elem );
                    }

                    if( resource )
                    {
                        if( !elem->Attribute( "id" ) || !elem->Attribute( "scenescope" ) || !elem->Attribute( "filename" ) )
                            OOTHROW( 2, "Error when loading a resource, no id, scenescope or filename provided" );
                        unsigned int id = std::atoi( elem->Attribute( "id" ) );
                        unsigned int scope = std::atoi( elem->Attribute( "scenescope" ) );
                        std::string filename = elem->Attribute( "filename" );
                        std::string filepath = tools::GetModulePath() + std::string( "/../../data/graphic/" ) + filename;
                        if( !resource->Initialize( id, scope, filepath, type ) )
                            OOTHROW( 2, "Error when loading a resource from " + filepath + " XML file" );
                        resourceByScope_[ resource->GetResourceScope( ) ].push_back( resource );
                        resourceCount_++;
                    }
                }
            }
            logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Resource Manager: load data from XML file: " << filename );
            return true;
        }
    }
    return false;
}

void ResourceManager::SetCurrentScope( unsigned int scope )
{
    if( currentScope_ != 0 ) // different from game scope
    {
        for( auto it = resourceByScope_[ currentScope_ ].begin( ); it != resourceByScope_[ currentScope_ ].end( ); it++ )
        {
            ( *it )->UnLoad();
        }
    }
    currentScope_ = scope;
    for( auto it = resourceByScope_[ currentScope_ ].begin( ); it != resourceByScope_[ currentScope_ ].end( ); it++ )
    {
        ( *it )->Load();
    }
}

std::shared_ptr< Resource_ABC > ResourceManager::FindResourceByID( const unsigned int& id ) const
{
    for( auto it = resourceByScope_.begin(); it != resourceByScope_.end(); it++ )
    {
        if( !( *it ).second.empty() )
        {
            for( auto listIt = ( *it ).second.begin(); listIt != ( *it ).second.end(); listIt++ )
            {
                if( ( *listIt )->GetResourceID() == id )
                    return *listIt;
            }
        }
    }
    return 0;
}

unsigned int ResourceManager::GetResourceCount() const
{
    return resourceCount_;
}

void ResourceManager::Clear()
{
    for( auto it = resourceByScope_.begin(); it != resourceByScope_.end(); it++ )
    {
        if( !( *it ).second.empty( ) )
        {
            for( auto listIt = ( *it ).second.begin( ); listIt != ( *it ).second.end( ); listIt++ )
            {
                ( *listIt )->UnLoad();
            }
            ( *it ).second.clear();
        }
    }
    resourceByScope_.clear();
    resourceCount_ = 0;
}