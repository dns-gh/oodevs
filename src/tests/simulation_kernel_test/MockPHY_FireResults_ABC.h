// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_FireResults_ABC_h_
#define __MockPHY_FireResults_ABC_h_

#include "Entities/Actions/PHY_FireResults_ABC.h"

// =============================================================================
/** @class  MockPHY_FireResults_ABC
    @brief  MockPHY_FireResults_ABC
*/
// Created: SLG 2010-01-19
// =============================================================================
class MockPHY_FireResults_ABC
    : public mockpp::ChainableMockObject
    , public PHY_FireResults_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    MockPHY_FireResults_ABC()
        : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockPHY_FireResults_ABC" ) )
        , MOCKPP_CONSTRUCT_CHAINABLE_MEMBERS( Hit ) {}
    virtual ~MockPHY_FireResults_ABC() {}
    //@}

    MOCKPP_VOID_CHAINABLE0( MockPHY_FireResults_ABC, Hit );

};

#endif // __MockPHY_FireResults_ABC_h_
