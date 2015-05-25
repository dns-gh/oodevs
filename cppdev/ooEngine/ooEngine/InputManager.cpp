#include "InputManager.h"
#include "SDL_events.h"

#include <utility>
#include <algorithm>

InputManager::InputManager( LogTools& logger )
    : logger_( logger )
{
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Input Manager: creation (id:" << GetID() << ")" );
}

InputManager::~InputManager()
{
    for( auto& bd : keyboardBinding_ )
        for( auto it = bd.second.begin(); it != bd.second.end(); ++it )
            OODELETE( *it );
    logger_.OOLOG( FILE_INFOS ) << OOSTREAM( LOG_MESSAGE, "Input Manager: clear key states bindings & destruction" );
}

void InputManager::Bind( int action, int key )
{
    const auto it = keyboardBinding_.find( action );
    if( it != keyboardBinding_.end() )
    {
        if( !isKeyInStateList( key, it->second ) )
            keyboardBinding_[ action ].push_back( new KeyState( key ) );
        return;
    }
    keyboardBinding_[ action ].push_back( new KeyState( key ) );
}

void InputManager::UnBind( int action, int key )
{
    const auto pair = keyboardBinding_.find( action );
    if( pair != keyboardBinding_.end() )
    {
        auto it = std::find_if( pair->second.begin(), pair->second.end(),
            [&key]( const KeyState* state )
            {
                return state->key_ == key;
            }
        );
        if( it != pair->second.end() )
            pair->second.erase( it );
        if( pair->second.size() == 0 )
            keyboardBinding_.erase( pair );
    }
}

bool InputManager::PerformAction( int action )
{
    auto it = keyboardBinding_.find( action );
    if( it != keyboardBinding_.end() )
    {
        if( it->second.size() != 0 )
        {
            for( auto& key : it->second )
            if( !key->pressed_ )
                return false;
            return true;
        }
    }
    return false;
}

void InputManager::Update()
{
    SDL_PumpEvents(); // done implicitly by SDL_PollEvent
    const Uint8 *keys = SDL_GetKeyboardState( NULL );
    for( auto& kb : keyboardBinding_ )
    {
        for( auto& key : kb.second )
        {
            if( keys[ key->key_ ] )
                key->pressed_ = true;
            else
                key->pressed_ = false;
        }
    }
}

bool InputManager::isKeyInStateList( int key, const std::vector< KeyState* >& list )
{
    for( auto& it : list )
    {
        if( it->key_ == key )
            return true;
    }
    return false;
}