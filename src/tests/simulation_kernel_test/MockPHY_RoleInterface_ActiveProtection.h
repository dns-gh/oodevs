// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_RoleInterface_ActiveProtection_h_
#define __MockPHY_RoleInterface_ActiveProtection_h_

#include "Entities/Agents/Roles/Protection/PHY_RoleInterface_ActiveProtection.h"

// =============================================================================
/** @class  MockPHY_RoleInterface_ActiveProtection
    @brief  MockPHY_RoleInterface_ActiveProtection
*/
// Created: LDC 2010-01-07
// =============================================================================
MOCK_BASE_CLASS( MockPHY_RoleInterface_ActiveProtection, PHY_RoleInterface_ActiveProtection )
{
    MOCK_METHOD( UseAmmunition, 1 );
    MOCK_METHOD( GetPHModifier, 1 );
    MOCK_METHOD( CounterIndirectFire, 1 );
    MOCK_METHOD( DestroyIndirectFire, 1 );
};

#endif // __MockPHY_RoleInterface_ActiveProtection_h_
