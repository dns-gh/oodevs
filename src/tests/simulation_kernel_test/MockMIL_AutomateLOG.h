// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockMIL_AutomateLOG_h__
#define __MockMIL_AutomateLOG_h__

#include "simulation_kernel/Entities/Specialisations/LOG/MIL_AutomateLOG.h"

// =============================================================================
/** @class  MockMIL_AutomateLOG
    @brief  MockMIL_AutomateLOG
*/
// Created: NLD 2011-01-19
// =============================================================================
MOCK_BASE_CLASS( MockMIL_AutomateLOG, MIL_AutomateLOG )
{
    MockMIL_AutomateLOG( MIL_Formation& formation, const PHY_LogisticLevel& level ) : MIL_AutomateLOG( formation, level ) {}
    MockMIL_AutomateLOG( MIL_Automate&  automat, const PHY_LogisticLevel& level) : MIL_AutomateLOG( automat, level ) {}

    MOCK_METHOD( NotifyQuotaThresholdReached, 1 );
    MOCK_METHOD( Serialize, 1 );
};

#endif // __MockMIL_AutomateLOG_h__