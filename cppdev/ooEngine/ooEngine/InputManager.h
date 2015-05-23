#ifndef _INPUTMANAGER_
#define _INPUTMANAGER_

#include "SDL_keyboard.h"
#include "SDL_Keycode.h"
#include "LogTools.h"
#include <map>
#include <vector>

class InputManager
{
public:
    InputManager( LogTools& logger );
    ~InputManager();
    // Bind action (int) with a combination of SDL_SCANCODE (layout independant, keyboard only)
    void Bind( int, int );
    void UnBind( int, int ){} // TODO
    bool PerformAction( int );
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
    bool isKeyInStateList( int key, const std::vector< KeyState* >& list );

private:
    std::map< int, std::vector< KeyState* > > keyboardBinding_;
    LogTools& logger_;
};

#endif