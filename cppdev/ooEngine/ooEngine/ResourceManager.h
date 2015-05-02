#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "tools.h"
#include "Resource_ABC.h"
#include "ErrorLogManager.h"
#include <iostream>
#include <list>
#include <map>
#include <string>

class ResourceManager : public EngineObject
{
public:
    ResourceManager();
    ~ResourceManager();

    bool LoadFromXMLFile( const std::string& filename );
    void SetCurrentScope( unsigned int scope );
    Resource_ABC* FindResourceByID( const unsigned int& id ) const;
    unsigned int GetResourceCount() const;
    void Clear();

private:
    unsigned int currentScope_;
    unsigned int resourceCount_;
    std::map< unsigned int, std::list< Resource_ABC* > > resourceByScope_;
};

#endif