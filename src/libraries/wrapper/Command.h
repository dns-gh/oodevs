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
#include "MT_Tools/MT_Profiler.h"
#include <module_api/Command.h>
#include <module_api/Hook.h>
#include <module_api/Log.h>
#include <boost/lexical_cast.hpp>
#include <string>
#include <list>

#define WRAPPER_TRY try {
#define WRAPPER_CATCH( tag ) \
    } \
    catch( const std::exception& e ) \
    { \
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, tools::GetExceptionMsg( e ).c_str() ); \
    } \
    catch( ... ) \
    { \
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown error while " tag " command" ); \
    }

namespace sword
{
namespace wrapper
{
namespace detail
{
    class Data
    {
    public:
        Data( const char* name, void* context )
            : name_        ( name )
            , context_     ( context )
            , creations_   ( 0 )
            , destructions_( 0 )
        {}
        template< typename T >
        T& GetContext() const
        {
            return *static_cast< T* >( context_ );
        }
        void Created()
        {
            ++creations_;
        }
        void Destroyed()
        {
            ++destructions_;
        }
        void Log()
        {
            if( creations_ || destructions_ || profiler_.GetCount() )
                ::SWORD_Log( SWORD_LOG_LEVEL_INFO, ( "<profiling> command " + name_ + " "
                    + boost::lexical_cast< std::string >( creations_ ) + " created "
                    + boost::lexical_cast< std::string >( destructions_ ) + " destroyed "
                    + boost::lexical_cast< std::string >( profiler_.GetCount() ) + " alive "
                    + boost::lexical_cast< std::string >( profiler_.GetTotalTime() ) + " ms" ).c_str() );
            creations_ = 0;
            destructions_ = 0;
            profiler_.Reset();
        }
        void Start()
        {
            profiler_.Start();
        }
        void Stop()
        {
            if( profiler_.IsStarted() )
                profiler_.Stop();
        }
    private:
        std::string name_;
        void* context_;
        std::size_t creations_;
        std::size_t destructions_;
        MT_Profiler profiler_;
    };

    class Commands
    {
    public:
        static bool& IsProfiling()
        {
            static bool profiling = false;
            return profiling;
        }
        static Data* MakeData( const char* name, void* context = 0 )
        {
            GetData().push_back( Data( name, context ) );
            return &GetData().back();
        }
    protected:
        Commands( const View& model )
        {
            IsProfiling() = model[ "profiling/command" ];
            if( IsProfiling() )
                SWORD_RegisterHook( 0, &GetPrevious(), &LogProfiling, "LogProfiling", "void()" );
        }
    private:
        static std::list< Data >& GetData()
        {
            static std::list< Data > data;
            return data;
        }
        static SWORD_Hook& GetPrevious()
        {
            static SWORD_Hook previous;
            return previous;
        }
        static void LogProfiling()
        {
            std::for_each( GetData().begin(), GetData().end(), std::mem_fun_ref( &Data::Log ) );
            if( GetPrevious() )
                GetPrevious()();
        }
    };

    template< typename T >
    struct ProfiledCommand
    {
        template< typename C >
        static void Create( void** command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* context )
        {
            static_cast< Data* >( context )->Created();
            Command< T >::Create< C >( command, parameters, model, context );
        }
        static void Create( void** command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* context )
        {
            static_cast< Data* >( context )->Created();
            Command< T >::Create( command, parameters, model, context );
        }
        static void Destroy( void* command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* context )
        {
            static_cast< Data* >( context )->Destroyed();
            Command< T >::Destroy( command, parameters, model, context );
        }
        static void Execute( const void* command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* context )
        {
            if( Commands::IsProfiling() )
                static_cast< Data* >( context )->Start();
            Command< T >::Execute( command, parameters, model, context );
            static_cast< Data* >( context )->Stop();
        }
        static void ExecutePaused( const void* command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* context )
        {
            if( Commands::IsProfiling() )
                static_cast< Data* >( context )->Start();
            Command< T >::Execute( command, parameters, model, context );
            static_cast< Data* >( context )->Stop();
        }
    };

    template< typename T >
    struct Command
    {
        template< typename C >
        static void Create( void** command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* context )
        {
            WRAPPER_TRY
                *command = new T( static_cast< Data* >( context )->GetContext< C >(), parameters, model );
            WRAPPER_CATCH( "creating" )
        }
        static void Create( void** command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* /*context*/ )
        {
            WRAPPER_TRY
                *command = new T( parameters, model );
            WRAPPER_CATCH( "creating" )
        }
        static void Destroy( void* command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* /*context*/ )
        {
            WRAPPER_TRY
                if( command )
                    static_cast< const T* >( command )->Destroy( parameters, model );
            WRAPPER_CATCH( "destroying" )
            delete static_cast< T* >( command );
        }
        static void Execute( const void* command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* /*context*/ )
        {
            WRAPPER_TRY
                if( ! command )
                    return;
                static_cast< const T* >( command )->Execute( parameters, model );
            WRAPPER_CATCH( "executing" )
        }
        static void ExecutePaused( const void* command,
            const SWORD_Model* parameters, const SWORD_Model* model, void* /*context*/ )
        {
            WRAPPER_TRY
                if( ! command )
                    return;
                static_cast< const T* >( command )->ExecutePaused( parameters, model );
            WRAPPER_CATCH( "executing" )
        }
    };
}

#undef WRAPPER_TRY
#undef WRAPPER_CATCH

template< typename T >
void RegisterCommand( const char* name )
{
    if( detail::Commands::IsProfiling() )
        ::SWORD_RegisterCommand( name, &detail::ProfiledCommand< T >::Create, &detail::ProfiledCommand< T >::Execute,
            &detail::ProfiledCommand< T >::ExecutePaused, &detail::ProfiledCommand< T >::Destroy, detail::Commands::MakeData( name ) );
    else
        ::SWORD_RegisterCommand( name, &detail::Command< T >::Create, &detail::Command< T >::Execute,
            &detail::Command< T >::ExecutePaused, &detail::Command< T >::Destroy, detail::Commands::MakeData( name ) );
}
template< typename T, typename C >
void RegisterCommand( const char* name, C& context )
{
    if( detail::Commands::IsProfiling() )
        ::SWORD_RegisterCommand( name, &detail::ProfiledCommand< T >::Create< C >, &detail::ProfiledCommand< T >::Execute,
            &detail::ProfiledCommand< T >::ExecutePaused, &detail::ProfiledCommand< T >::Destroy, detail::Commands::MakeData( name, &context ) );
    else
        ::SWORD_RegisterCommand( name, &detail::Command< T >::Create< C >, &detail::Command< T >::Execute,
            &detail::Command< T >::ExecutePaused, &detail::Command< T >::Destroy, detail::Commands::MakeData( name, &context ) );
}

}
}

#endif // WRAPPER_COMMAND_H
