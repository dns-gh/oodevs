// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef WRAPPER_COMMAND_H
#define WRAPPER_COMMAND_H

#include "View.h"
#include <module_api/Command.h>
#include <module_api/Log.h>
#include <stdexcept>

namespace sword
{
namespace wrapper
{
namespace detail
{
    template< typename T, typename C = void >
    struct Command
    {
        static void Create( void** command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* context )
        {
            try
            {
                *command = new T( *static_cast< C* >( context ), parameters, model );
            }
            catch( std::exception& e )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
            }
            catch( ... )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown error while creating command" );
            }
        }
        static void Destroy( void* command,
            const SWORD_Model* /*parameters*/, const SWORD_Model* /*model*/, void* /*context*/ )
        {
            delete static_cast< T* >( command );
        }
        static void Execute( const void* command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* /*context*/ )
        {
            try
            {
                if( command )
                    static_cast< const T* >( command )->Execute( parameters, model );
            }
            catch( std::exception& e )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
            }
            catch( ... )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown error while executing command" );
            }
        }
        static void ExecutePaused( const void* command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* /*context*/ )
        {
            try
            {
                if( command )
                    static_cast< const T* >( command )->ExecutePaused( parameters, model );
            }
            catch( std::exception& e )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
            }
            catch( ... )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown error while executing command" );
            }
        }
    };

    template< typename T >
    struct Command< T, void >
    {
        static void Create( void** command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* /*userData*/ )
        {
            try
            {
                *command = new T( parameters, model );
            }
            catch( std::exception& e )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
            }
            catch( ... )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown error while creating command" );
            }
        }
        static void Destroy( void* command,
            const SWORD_Model* /*parameters*/, const SWORD_Model* /*model*/, void* /*context*/ )
        {
            delete static_cast< T* >( command );
        }
        static void Execute( const void* command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* /*userData*/ )
        {
            try
            {
                if( command )
                    static_cast< const T* >( command )->Execute( parameters, model );
            }
            catch( std::exception& e )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
            }
            catch( ... )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown error while executing command" );
            }
        }
        static void ExecutePaused( const void* command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* /*userData*/ )
        {
            try
            {
                if( command )
                    static_cast< const T* >( command )->ExecutePaused( parameters, model );
            }
            catch( std::exception& e )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, e.what() );
            }
            catch( ... )
            {
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown error while executing command" );
            }
        }
    };
}

template< typename T >
void RegisterCommand( const std::string& name )
{
    ::SWORD_RegisterCommand( name.c_str(), &detail::Command< T >::Create, &detail::Command< T >::Execute,
        &detail::Command< T >::ExecutePaused, &detail::Command< T >::Destroy, 0 );
}
template< typename T, typename C >
void RegisterCommand( const std::string& name, C& context )
{
    ::SWORD_RegisterCommand( name.c_str(), &detail::Command< T, C >::Create, &detail::Command< T, C >::Execute,
        &detail::Command< T, C >::ExecutePaused, &detail::Command< T, C >::Destroy, &context );
}

}
}

#endif // WRAPPER_COMMAND_H
