#ifndef _INPUTMANAGER_
#define _INPUTMANAGER_

#include "EngineObject.h"
#include "LogTools.h"

#include "SDL_keyboard.h"
#include "SDL_Keycode.h"

#include <map>
#include <vector>
#include <functional>

class InputManager : EngineObject
{
public:
    InputManager( LogTools& logger );
    ~InputManager();
    // Bind action (int) with a combination of SDL_SCANCODE (layout independant, keyboard only)
    void Bind( int, int );
    void UnBind( int, int );
    void Register( int, std::function<void()> );
    void Update();

private:
    struct KeyState
    {
        bool pressed_;
        int key_;
        KeyState( int key )
        {
            pressed_ = false;
            key_ = key;
        }
    };

private:
    // helpers
    bool PerformAction( int );
    void ExecuteCallbacks();
    bool isKeyInStateList( int key, const std::vector< KeyState* >& list );

private:
    std::map< int, std::vector< KeyState* > > keyboardBinding_;
    std::map< int, std::function<void()> > callbacks_;
    LogTools& logger_;
};

#endif