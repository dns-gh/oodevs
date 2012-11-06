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

#include "MT_Tools/MT_Profiler.h"
#include <directia/tools/binders/ScriptRef.h>
#include <directia/brain/Brain.h>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <string>
#include <memory>
#include <map>

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

    template< typename Method >
    void RegisterMethod( const char* const name, const Method& method )
    {
        brain_->Register( name, method );
    }
    template< typename Signature >
    void RegisterFunction( const char* const name, const boost::function< Signature >& function )
    {
        (*brain_)[ name ] = ProfilerProxy< Signature >( name, function );
    }
    template< typename Function >
    void RegisterFunction( const char* const name, const Function& function )
    {
        RegisterFunction( name, boost::function< Function::result_type() >( function ) );
    }

#define SWORD_BRAIN_FREE_FUNCTION(z, n, d) \
    template< typename R BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T) > \
    void RegisterFunction( const char* const name, R(*function)( BOOST_PP_ENUM_PARAMS(n, T) ) ) \
    { \
        RegisterFunction( name, boost::function< R( BOOST_PP_ENUM_PARAMS(n, T) ) >( function ) ); \
    }
    BOOST_PP_REPEAT(6, SWORD_BRAIN_FREE_FUNCTION, BOOST_PP_EMPTY)
#undef SWORD_BRAIN_FREE_FUNCTION

    static void ResetProfiling( bool log );

    static double GetTotalTime();
    //@}

private:
    //! @name Types
    //@{
    struct ProfilerGuard : boost::noncopyable
    {
        explicit ProfilerGuard( const std::string& name )
            : profiler_( profilers_[ name ] )
        {
            profiler_.Start();
        }
        ~ProfilerGuard()
        {
            profiler_.Stop();
        }
        MT_Profiler& profiler_;
    };

    template< typename Signature >
    struct ProfilerProxy;

#define SWORD_BRAIN_PROFILER_PROXY(z, n, d ) \
    template< typename R BOOST_PP_COMMA_IF(n) BOOST_PP_ENUM_PARAMS(n, typename T) > \
    struct ProfilerProxy< R( BOOST_PP_ENUM_PARAMS(n,T) ) > \
    { \
        typedef typename boost::function< R( BOOST_PP_ENUM_PARAMS(n,T) ) > function_type; \
        typedef typename function_type::result_type result_type; \
        ProfilerProxy( const std::string& name, const function_type& f ) \
            : name_( name ) \
            , f_   ( f ) \
        {} \
        R operator()( BOOST_PP_ENUM_BINARY_PARAMS(n, T, t) ) const \
        { \
            ProfilerGuard guard( name_ ); \
            return f_( BOOST_PP_ENUM_PARAMS(n, t) ); \
        } \
        std::string name_; \
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
    static bool profiling_;
    //@}
};

}

#endif // SWORD_BRAIN_H
