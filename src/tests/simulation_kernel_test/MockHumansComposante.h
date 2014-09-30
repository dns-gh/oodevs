// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockHumansComposante_ABC_h_
#define __MockHumansComposante_ABC_h_

#include "simulation_kernel/Entities/Agents/Units/Humans/HumansComposante_ABC.h"

// =============================================================================
// @class  MockHuman
// Created: MGD 2010-03-17
// =============================================================================
MOCK_BASE_CLASS( MockHumansComposante, HumansComposante_ABC )
{
    MOCK_METHOD( ChangeHumanRank, 3 );
    MOCK_METHOD( ChangeHumanState, 2 );
    MOCK_METHOD( UpdateHumanState, 2 );
    MOCK_METHOD( RemoveHealthyHumans, 2 );
    MOCK_METHOD( CancelLogisticRequests, 0 );
    MOCK_METHOD( ApplyContamination, 0 );
    MOCK_METHOD( ApplyEffect, 1 );
    MOCK_METHOD( ApplyWounds, 2 );
    MOCK_METHOD( WoundHumans, 3 );
    MOCK_METHOD( HealHumans, 2 );
    MOCK_METHOD( HealAllHumans, 1 );
    MOCK_METHOD( KillAllHumans, 0 );
    MOCK_METHOD( OverloadHumans, 5 );
    MOCK_METHOD( GetOperationalState, 0 );
    MOCK_METHOD( IsEmpty, 0 );
    MOCK_METHOD( NotifyComposanteHandledByMaintenance, 0 );
    MOCK_METHOD( NotifyComposanteBackFromMaintenance, 0 );
    MOCK_METHOD( NotifyComposanteTransfered, 2 );
    MOCK_METHOD( NotifyHumanAdded, 1 );
    MOCK_METHOD( NotifyHumanRemoved, 1 );
    MOCK_METHOD( NotifyHumanChanged, 2 );
    MOCK_METHOD( HasWoundedHumansToEvacuate, 0 );
    MOCK_METHOD( EvacuateWoundedHumans, 1 );
    MOCK_METHOD( GetComposante, 0 );
    MOCK_METHOD( GetNbrHumans, 0 );
    MOCK_METHOD( GetNbrLivingHumans, 0 );
    MOCK_METHOD( GetNbrHealthyHumans, 1 );
};

#endif // __MockHumansComposante_ABC_h_
