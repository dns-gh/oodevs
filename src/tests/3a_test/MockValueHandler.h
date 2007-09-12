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
    , Handle_mocker( "Handle", this )
    {}
    //@}

    //! @name Operations
    //@{
    virtual void Handle( const T& value )
    {
        Handle_mocker.forward( value );
    }
    //@}

    //! @name Member data
    //@{
    mockpp::ChainableMockMethod< void, T > Handle_mocker;
    //@}
};

#endif // __MockValueHandler_h_
