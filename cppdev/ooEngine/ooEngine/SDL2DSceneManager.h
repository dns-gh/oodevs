#ifndef SDL2D_SCENE_MANAGER_H_
#define SDL2D_SCENE_MANAGER_H_

#include "EngineObject.h"
#include "Layer2D.h"
#include "OOTimer.h"
#include "SceneManager2D_ABC.h"

#include "tinyxml2.h"

#include <map>

class SceneListener_ABC;

class SDL2DSceneManager : public EngineObject
                        , public SceneManager2D_ABC
{
public:
    std::shared_ptr< Layer2D > CreateLayer( std::string );
    std::shared_ptr< Layer2D > FindLayer( std::string );
    void RemoveLayer( std::string name );
    void SortLayer();
    bool LoadFromXMLFile( std::string filename );
    unsigned int AddTimer( unsigned long interval );
    void AddListener( SceneListener_ABC* listener );
    void Update();

private:
    void addLayerObjects( Layer2D& layer, tinyxml2::XMLElement* element );
    void checkTimerExpiration();

    std::list< std::shared_ptr< Layer2D > > layers_;
    std::list< std::shared_ptr< OOTimer > > timers_;
    std::list< SceneListener_ABC* > listeners_;
};

#endif // SDL2DSceneManager.h