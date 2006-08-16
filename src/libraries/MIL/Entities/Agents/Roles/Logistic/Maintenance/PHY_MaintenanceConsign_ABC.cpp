// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceConsign_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 17:55 $
// $Revision: 7 $
// $Workfile: PHY_MaintenanceConsign_ABC.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_MaintenanceConsign_ABC.h"

#include "PHY_MaintenanceComposanteState.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Roles/Logistic/Maintenance/PHY_RolePionLOG_Maintenance.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Network/NET_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceConsign_ABC::PHY_MaintenanceConsign_ABC( PHY_RolePionLOG_Maintenance& maintenance, PHY_MaintenanceComposanteState& composanteState )
    : pMaintenance_    ( &maintenance )
    , pComposanteState_( &composanteState )
    , nTimer_          ( 0 )
    , bHasChanged_     ( true )
{
    pComposanteState_->SetConsign( this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MaintenanceConsign_ABC::PHY_MaintenanceConsign_ABC()
    : pMaintenance_    ( 0 )
    , pComposanteState_( 0 )
    , bHasChanged_     ( true )
    , nTimer_          ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceConsign_ABC::~PHY_MaintenanceConsign_ABC()
{
    if( pComposanteState_ )
        pComposanteState_->SetConsign( 0 );
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetComposanteType
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
const PHY_ComposanteTypePion& PHY_MaintenanceConsign_ABC::GetComposanteType() const
{
    assert( pComposanteState_ );
    return pComposanteState_->GetComposante().GetType();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::GetComposanteBreakdown
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
const PHY_Breakdown& PHY_MaintenanceConsign_ABC::GetComposanteBreakdown() const
{
    assert( pComposanteState_ );
    return pComposanteState_->GetComposanteBreakdown();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::Cancel()
{
    assert( pComposanteState_ );
    

    SetState( eFinished );
    nTimer_ = 0;
    pComposanteState_ = 0;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::EnterStateFinished
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::EnterStateFinished()
{
    SetState( eFinished );
    nTimer_ = 0;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::SendFullState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::SendFullState( NET_ASN_MsgLogMaintenanceTraitementEquipementUpdate& asn ) const
{
    assert( pComposanteState_ );
    assert( pMaintenance_ );
    
    asn.GetAsnMsg().oid_pion_log_traitant = pMaintenance_->GetPion().GetID();
    asn.GetAsnMsg().m.etatPresent         = 1;
    asn.GetAsnMsg().etat                  = (ASN1T_EnumLogMaintenanceTraitementEtat)nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceConsign_ABC::SendChangedState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceConsign_ABC::SendChangedState( NET_ASN_MsgLogMaintenanceTraitementEquipementUpdate& asn ) const
{
    assert( pComposanteState_ );
    if( bHasChanged_ )
        SendFullState( asn );
}
