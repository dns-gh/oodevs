#ifndef _OOENGINE_H_
#define _OOENGINE_H_

#include "Manager_ABC.h"

#include <memory>
#include <vector>

class EntityFactory_ABC;
class InputManager;
class LogTools;
class RenderManager_ABC;
class ResourceManager;
class SceneManager2D;

class ooEngine
{
public:
    ooEngine();
    virtual ~ooEngine();
    static std::shared_ptr< ooEngine > CreateEngine();

    // -- API --
    void Run();
    void Pause();
    void Resume();
    void Stop();

    std::shared_ptr< LogTools > GetLogger() const;
    std::shared_ptr< RenderManager_ABC > GetRenderManager2D() const;
    std::shared_ptr< ResourceManager > GetResourceManager() const;
    std::shared_ptr< SceneManager2D > GetSceneManager2D() const;
    std::shared_ptr< InputManager > GetInputManager() const;

private:
    // main loop
    void MainLoop();
    void Initialize();

private:
    bool running_;
    bool paused_;
    std::shared_ptr< LogTools > logger_;
    std::shared_ptr< RenderManager_ABC > renderManager2D_;
    std::shared_ptr< ResourceManager > resourcesManager_;
    std::shared_ptr< EntityFactory_ABC > entityFactory_;
    std::shared_ptr< SceneManager2D > sceneManager2D_;
    std::shared_ptr< InputManager > inputManager_;
};

#endif // ooEngine.h