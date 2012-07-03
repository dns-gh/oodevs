// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockRolePerceiver_h_
#define __MockRolePerceiver_h_

#include "simulation_kernel/Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"

MOCK_BASE_CLASS( MockRolePerceiver, PHY_RolePion_Perceiver )
{
    MockRolePerceiver( MIL_Agent_ABC& pion ) : PHY_RolePion_Perceiver( pion ) {}
    MOCK_METHOD( NotifyExternalPerception, 2 );
};

#endif // __MockRolePerceiver_h_
