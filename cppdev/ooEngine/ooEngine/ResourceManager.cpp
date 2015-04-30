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
            for( XMLNode* child = tree->FirstChild(); child; child->NextSibling() )
            {
                XMLElement* elem = child->ToElement();
                if( elem )
                {
                    Resource* resource = NULL;
                    for( const XMLAttribute* att = elem->FirstAttribute(); att; att->Next() )
                    {
                        std::string attName = att->Name();
                        std::string attValue = att->Value();
                        if( attName == "type" )
                        {
                            if( attValue == "graphic" )
                            {
                                // Create a resource by the graphic manager
                                resource = SDL2DRenderManager::GetInstance()->CreateRenderResource();
                            }
                            else if( attValue == "text" )
                            {
                                // load of the resource by the config manager
                                // resource = ConfigManager->LoadResourceFromXML( elem );
                            }
                        }

                        if( resource )
                        {
                            unsigned int id = -1;
                            unsigned int scope = -1;
                            std::string filename;
                            if( attName == "id" )
                            {
                                id = std::atoi( attValue.c_str() );
                            }
                            else if( attName == "scenescope" )
                            {
                                scope = std::atoi( attValue.c_str( ) );
                            }
                            else if( attName == "filename" )
                            {
                                filename = attValue;
                            }
                            if( !resource->Initialize( id, scope, filename ) )
                                OOTHROW( 2, "Error when loading a resource from " + filename + " XML file");
                        }
                    }
                    
                    if( resource )
                    {
                        resourceByScope_[ resource->GetResourceScope() ].push_back( resource );
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
        ( *it )->Load( );
    }
}

Resource* ResourceManager::FindResourceByID( const unsigned int& id ) const
{
    for( auto it = resourceByScope_.begin(); it != resourceByScope_.end(); it++ )
    {
        if( !( *it ).second.empty() )
        {
            for( auto listIt = ( *it ).second.begin(); listIt != ( *it ).second.end(); listIt++ )
            {
                if( ( *listIt )->GetResourceID( ) != id )
                    return *listIt;
            }
        }
    }
    return NULL;
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
                OODELETE( *listIt );
            }
            ( *it ).second.clear();
        }
    }
    resourceByScope_.clear();
    resourceCount_ = 0;
}