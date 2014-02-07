// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MockPHY_RoleInterface_Composantes_h_
#define __MockPHY_RoleInterface_Composantes_h_

#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Knowledge/DEC_Knowledge_AgentComposante.h"

// =============================================================================
/** @class  MockPHY_RoleInterface_Composantes
    @brief  MockPHY_RoleInterface_Composantes
*/
// Created: LDC 2010-04-30
// =============================================================================

MOCK_BASE_CLASS( MockPHY_RoleInterface_Composantes, PHY_RoleInterface_Composantes )
{
    MOCK_METHOD( Update, 1 );
    MOCK_METHOD( Clean, 0 );
    MOCK_METHOD( ChangeComposantesAvailability, 2 );
    MOCK_METHOD( RepairAllComposantes, 1 );
    MOCK_METHOD( DestroyRandomComposante, 0 );
    MOCK_METHOD( DestroyAllComposantes, 0 );
    MOCK_METHOD( LendComposante, 2 );
    MOCK_METHOD( RetrieveLentComposante, 2 );
    MOCK_METHOD( NotifyLentComposanteReceived, 2 );
    MOCK_METHOD( NotifyLentComposanteReturned, 2 );
    MOCK_METHOD( ReturnAllBorrowedComposantes, 0 );
    MOCK_METHOD( RetrieveAllLentComposantes, 0 );
    MOCK_METHOD( PreprocessRandomBreakdowns, 1 );
    MOCK_METHOD( NotifyComposanteWaitingForMaintenance, 1 );
    MOCK_METHOD( NotifyComposanteBackFromMaintenance, 1 );
    MOCK_METHOD( FindRequest, 1 );
    MOCK_METHOD( GetSignificantVolume, 1 );
    MOCK_METHOD( GetVisibleVolumes, 0 );
    MOCK_METHOD( BuildKnowledgeComposantes, 1 );
    MOCK_METHOD( GetMajorComposante, 0 );
    MOCK_METHOD( GetOperationalState, 0 );
    MOCK_METHOD( GetMajorOperationalState, 0 );
    MOCK_METHOD( GetMaxWeight, 1 );
    MOCK_METHOD( CanTransportDestroyed, 0 );
    MOCK_METHOD( IsNeutralized, 0 );
    MOCK_METHOD( GetComposantesAbleToBeFired, 3, void( PHY_Composante_ABC::T_ComposanteVector&, unsigned int, bool ), GetComposantesAbleToBeFired3 );
    MOCK_METHOD( GetComposantesAbleToBeFired, 2, void( PHY_Composante_ABC::T_ComposanteVector&, bool ), GetComposantesAbleToBeFired2 );
    MOCK_METHOD( GetConvoyTransportersTotal, 0 );
    MOCK_METHOD( Neutralize, 0 );
    MOCK_METHOD( ApplyPopulationFire, 5 );
    MOCK_METHOD( ApplyDirectFire, 3 );
    MOCK_METHOD( ApplyIndirectFire, 3 );
    MOCK_METHOD( ApplyDirectFireOnMajorComposantes, 2 );
    MOCK_METHOD( ApplyExplosion, 2 );
    MOCK_METHOD( ApplyUrbanObjectCrumbling, 1 );
    MOCK_METHOD( ApplyContamination, 0 );
    MOCK_METHOD( GetDangerosity, 3 );
    MOCK_METHOD( GetOnlyLoadableMaxRangeToFireOn, 2 );
    MOCK_METHOD( GetMaxRangeToFireOn, 3 );
    MOCK_METHOD( GetMinRangeToFireOn, 2 );
    MOCK_METHOD( GetMaxRangeToFireOnActualPosture, 2 );
    MOCK_METHOD( GetMinRangeToFireOnActualPosture, 2 );
    MOCK_METHOD( GetMaxRangeToIndirectFire, 3, double( const PHY_DotationCategory&, bool, bool), GetMaxRangeToIndirectFire1 );
    MOCK_METHOD( GetMaxRangeToIndirectFire, 0, double(), GetMaxRangeToIndirectFire2 );
    MOCK_METHOD( GetMinRangeToIndirectFire, 3 );
    MOCK_METHOD( GetMaxRangeToFire, 2 );
    MOCK_METHOD( NotifyComposanteAdded, 2 );
    MOCK_METHOD( NotifyComposanteRemoved, 1 );
    MOCK_METHOD( NotifyComposanteChanged, 2 );
    MOCK_METHOD( NotifyComposanteRepaired, 0 );
    MOCK_METHOD( HasChanged, 0 );
    MOCK_METHOD( IsUsable, 0 );
    MOCK_METHOD( IsImmobilized, 0 );
    MOCK_METHOD( GetPion, 0 );
    MOCK_METHOD( WoundLoadedHumans, 3 );
};

#endif // __MockPHY_RoleInterface_Composantes_h_
