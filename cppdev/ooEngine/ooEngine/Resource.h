#ifndef _RESOURCE_H
#define _RESOURCE_H

#include "EngineObject.h"
#include <string>

typedef enum {
    RESOURCE_NUL = 0,
    RESOURCE_GRAPHIC = 1,
}RESOURCE_TYPE;

class Resource : public EngineObject
{
public:
    Resource()
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
    unsigned int GetResourceScope() const { return scope_; }
    virtual ~Resource(){};
    virtual void Load(){};
    virtual void UnLoad(){};

private:
    unsigned int id_;
    unsigned int scope_;
    std::string filename_;
    RESOURCE_TYPE type_;
};

#endif // Resource.h