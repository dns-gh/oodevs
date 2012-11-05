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

#include <directia/tools/binders/ScriptRef.h>
#include <directia/brain/Brain.h>
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>
#include <memory>

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
        (*brain_)[ name ] = function;
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
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< directia::brain::Brain > brain_;
    //@}
};

}

#endif // SWORD_BRAIN_H
