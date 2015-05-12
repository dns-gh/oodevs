#ifndef _SCENE_MANAGER_2D_H_
#define _SCENE_MANAGER_2D_H_

#include "EngineObject.h"
#include "Layer2D.h"
#include "OOTimer.h"
#include "ResourceManager.h"

#include "tinyxml2.h"

#include <memory>

class SceneListener_ABC;

class SceneManager2D : public EngineObject
{
public:
    SceneManager2D( const std::shared_ptr< ResourceManager >& resourceManager );
    virtual ~SceneManager2D(){};
    std::shared_ptr< Layer2D > CreateLayer( std::string );
    std::shared_ptr< Layer2D > FindLayer( std::string ) const;
    const std::list< std::shared_ptr< Layer2D > >& GetLayers() const;
    void RemoveLayer( std::string name );
    void SortLayer();
    bool LoadFromXMLFile( std::string filename );
    unsigned int AddTimer( unsigned long interval ); // return the EngineObject id
    void AddListener( SceneListener_ABC* listener );
    void Update();

private:
    void AddLayerObject( std::shared_ptr< Layer2D >& layer, tinyxml2::XMLElement* element );
    void checkTimerExpiration();

    std::list< std::shared_ptr< Layer2D > > layers_;
    std::list< std::shared_ptr< OOTimer > > timers_;
    std::list< SceneListener_ABC* > listeners_;
    const std::shared_ptr< ResourceManager >& resourceManager_;
};

#endif // SceneManager2D.h