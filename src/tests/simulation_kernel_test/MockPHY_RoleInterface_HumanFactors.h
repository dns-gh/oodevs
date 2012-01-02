// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_RoleInterface_HumanFactors_h_
#define __MockPHY_RoleInterface_HumanFactors_h_

#include "Entities/Agents/Roles/HumanFactors/PHY_RoleInterface_HumanFactors.h"

// =============================================================================
/** @class  MockPHY_RoleInterface_HumanFactors
    @brief  MockPHY_RoleInterface_HumanFactors
*/
// Created: SLG 2010-01-20
// =============================================================================
MOCK_BASE_CLASS( MockPHY_RoleInterface_HumanFactors, PHY_RoleInterface_HumanFactors )
{
    MOCK_METHOD( Update, 1 );
    MOCK_METHOD( Clean, 0 );
    MOCK_METHOD( HasChanged, 0 );

    MOCK_METHOD( ReadOverloading, 1 );
    MOCK_METHOD( SetMorale, 1 );
    MOCK_METHOD( SetExperience, 1 );
    MOCK_METHOD( SetTiredness, 2 );

    MOCK_METHOD( GetMorale, 0 );
    MOCK_METHOD( GetTiredness, 0 );

    MOCK_METHOD( ModifyPH, 1 );

    MOCK_METHOD( SendChangedState, 1 );
    MOCK_METHOD( SendFullState, 1 );
};

#endif // __MockPHY_RoleInterface_HumanFactors_h_
