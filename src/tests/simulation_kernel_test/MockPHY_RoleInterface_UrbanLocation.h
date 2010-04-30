// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_RoleInterface_UrbanLocation_h_
#define __MockPHY_RoleInterface_UrbanLocation_h_

#include "Entities/Agents/Roles/Urban/PHY_RoleInterface_UrbanLocation.h"

// =============================================================================
/** @class  MockPHY_RoleInterface_UrbanLocation
    @brief  MockPHY_RoleInterface_UrbanLocation
*/
// Created: LDC 2010-04-30
// =============================================================================
MOCK_BASE_CLASS( MockPHY_RoleInterface_UrbanLocation, PHY_RoleInterface_UrbanLocation )
{
    MOCK_METHOD( ComputeUrbanProtection, 1 );
    MOCK_METHOD( GetFirerPosition, 1 );
    MOCK_METHOD( GetTargetPosition, 1 );
    MOCK_METHOD( ComputeDistanceInsideSameUrbanBlock, 1 );
    MOCK_METHOD( ComputeRatioPionInside, 1 );
    MOCK_METHOD( GetCurrentUrbanBlock, 0 );
};

#endif // __MockPHY_RoleInterface_UrbanLocation_h_
