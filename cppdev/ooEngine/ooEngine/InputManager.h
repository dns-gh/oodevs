#ifndef _INPUTMANAGER_
#define _INPUTMANAGER_

#include "SDL_keyboard.h"
#include "SDL_Keycode.h"
#include "LogTools.h"
#include <map>

class InputManager
{
public:
    InputManager( LogTools& logger );
    ~InputManager();
    void Bind( int, int );
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
    std::map< int, KeyState* > keyboardBinding_;
    LogTools& logger_;
};

#endif