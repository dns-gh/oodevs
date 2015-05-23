#ifndef _INPUTMANAGER_
#define _INPUTMANAGER_

#include "SDL_keyboard.h"
#include "SDL_Keycode.h"
#include <map>

class InputManager
{
public:
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
};

#endif