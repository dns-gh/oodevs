#include "ResourceManager.h"
#include "SDL2DRenderManager.h"
#include "tinyxml2.h"

using namespace tinyxml2;

// TODO : add of a memory budget to allow caching of resources from multiple scopes

ResourceManager::ResourceManager()
{
    currentScope_ = 0;
    resourceCount_ = 0;
}

ResourceManager::~ResourceManager( )
{
    Clear();
    currentScope_ = 0;
    resourceCount_ = 0;
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
                XMLElement* elem = child->ToElement();
                if( elem )
                {
                    std::shared_ptr< Resource_ABC > resource;
                    std::string attValue = elem->Attribute( "type" );
                    RESOURCE_TYPE type;
                    if( attValue == "graphic" )
                    {
                        // Create a resource by the graphic manager
                        resource = SDL2DRenderManager::GetInstance()->CreateRenderResource();
                        type = RESOURCE_GRAPHIC;
                    }
                    else if( attValue == "text" )
                    {
                        // load of the resource by the config manager
                        // resource = ConfigManager->LoadResourceFromXML( elem );
                    }

                    if( resource )
                    {
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