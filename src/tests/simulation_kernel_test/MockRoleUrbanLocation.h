// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __MockRoleUrbanLocation_h_
#define __MockRoleUrbanLocation_h_

#include "simulation_kernel/Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"

MOCK_BASE_CLASS( MockRoleUrbanLocation, PHY_RolePion_UrbanLocation )
{
    MockRoleUrbanLocation( MIL_Agent_ABC& pion ) : PHY_RolePion_UrbanLocation( pion ) {}
    MOCK_METHOD( IsInCity, 0 );
};

#endif // __MockRolePerceiver_h_
