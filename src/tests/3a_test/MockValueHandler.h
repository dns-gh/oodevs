// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MockValueHandler_h_
#define __MockValueHandler_h_

#include "3a/ValueHandler_ABC.h"

// =============================================================================
/** @class  MockValueHandler
    @brief  MockValueHandler
*/
// Created: AGE 2007-08-31
// =============================================================================
template< typename T >
class MockValueHandler : public ValueHandler_ABC< T >, public mockpp::ChainableMockObject
{

public:
    //! @name Constructors/Destructor
    //@{
    MockValueHandler()
    : mockpp::ChainableMockObject( "MockValueHandler", 0 )
    , BeginTick_mocker( "BeginTick", this )
    , Handle_mocker   ( "Handle", this )
    , EndTick_mocker  ( "EndTick", this )
    {}
    //@}

    //! @name Operations
    //@{
    virtual void BeginTick()
    {
        BeginTick_mocker.forward();
    }
    virtual void Handle( const T& value )
    {
        Handle_mocker.forward( value );
    }
    virtual void EndTick()
    {
        EndTick_mocker.forward();
    }
    //@}

    //! @name Member data
    //@{
    mockpp::ChainableMockMethod< void    > BeginTick_mocker;
    mockpp::ChainableMockMethod< void, T > Handle_mocker;
    mockpp::ChainableMockMethod< void    > EndTick_mocker;
    //@}
};

#endif // __MockValueHandler_h_
