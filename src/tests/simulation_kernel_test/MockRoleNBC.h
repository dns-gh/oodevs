// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockRoleNBC_h_
#define __MockRoleNBC_h_

#include "simulation_kernel/Entities/Agents/Roles/NBC/PHY_RoleInterface_NBC.h"

// =============================================================================
/** @class  MockRoleNBC
    @brief  MockRoleNBC
*/
// Created: JCR 2008-09-01
// =============================================================================
MOCK_BASE_CLASS( MockRoleNBC, nbc::PHY_RoleInterface_NBC )
{
    MOCK_METHOD( IsContaminated, 0 );
    MOCK_METHOD( Poison, 1 );
    MOCK_METHOD( Contaminate, 1 );
    MOCK_METHOD_EXT( Decontaminate, 0, void(), DecontaminateComplete );
    MOCK_METHOD_EXT( Decontaminate, 1, void( double ), DecontaminatePartial );
    MOCK_METHOD( Update, 1 );
    MOCK_METHOD( Clean, 0 );
    MOCK_METHOD( HasChanged, 0 );
    MOCK_METHOD( WearNbcProtectionSuit, 0 );
    MOCK_METHOD( RemoveNbcProtectionSuit, 0 );
    MOCK_METHOD( GetContaminationQuantity, 0 );
    MOCK_METHOD( IsImmune, 0 );
    MOCK_METHOD( ImmunizeAgent, 0 );
    MOCK_METHOD( StopImmunizeAgent, 0 );
};

#endif // __MockRoleNBC_h_
