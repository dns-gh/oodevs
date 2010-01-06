// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockMIL_Time_ABC_h_
#define __MockMIL_Time_ABC_h_

#include "MIL_Time_ABC.h"
#include "MIL_Singletons.h"

// =============================================================================
/** @class  MockMIL_Time_ABC
    @brief  MockMIL_Time_ABC
*/
// Created: LDC 2009-12-31
// =============================================================================
class MockMIL_Time_ABC : public mockpp::ChainableMockObject
    , public MIL_Time_ABC
{
public:
    MockMIL_Time_ABC()
    : mockpp::ChainableMockObject( MOCKPP_PCHAR( "MockMIL_Time_ABC" ) )
	, MIL_Time_ABC()
    , GetCurrentTick_mocker( "GetCurrentTick", this )
    , GetTickDuration_mocker( "GetTickDuration", this )
    {
        MIL_Singletons::RegisterTime( *this );
    }
    virtual ~MockMIL_Time_ABC()
    {
        MIL_Singletons::UnregisterTime( *this );
    }

    virtual unsigned int GetCurrentTick() const
    {
        return GetCurrentTick_mocker.forward();
    }
    virtual unsigned int GetTickDuration() const
    {
        return GetTickDuration_mocker.forward();
    }

    mockpp::ChainableMockMethod< unsigned int > GetCurrentTick_mocker;
    mockpp::ChainableMockMethod< unsigned int > GetTickDuration_mocker;

};

#endif // __MockMIL_Time_ABC_h_
