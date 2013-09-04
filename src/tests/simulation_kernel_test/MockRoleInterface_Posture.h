// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockRoleImplantable_h_
#define __MockRoleImplantable_h_

#include "simulation_kernel/Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"

// =============================================================================
/** @class  MockRoleImplantable
    @brief  MockRoleImplantable
*/
// Created: MGD 2009-03-04
// =============================================================================
MOCK_BASE_CLASS( MockRoleInterface_Posture, PHY_RoleInterface_Posture )
{
    MOCK_METHOD( GetLastPosture, 0 );
    MOCK_METHOD( GetCurrentPosture, 0 );
    MOCK_METHOD( GetPostureCompletionPercentage, 0 );

    MOCK_METHOD( SetPosturePostePrepareGenie, 0 );
    MOCK_METHOD( UnsetPosturePostePrepareGenie, 0 );
    MOCK_METHOD( SetTimingFactor, 1 );
    MOCK_METHOD( SetPosture, 1 );

    MOCK_METHOD( Update, 1 );
    MOCK_METHOD( Clean, 0 );
    MOCK_METHOD( HasChanged, 0 );

    MOCK_METHOD( EnableDiscreteMode, 0 );
    MOCK_METHOD( DisableDiscreteMode, 0 );

    MOCK_METHOD( IsInstalled, 0 );
    MOCK_METHOD( IsUninstalled, 0 );
    MOCK_METHOD( Install, 0 );

    MOCK_METHOD( SetStealthFactor, 1 );

    MOCK_METHOD( SetElongationFactor, 1 );
    MOCK_METHOD( GetElongationFactor, 0 );

    MOCK_METHOD( IsStealth, 0 );
    MOCK_METHOD( IsMovingPosture, 0 );
    MOCK_METHOD( SetAmbianceSafety, 1 );
};

#endif // __MockRoleImplantable_h_
