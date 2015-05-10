#ifndef _RESOURCE_ABC_H
#define _RESOURCE_ABC_H

#include <string>

typedef enum {
    RESOURCE_NUL = 0,
    RESOURCE_GRAPHIC = 1,
}RESOURCE_TYPE;

class Resource_ABC
{
public:
    Resource_ABC()
    {
        id_ = -1;
        scope_ = -1;
        type_ = RESOURCE_NUL;
    }
    bool Initialize( unsigned id, unsigned scope, std::string filename, RESOURCE_TYPE type )
    {
        id_ = id;
        scope_ = scope;
        filename_ = filename;
        type_ = type;
        if( id != -1 && scope != -1 && filename != "" && type != RESOURCE_NUL )
            return true;
        return false;
    }
    void Reset()
    {
        id_ = -1;
        scope_ = -1;
        type_ = RESOURCE_NUL;
    }
    unsigned int GetResourceID() const { return id_; }
    unsigned int GetResourceScope() const { return scope_; }
    std::string GetResourceFilename() const { return filename_; }
    RESOURCE_TYPE GetResourceType() const { return type_; };
    virtual ~Resource_ABC(){};
    virtual void Load() = 0;
    virtual void UnLoad() = 0;

private:
    unsigned int id_;
    unsigned int scope_;
    std::string filename_;
    RESOURCE_TYPE type_;
};

#endif // Resource_ABC.h