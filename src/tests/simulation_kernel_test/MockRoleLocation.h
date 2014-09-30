// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockRoleLocation_h_
#define __MockRoleLocation_h_

#include "simulation_kernel/Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"

// =============================================================================
/** @class  MockRoleLocation
    @brief  MockRoleLocation
*/
// Created: JCR 2008-09-01
// =============================================================================
MOCK_BASE_CLASS( MockRoleLocation, PHY_RoleInterface_Location )
{
    MOCK_METHOD( NotifyTerrainObjectCollision, 3 );
    MOCK_METHOD( NotifyTerrainPutInsideObject, 1 );
    MOCK_METHOD( NotifyTerrainPutOutsideObject, 1 );

    MOCK_METHOD( Update, 1 );
    MOCK_METHOD( Clean, 0 );

    MOCK_METHOD( MagicMove, 1 );
    MOCK_METHOD( Move, 3 );

    MOCK_METHOD( GetHeight, 0 );
    MOCK_METHOD( GetAltitude, 0 );
    MOCK_METHOD( GetPosition, 0 );
    MOCK_METHOD( GetSharedPosition, 0 );
    MOCK_METHOD( GetDirection, 0 );
    MOCK_METHOD( GetAgent, 0 );
    MOCK_METHOD( GetCurrentSpeed, 0 );
    MOCK_METHOD( HasDoneMagicMove, 0 );
};

#endif // __MockRoleLocation_h_
