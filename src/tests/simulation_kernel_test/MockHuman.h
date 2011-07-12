// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockHuman_h_
#define __MockHuman_h_

#include "simulation_kernel/Entities/Units/Humans/Human_ABC.h"

// =============================================================================
// @class  MockHuman
// Created: MGD 2010-03-17
// =============================================================================
MOCK_BASE_CLASS( MockHuman, Human_ABC )
{
    MOCK_METHOD( Heal, 0 );
    MOCK_METHOD( SetRank, 1 );
    MOCK_METHOD( SetWound, 1 );
    MOCK_METHOD( ApplyWound, 1 );
    MOCK_METHOD( ApplyPoisonous, 1 );
    MOCK_METHOD( ApplyContamination, 1 );
    MOCK_METHOD( ApplyMentalDisease, 0 );
    MOCK_METHOD( CancelLogisticRequest, 0 );
    MOCK_METHOD( GetRank, 0 );
    MOCK_METHOD( GetWound, 0 );
    MOCK_METHOD( GetLocation, 0 );
    MOCK_METHOD( IsUsable, 0 );
    MOCK_METHOD( IsDead, 0 );
    MOCK_METHOD( IsWounded, 0 );
    MOCK_METHOD( IsContaminated, 0 );
    MOCK_METHOD( IsMentalDiseased, 0 );
    MOCK_METHOD( IsAnEmergency, 0 );
    MOCK_METHOD( Update, 0 );
    MOCK_METHOD( NeedEvacuation, 0 );
    MOCK_METHOD( Evacuate, 1 );
    MOCK_METHOD( NeedMedical, 0 );
    MOCK_METHOD( SetMedicalState, 1 );
    MOCK_METHOD( NotifyHandledByMedical, 0 );
    MOCK_METHOD( NotifyBackToWar, 0 );
    MOCK_METHOD( HealMentalDisease, 0 );
    MOCK_METHOD( IsAnEmergency, 0 );
    MOCK_METHOD( HealWound, 0 );
    MOCK_METHOD( HealContamination, 0 );
    MOCK_METHOD( NotifyComposanteHandledByMaintenance, 0 );
    MOCK_METHOD( NotifyComposanteBackFromMaintenance, 0 );
};
//@}

#endif // __MockHuman_h_
