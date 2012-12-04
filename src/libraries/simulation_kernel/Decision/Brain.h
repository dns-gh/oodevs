// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef SWORD_BRAIN_H
#define SWORD_BRAIN_H

#include "MT_Tools/MT_ProfilerGuard.h"
#include <directia/tools/binders/ScriptRef.h>
#define private public
#include <directia/brain/Brain.h>
#undef private
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <string>
#include <memory>
#include <map>

namespace directia
{
namespace tools
{
namespace binders
{
    template<> inline static void RegisterType< void >( VirtualMachine_ABC& )
    {
        // NOTHING
    }
}
}
}

namespace sword
{
// =============================================================================
/** @class  Brain
    @brief  Brain
*/
// Created: MCO 2012-10-31
// =============================================================================
class Brain : boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Brain( const std::string& config );
    explicit Brain( Brain& parent );
    //@}

    //! @name Operations
    //@{
    void SelectActions();
    void TriggerSelectedActions( double elapsedTime = 1 ) const;

    directia::tools::binders::ScriptRef GetScriptRef( const std::string& );
    directia::tools::binders::ScriptRef GetScriptRef();

#define SWORD_SCRIPT_MEMBER_MAX_ARITY 3
#define SWORD_SCRIPT_MEMBER_REGISTER_PARAM(z, n, d) \
    RegisterType< P##n >( vm );
#define SWORD_SCRIPT_MEMBER_READ_PARAM(z, n, d) \
    BOOST_PP_COMMA_IF(n) Read< P##n >( vm, BOOST_PP_INC(BOOST_PP_INC(n)) )
#define SWORD_SCRIPT_MEMBER_READ(z, n, d) \
    template< typename T, typename R BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename P) > \
    struct Reader##n \
    { \
        template< typename Function > \
        int DoRead( directia::tools::VirtualMachine_ABC& vm, const Function& f ) const \
        { \
            using namespace directia::tools::binders; \
            R result = f( Read< T& >( vm, 1 ) BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT(n, SWORD_SCRIPT_MEMBER_READ_PARAM, BOOST_PP_EMPTY) ); \
            Write< R >( vm, result ); \
            return 1; \
        } \
    }; \
    template< typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename P) > \
    struct Reader##n< T, void BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, P) > \
    { \
        template< typename Function > \
        int DoRead( directia::tools::VirtualMachine_ABC& vm, const Function& f ) const \
        { \
            using namespace directia::tools::binders; \
            f( Read< T& >( vm, 1 ) BOOST_PP_COMMA_IF(n) BOOST_PP_REPEAT(n, SWORD_SCRIPT_MEMBER_READ_PARAM, BOOST_PP_EMPTY) ); \
            return 0; \
        } \
    };
    BOOST_PP_REPEAT(SWORD_SCRIPT_MEMBER_MAX_ARITY, SWORD_SCRIPT_MEMBER_READ, BOOST_PP_EMPTY)
 #define SWORD_SCRIPT_MEMBER_FUNCTION(z, n, d) \
    template< typename T, typename R BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename P) > \
    struct ScriptMemberFunction##n \
        : public directia::tools::binders::ScriptFunction_ABC \
        , Reader##n< T, R BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, P) > \
    { \
        typedef boost::function< R( T& BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, P) ) > Function; \
        ScriptMemberFunction##n( directia::tools::VirtualMachine_ABC& vm, const char* const name, Function f ) \
            : f_( f ) \
        { \
            using namespace directia::tools::binders; \
            RegisterType< T& >( vm ); \
            RegisterType< R >( vm ); \
            BOOST_PP_REPEAT(n, SWORD_SCRIPT_MEMBER_REGISTER_PARAM, BOOST_PP_EMPTY) \
            RegisterMember< T& >( vm, name ); \
        } \
        int operator()( directia::tools::VirtualMachine_ABC& vm ) const \
        { \
            using namespace directia::tools::binders; \
            impl::CheckSize( vm, BOOST_PP_INC(n) ); \
            return DoRead( vm, f_ ); \
        } \
        Function f_; \
    };
    BOOST_PP_REPEAT(SWORD_SCRIPT_MEMBER_MAX_ARITY, SWORD_SCRIPT_MEMBER_FUNCTION, BOOST_PP_EMPTY)
#undef SWORD_SCRIPT_MEMBER_FUNCTION
#undef SWORD_SCRIPT_MEMBER_READ_PARAM
#undef SWORD_SCRIPT_MEMBER_REGISTER_PARAM

#define SWORD_BRAIN_MEMBER_FUNCTION(z, n, c) \
    template< typename R, typename T BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename P) > \
    void RegisterMethod( const char* const name, R(T::*method)( BOOST_PP_ENUM_PARAMS(n, P) ) c ) \
    { \
        new ( directia::tools::binders::ScriptFunction_ABC::Alloc( brain_->vm_, \
                sizeof( ScriptMemberFunction##n< T, R BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, P) > ) ) ) \
            ScriptMemberFunction##n< T, R BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, P) >( \
                    brain_->vm_, name, \
                        ProfilerProxy< R( T& BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, P) ) >( profilers_[ name ], \
                            boost::bind( method, BOOST_PP_ENUM_SHIFTED_PARAMS(BOOST_PP_INC(BOOST_PP_INC(n)), _) ) ) ); \
    }
    BOOST_PP_REPEAT(SWORD_SCRIPT_MEMBER_MAX_ARITY, SWORD_BRAIN_MEMBER_FUNCTION,)
    BOOST_PP_REPEAT(SWORD_SCRIPT_MEMBER_MAX_ARITY, SWORD_BRAIN_MEMBER_FUNCTION,const)
#undef SWORD_BRAIN_MEMBER_FUNCTION
#undef SWORD_SCRIPT_MEMBER_MAX_ARITY

    template< typename Signature >
    void RegisterFunction( const char* const name, const boost::function< Signature >& function )
    {
        (*brain_)[ name ] = ProfilerProxy< Signature >( profilers_[ name ], function );
    }
    template< typename Function >
    void RegisterFunction( const char* const name, const Function& function )
    {
        RegisterFunction( name, boost::function< Function::result_type() >( function ) );
    }

#define SWORD_BRAIN_FREE_FUNCTION(z, n, d) \
    template< typename R BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename P) > \
    void RegisterFunction( const char* const name, R(*function)( BOOST_PP_ENUM_PARAMS(n, P) ) ) \
    { \
        RegisterFunction( name, boost::function< R( BOOST_PP_ENUM_PARAMS(n, P) ) >( function ) ); \
    }
    BOOST_PP_REPEAT(6, SWORD_BRAIN_FREE_FUNCTION, BOOST_PP_EMPTY)
#undef SWORD_BRAIN_FREE_FUNCTION

    static void ResetProfiling( bool log );

    static double GetTotalTime();
    //@}

private:
    //! @name Types
    //@{
    template< typename Signature >
    struct ProfilerProxy;

#define SWORD_BRAIN_PROFILER_PROXY(z, n, d ) \
    template< typename R BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename P) > \
    struct ProfilerProxy< R( BOOST_PP_ENUM_PARAMS(n,P) ) > \
    { \
        typedef typename boost::function< R( BOOST_PP_ENUM_PARAMS(n,P) ) > function_type; \
        typedef typename function_type::result_type result_type; \
        ProfilerProxy( MT_Profiler& profiler, const function_type& f ) \
            : profiler_( &profiler ) \
            , f_       ( f ) \
        {} \
        R operator()( BOOST_PP_ENUM_BINARY_PARAMS(n, P, t) ) const \
        { \
            MT_ProfilerGuard guard( *profiler_ ); \
            return f_( BOOST_PP_ENUM_PARAMS(n, t) ); \
        } \
        MT_Profiler* profiler_; \
        function_type f_; \
    }; \

    BOOST_PP_REPEAT(7, SWORD_BRAIN_PROFILER_PROXY, BOOST_PP_EMPTY)
#undef SWORD_BRAIN_PROFILER_PROXY

    typedef std::map< std::string, MT_Profiler > T_Profilers;
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< directia::brain::Brain > brain_;
    static T_Profilers profilers_;
    //@}
};

}

#endif // SWORD_BRAIN_H
