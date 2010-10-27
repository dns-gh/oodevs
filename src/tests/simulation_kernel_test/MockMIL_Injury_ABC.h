// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __Mock_MIL_Injury_ABC_h_
#define __Mock_MIL_Injury_ABC_h_

#include "Entities/Agents/Units/Humans/MIL_Injury_ABC.h"
#include <turtle/mock.hpp>

// =============================================================================
/** @class  Mock_MIL_Injury_ABC
    @brief  Mock_MIL_Injury_ABC
*/
// Created: LDC 2010-07-01
// =============================================================================
MOCK_BASE_CLASS( Mock_MIL_Injury_ABC, MIL_Injury_ABC )
{
public:
    Mock_MIL_Injury_ABC() {}
    virtual ~Mock_MIL_Injury_ABC() {}

    MOCK_METHOD( GetInjuryID, 0 );
    MOCK_METHOD( GetInjuryCategory, 0 );
    MOCK_METHOD( GetLifeExpectancy, 0 );
    MOCK_METHOD( GetAgentDose, 0 );

    MOCK_METHOD( SetLifeExpectancy, 0 );
    MOCK_METHOD( IsInjured, 1 );
    MOCK_METHOD( SetInjury, 2 );
    MOCK_METHOD( Injure, 1 );
    MOCK_METHOD( SetInjuryCategory, 1 );
    MOCK_METHOD( CanInjuryBeDeadly, 0 );

    MOCK_METHOD( UpdateLifeExpectancy, 1 );
    MOCK_METHOD( UpdateInjuryCategory, 0 );
};

#endif // __Mock_MIL_Injury_ABC_h_
