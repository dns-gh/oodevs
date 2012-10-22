// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_HOOK_TOOLS_H
#define SWORD_HOOK_TOOLS_H

#include "MT_Tools/MT_Logger.h"
#include <core/Facade.h>
#include <algorithm>
#include <vector>
#include <boost/bind.hpp>
#include <boost/preprocessor.hpp>
#include <boost/function_types/parameter_types.hpp>
#include <boost/function_types/result_type.hpp>
#include <boost/function_types/function_arity.hpp>
#include <boost/mpl/at.hpp>

namespace sword // $$$$ _RC_ SLI 2012-10-22: DRY with wrapper/Hook.h
{
    class Hook_ABC
    {
    public:
        virtual void Apply( core::Facade& facade ) = 0;
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
        static void Initialize( core::Facade& facade )
        {
            std::for_each( GetUses().begin(), GetUses().end(), boost::bind( &Hook_ABC::Apply, _1, boost::ref( facade ) ) );
            std::for_each( GetRegistrations().begin(), GetRegistrations().end(), boost::bind( &Hook_ABC::Apply, _1, boost::ref( facade ) ) );
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
    };
}

#define DECLARE_HOOK( Hook, result, parameters ) \
    static struct Hook ## Wrapper : private sword::Hook_ABC \
    { \
        typedef result result_type; \
        Hook ## Wrapper() \
        { \
            sword::Hooks::Use( this ); \
        } \
        typedef result (*Function) parameters; \
        operator Function const() \
        { \
            return current_; \
        } \
        Function current_; \
    private: \
        virtual void Apply( core::Facade& facade ) \
        { \
            facade.UseHook( &current_, #Hook, #result #parameters ); \
        } \
} Hook##_;

namespace sword
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

#define HOOK_PARAM(z, n, d) \
    BOOST_PP_COMMA_IF(n) d, n >::type p##n
#define HOOK_PARAM_CALL(z, n, d) \
    BOOST_PP_COMMA_IF(n) d##n
#define HOOK_PARAMS(n, S ) \
    BOOST_PP_REPEAT(n, HOOK_PARAM, sword::detail::parameter< S)
#define HOOK_DECL( n, S ) \
    ( HOOK_PARAMS(n, S ) )

#define DEFINE_HOOK( Hook, arity, result, parameters ) \
    static struct Hook ## Wrapper : private sword::Hook_ABC \
    { \
        typedef result result_type; \
        Hook ## Wrapper() \
        { \
            BOOST_MPL_ASSERT_RELATION( arity, ==, \
                boost::function_types::function_arity< result parameters >::value );\
            sword::Hooks::Register( this ); \
        } \
        typedef result (*Function) parameters; \
        operator Function const() \
        { \
            return current_; \
        } \
        Function current_; \
        Function Previous; \
        static result Implement parameters; \
        static result SafeImplement HOOK_DECL( arity, result parameters ) \
        { \
            try\
            {\
                return Implement( BOOST_PP_REPEAT( arity, HOOK_PARAM_CALL, p ) ); \
            }\
            catch( std::exception& e )\
            {\
                MT_LOG_ERROR_MSG( "Exception in hook " #Hook << e.what() ); \
            }\
            catch( ... )\
            {\
                MT_LOG_ERROR_MSG( "Unknown exception in " #Hook " hook" );\
            }\
            return boost::function_types::result_type< result parameters >::type();\
        }\
    private: \
        virtual void Apply( core::Facade& facade ) \
        { \
            facade.RegisterHook( &current_, &Previous, SafeImplement, #Hook, #result #parameters ); \
        } \
    } Hook##_; \
    result Hook ## Wrapper::Implement parameters

// $$$$ MCO : or Hook.current_ if we want to keep the macro
#define GET_HOOK( Hook ) Hook##_

// $$$$ MCO : simply Previous should be possible, Previous has to be static in the wrapper though
#define GET_PREVIOUS_HOOK( Hook ) Hook##_.Previous

#endif // SWORD_HOOK_TOOLS_H
