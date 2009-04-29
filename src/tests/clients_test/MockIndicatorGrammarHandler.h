// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockIndicatorGrammarHandler_h_
#define __MockIndicatorGrammarHandler_h_

#include "indicators/GrammarHandler_ABC.h"

// =============================================================================
/** @class  MockIndicatorGrammarHandler
    @brief  MockIndicatorGrammarHandler
*/
// Created: SBO 2009-03-16
// =============================================================================
class MockIndicatorGrammarHandler : public indicators::GrammarHandler_ABC
                                  , public mockpp::ChainableMockObject
{

public:
    //! @name Constructors/Destructor
    //@{
             MockIndicatorGrammarHandler()
                : mockpp::ChainableMockObject( "MockIndicatorGrammarHandler", 0 )
                , HandleNumber_mocker( "HandleNumber", this )
                , HandleString_mocker( "HandleString", this )
                , HandleVariable_mocker( "HandleVariable", this )
                , HandleFunctionCall_mocker( "HandleFunctionCall", this )
             {}
    virtual ~MockIndicatorGrammarHandler() {}
    //@}

    //! @name Operations
    //@{
    virtual void HandleNumber( double value )
    {
        HandleNumber_mocker.forward( value );
    }

    virtual void HandleString( const std::string& value )
    {
        HandleString_mocker.forward( value );
    }

    virtual void HandleVariable( const std::string& name )
    {
        HandleVariable_mocker.forward( name );
    }

    virtual void HandleFunctionCall( const std::string& name, unsigned int parameters )
    {
        HandleFunctionCall_mocker.forward( name, parameters );
    }
    //@}

    //! @name Member data
    //@{
    mockpp::ChainableMockMethod< void, double > HandleNumber_mocker;
    mockpp::ChainableMockMethod< void, std::string > HandleString_mocker;
    mockpp::ChainableMockMethod< void, std::string > HandleVariable_mocker;
    mockpp::ChainableMockMethod< void, std::string, unsigned int > HandleFunctionCall_mocker;
    //@}
};

#endif // __MockIndicatorGrammarHandler_h_
