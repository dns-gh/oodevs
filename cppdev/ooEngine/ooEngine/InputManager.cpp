#include "InputManager.h"
#include "SDL_events.h"
#include <utility>

void InputManager::Bind( int action, int key )
{
    keyboardBinding_[ action ] = new KeyState( key );    
}

bool InputManager::PerformAction( int action )
{
    auto it = keyboardBinding_.find( action );
    if( it != keyboardBinding_.end() && it->second->pressed_ )
        return true;
    return false;
}

void InputManager::Update()
{
    //SDL_PumpEvents();
    const Uint8 *keys = SDL_GetKeyboardState( NULL );
    for( auto& kb : keyboardBinding_ )
    {
        if( keys[ kb.second->key_ ] )
            kb.second->pressed_ = true;
        else
            kb.second->pressed_ = false;
    }
}