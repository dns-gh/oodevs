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

#include "preparation/IndicatorGrammarHandler_ABC.h"

// =============================================================================
/** @class  MockIndicatorGrammarHandler
    @brief  MockIndicatorGrammarHandler
*/
// Created: SBO 2009-03-16
// =============================================================================
class MockIndicatorGrammarHandler : public IndicatorGrammarHandler_ABC
                                  , public mockpp::ChainableMockObject
{

public:
    //! @name Constructors/Destructor
    //@{
             MockIndicatorGrammarHandler()
                : mockpp::ChainableMockObject( "MockIndicatorGrammarHandler", 0 )
                , HandleNumber_mocker( "HandleNumber", this )
                , HandleExtract_mocker( "HandleExtract", this )
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

    virtual void HandleVariable( const std::string& name )
    {
        HandleVariable_mocker.forward( name );
    }

    virtual void HandleFunctionCall( const std::string& name, unsigned int parameters )
    {
        HandleFunctionCall_mocker.forward( name, parameters );
    }

#define GENERATE_HANDLE_MOCKER( name ) \
    virtual void Handle##name( const boost::spirit::classic::tree_match< const char* >::const_tree_iterator& it ) \
    { \
        Handle##name##_mocker.forward( it ); \
    }

    GENERATE_HANDLE_MOCKER( Extract );
    //@}

    //! @name Member data
    //@{
    mockpp::ChainableMockMethod< void, double > HandleNumber_mocker;
    mockpp::ChainableMockMethod< void, const boost::spirit::classic::tree_match< const char* >::const_tree_iterator > HandleExtract_mocker;
    mockpp::ChainableMockMethod< void, std::string > HandleVariable_mocker;
    mockpp::ChainableMockMethod< void, std::string, unsigned int > HandleFunctionCall_mocker;
    //@}
};

#endif // __MockIndicatorGrammarHandler_h_
