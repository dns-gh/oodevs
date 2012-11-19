// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef WRAPPER_HOOK_H
#define WRAPPER_HOOK_H

#include "View.h"
#include "MT_Tools/MT_ProfilerGuard.h"
#include <module_api/Hook.h>
#pragma warning( push, 0 )
#include <boost/preprocessor.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/mpl/at.hpp>
#pragma warning( pop )
#include <boost/bind.hpp>
#include <functional>
#include <algorithm>
#include <vector>
#include <string>

namespace sword
{
namespace wrapper
{
    class Hook_ABC
    {
    public:
        virtual void Apply( bool profiling ) = 0;
        virtual void Log()
        {}
    protected:
        virtual ~Hook_ABC()
        {}
    };

    class Hooks
    {
    public:
        static void Use( Hook_ABC* hook )
        {
            GetUses().push_back( hook );
        }
        static void Register( Hook_ABC* hook )
        {
            GetRegistrations().push_back( hook );
        }
    protected:
        Hooks( const View& model )
        {
            const bool profiling = model[ "profiling" ];
            std::for_each( GetUses().begin(), GetUses().end(), boost::bind( &Hook_ABC::Apply, _1, profiling ) );
            std::for_each( GetRegistrations().begin(), GetRegistrations().end(), boost::bind( &Hook_ABC::Apply, _1, profiling ) );
            if( profiling )
                SWORD_RegisterHook( 0, &GetPrevious(), &LogProfiling, "LogProfiling", "void()" );
        }
    private:
        static std::vector< Hook_ABC* >& GetUses()
        {
            static std::vector< Hook_ABC* > hooks;
            return hooks;
        }
        static std::vector< Hook_ABC* >& GetRegistrations()
        {
            static std::vector< Hook_ABC* > hooks;
            return hooks;
        }
        static SWORD_Hook& GetPrevious()
        {
            static SWORD_Hook previous;
            return previous;
        }
        static void LogProfiling()
        {
            std::for_each( GetRegistrations().begin(), GetRegistrations().end(), std::mem_fun( &Hook_ABC::Log ) );
            if( GetPrevious() )
                GetPrevious()();
        }
    };
}
}

#define DECLARE_HOOK( Hook, result, parameters ) \
    static struct Hook ## Wrapper : private sword::wrapper::Hook_ABC \
    { \
        typedef result result_type; \
        Hook ## Wrapper() \
        { \
            sword::wrapper::Hooks::Use( this ); \
        } \
        typedef result (*Function) parameters; \
        Function current_; \
    private: \
        virtual void Apply( bool /*profiling*/ ) \
        { \
            SWORD_UseHook( reinterpret_cast< SWORD_Hook* >( &current_ ), \
                           #Hook, #result #parameters ); \
        } \
    } Hook;

namespace sword
{
namespace wrapper
{
namespace detail
{
    template< typename Signature, int n >
    struct parameter
    {
        typedef BOOST_DEDUCED_TYPENAME
            boost::mpl::at_c<
                BOOST_DEDUCED_TYPENAME
                    boost::function_types::parameter_types< Signature >,
                n
            >::type type;
    };
}
}
}

#define HOOK_PARAM(z, n, d) \
    BOOST_PP_COMMA_IF(n) d, n >::type p##n
#define HOOK_PARAM_CALL(z, n, d) \
    BOOST_PP_COMMA_IF(n) d##n
#define HOOK_PARAMS(n, S ) \
    BOOST_PP_REPEAT(n, HOOK_PARAM, sword::wrapper::detail::parameter< S)
#define HOOK_DECL( n, S ) \
    ( HOOK_PARAMS(n, S ) )

#define DEFINE_HOOK( Hook, arity, result, parameters ) \
    static struct Hook ## Wrapper : private sword::wrapper::Hook_ABC \
    { \
        typedef result result_type; \
        Hook ## Wrapper() \
        { \
            BOOST_MPL_ASSERT_RELATION( arity, ==, \
                boost::function_types::function_arity< result parameters >::value );\
            sword::wrapper::Hooks::Register( this ); \
        } \
        typedef result (*Function) parameters; \
        Function current_; \
        Function previous_; \
        static result Implement parameters; \
        static result SafeProfiledImplement HOOK_DECL( arity, result parameters ) \
        { \
            MT_ProfilerGuard guard( GetProfiler() ); \
            return SafeImplement( BOOST_PP_REPEAT( arity, HOOK_PARAM_CALL, p ) ); \
        } \
        static result SafeImplement HOOK_DECL( arity, result parameters ) \
        { \
            try \
            { \
                return Implement( BOOST_PP_REPEAT( arity, HOOK_PARAM_CALL, p ) ); \
            } \
            catch( std::exception& e ) \
            { \
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, ( "Exception in " #Hook " hook: " + std::string( e.what() ) ).c_str() ); \
            } \
            catch( ... ) \
            { \
                ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Unknown exception in " #Hook " hook" ); \
            } \
            return boost::function_types::result_type< result parameters >::type(); \
        } \
    private: \
        virtual void Apply( bool profiling ) \
        { \
            SWORD_RegisterHook( reinterpret_cast< SWORD_Hook* >( &current_ ), \
                                reinterpret_cast< SWORD_Hook* >( &previous_ ), \
                                reinterpret_cast< SWORD_Hook >( profiling ? &SafeProfiledImplement : &SafeProfiledImplement ), \
                                #Hook, #result #parameters ); \
        } \
        virtual void Log() \
        { \
            MT_Profiler& profiler = GetProfiler(); \
            if( profiler.GetCount() == 0 ) \
                return; \
            ::SWORD_Log( SWORD_LOG_LEVEL_INFO, ( "<profiling> hook " #Hook " " \
                + boost::lexical_cast< std::string >( profiler.GetCount() ) + " calls " \
                + boost::lexical_cast< std::string >( profiler.GetTotalTime() ) + " ms" ).c_str() ); \
            profiler.Reset(); \
        } \
        static MT_Profiler& GetProfiler() \
        { \
            static MT_Profiler profiler; \
            return profiler; \
        } \
    } Hook; \
    result Hook ## Wrapper::Implement parameters

#define GET_HOOK( Hook ) Hook.current_

#define GET_PREVIOUS_HOOK( Hook ) Hook.previous_

#endif // WRAPPER_HOOK_H
