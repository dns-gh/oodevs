#include "ResourceManager.h"

using namespace tinyxml2;

ResourceManager::ResourceManager()
{
    currentScope_ = 0;
    resourceCount_ = 0;
}

unsigned int ResourceManager::GetResourceCount() const
{
    return resourceCount_;
}

bool ResourceManager::LoadFromXMLFile( const std::string& filename )
{
    XMLDocument doc;
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
                                // load of the resource by the graphic manager
                                // resource = RenderManager->LoadResourceFromXML( elem );
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