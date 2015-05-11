#ifndef SDL2D_SCENE_MANAGER_H_
#define SDL2D_SCENE_MANAGER_H_

#include "EngineObject.h"
#include "Layer2D.h"
#include "SceneManager2D_ABC.h"

#include "tinyxml2.h"

#include <map>

class SceneListener_ABC;

class SDL2DSceneManager : public EngineObject
                        , public SceneManager2D_ABC
{
public:
    virtual std::shared_ptr< Layer2D > CreateLayer( std::string );
    virtual std::shared_ptr< Layer2D > FindLayer( std::string ) const;
    virtual const std::list< std::shared_ptr< Layer2D > >& GetLayers( ) const;
    virtual void RemoveLayer( std::string name );
    virtual void SortLayer( );
    virtual bool LoadFromXMLFile( std::string filename );
    virtual unsigned int AddTimer( unsigned long interval );
    virtual void AddListener( SceneListener_ABC* listener );
    virtual void Update();

private:
    void addLayerObjects( Layer2D& layer, tinyxml2::XMLElement* element );
    void checkTimerExpiration();
};

#endif // SDL2DSceneManager.h