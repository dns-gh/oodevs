#ifndef _RESOURCEMANAGER_H
#define _RESOURCEMANAGER_H

#include "tools.h"
#include "EngineObject.h"
#include "ErrorLogManager.h"
#include <iostream>
#include <list>
#include <map>
#include <string>

typedef enum {
    RESOURCE_NUL = 0,
    RESOURCE_GRAPHIC = 1,
}RESOURCE_TYPE;

class Resource : public EngineObject
{
public:
    Resource( )
    {
        id_ = -1;
        scope_ = -1;
        type_ = RESOURCE_NUL;
    }
    bool Initialize( unsigned id, unsigned scope, std::string filename )
    {
        id_ = id;
        scope_ = scope;
        filename_ = filename;
        if( id != -1 && scope != -1 && filename != "" )
            return true;
        return false;
    }
    unsigned int GetResourceID() const { return id_; }
    unsigned int GetResourceScope( ) const { return scope_; }
    virtual ~Resource(){};
    virtual void Load(){};
    virtual void UnLoad(){};

private:
    unsigned int id_;
    unsigned int scope_;
    std::string filename_;
    RESOURCE_TYPE type_;
};

class ResourceManager : public EngineObject
{
public:
    ResourceManager();

    bool LoadFromXMLFile( const std::string& filename );
    void SetCurrentScope( unsigned int scope );
    Resource* FindResourceByID( const unsigned int& id ) const;
    unsigned int GetResourceCount() const;
    void Clear();

private:
    unsigned int currentScope_;
    unsigned int resourceCount_;
    std::map< unsigned int, std::list< Resource* > > resourceByScope_;
};

#endif