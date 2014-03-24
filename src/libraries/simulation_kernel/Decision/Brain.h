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
#include "DEC_Logger.h"
#include "DEC_Decision_ABC.h"
#include <directia/tools/binders/ScriptRef.h>
#define private public
#include <directia/brain/Brain.h>
#undef private
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <boost/bind.hpp>
#include <boost/preprocessor/iteration/iterate.hpp>
#include <boost/preprocessor/repetition/enum_params.hpp>
#include <boost/preprocessor/repetition/enum_binary_params.hpp>
#include <boost/utility/result_of.hpp>
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
     Brain( const std::string& config, DEC_Logger* logger );
     Brain( Brain& parent, DEC_Logger* logger );
    ~Brain() {}
    //@}

    //! @name Operations
    //@{
    void SelectActions();
    void TriggerSelectedActions( double elapsedTime = 1 ) const;
    void ExecuteScript( const std::string& script );

    directia::tools::binders::ScriptRef GetScriptRef( const std::string& );
    directia::tools::binders::ScriptRef GetScriptRef();

#define BRAIN_FUNCTION_MAX_ARITY 7
#define BOOST_PP_FILENAME_1 "Decision/BrainIterate.h"
#define BOOST_PP_ITERATION_LIMITS (0, BRAIN_FUNCTION_MAX_ARITY)
#include BOOST_PP_ITERATE()
#undef BOOST_PP_FILENAME_1
#undef BOOST_PP_ITERATION_LIMITS
#undef BRAIN_FUNCTION_MAX_ARITY

    template< typename Signature >
    void RegisterFunction( const char* const name, const boost::function< Signature >& function )
    {
        (*brain_)[ name ] = ProfilerProxy< Signature >( logger_, name, profilers_[ name ], function, brain_.get() );
    }

    template< typename Function >
    void RegisterFunction( const char* const name, const Function& function )
    {
        RegisterFunction( name, boost::function< boost::result_of< Function() >::type() >( function ) );
    }

    static void ResetProfiling( bool log );

    static double GetTotalTime();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< std::string, MT_Profiler > T_Profilers;
    //@}

private:
    //! @name Member data
    //@{
    DEC_Logger* logger_;
    std::auto_ptr< directia::brain::Brain > brain_;
    static T_Profilers profilers_;
    //@}
};

}

#endif // SWORD_BRAIN_H
