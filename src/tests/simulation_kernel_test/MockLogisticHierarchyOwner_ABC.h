// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __MockLogisticHierarchyOwner_ABC_h_
#define __MockLogisticHierarchyOwner_ABC_h_

#include "simulation_kernel/Entities/Specialisations/LOG/LogisticHierarchyOwner_ABC.h"

// =============================================================================
/** @class  MockLogisticHierarchyOwner_ABC
    @brief  MockLogisticHierarchyOwner_ABC
*/
// Created: NLD 2011-01-19
// =============================================================================
MOCK_BASE_CLASS( MockLogisticHierarchyOwner_ABC, logistic::LogisticHierarchyOwner_ABC )
{
    MOCK_METHOD( NotifyQuotaThresholdReached, 1 );
    MOCK_METHOD( Serialize, 1 );
    MOCK_METHOD( GetID, 0 );
};

#endif // __MockLogisticHierarchyOwner_ABC_h_
