// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalConsign_ABC.cpp $
// $Author: Jvt $
// $Modtime: 11/04/05 15:28 $
// $Revision: 5 $
// $Workfile: PHY_MedicalConsign_ABC.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_MedicalConsign_ABC.h"

#include "PHY_MedicalHumanState.h"
#include "Entities/Agents/Roles/Logistic/Medical/PHY_RolePionLOG_Medical.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Network/NET_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalConsign_ABC::PHY_MedicalConsign_ABC( PHY_RolePionLOG_Medical& medical, PHY_MedicalHumanState& humanState )
    : pMedical_   ( &medical )
    , pHumanState_( &humanState )
    , nTimer_     ( 0 )
    , bHasChanged_( true )
    , nState_     ( eWaitingForEvacuation )
{
    pHumanState_->SetConsign( this );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalConsign_ABC::PHY_MedicalConsign_ABC()
    : pMedical_   ( 0 )
    , pHumanState_( 0 )
    , nTimer_     ( 0 )
    , bHasChanged_( true )
    , nState_     ( eWaitingForEvacuation )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalConsign_ABC::~PHY_MedicalConsign_ABC()
{
    if( pHumanState_ )
        pHumanState_->SetConsign( 0 );
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::IsAnEmergency
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalConsign_ABC::IsAnEmergency() const
{
    assert( pHumanState_ );
    return pHumanState_->IsAnEmergency();
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::Cancel
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::Cancel()
{
    assert( pHumanState_ );
    SetState( eFinished );
    nTimer_ = 0;
    pHumanState_ = 0;
}
 
// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::EnterStateFinished
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::EnterStateFinished()
{
    SetState( eFinished );
    nTimer_ = 0;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::SendFullState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::SendFullState( NET_ASN_MsgLogSanteTraitementHumainUpdate& asn ) const
{
    assert( pHumanState_ );
    assert( pMedical_ );
    
    asn.GetAsnMsg().m.oid_pion_log_traitantPresent = 1;
    asn.GetAsnMsg().m.etatPresent                  = 1;
    asn.GetAsnMsg().oid_pion_log_traitant          = pMedical_->GetPion().GetID();
    asn.GetAsnMsg().etat                           = (ASN1T_EnumLogSanteTraitementEtat)nState_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalConsign_ABC::SendChangedState
// Created: NLD 2005-01-04
// -----------------------------------------------------------------------------
void PHY_MedicalConsign_ABC::SendChangedState( NET_ASN_MsgLogSanteTraitementHumainUpdate& asn ) const
{
    assert( pHumanState_ );
    if( bHasChanged_ )
        SendFullState( asn );
}
