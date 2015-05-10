#ifndef _SCENE_MANAGER_2D_ABC_H_
#define _SCENE_MANAGER_2D_ABC_H_

#include "Layer2D.h"
#include <memory>

class SceneListener_ABC;

class SceneManager2D_ABC
{
public:
    virtual ~SceneManager2D_ABC(){};
    virtual std::shared_ptr< Layer2D > CreateLayer( std::string ) = 0;
    virtual std::shared_ptr< Layer2D > FindLayer( std::string ) = 0;
    virtual void RemoveLayer( std::string name ) = 0;
    virtual void SortLayer( ) = 0;
    virtual bool LoadFromXMLFile( std::string filename ) = 0;
    virtual unsigned int AddTimer( unsigned long interval ) = 0; // return the EngineObject id
    virtual void AddListener( SceneListener_ABC* listener ) = 0;
    virtual void Update() = 0;
};

#endif // SceneManager2D_ABC.h