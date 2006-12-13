// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Maintenance/PHY_MaintenanceComposanteState.cpp $
// $Author: Jvt $
// $Modtime: 4/05/05 10:42 $
// $Revision: 5 $
// $Workfile: PHY_MaintenanceComposanteState.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "PHY_MaintenanceComposanteState.h"
#include "PHY_MaintenanceConsign_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"
#include "Network/NET_ASN_Messages.h"
#include "Tools/MIL_IDManager.h"

BOOST_CLASS_EXPORT_GUID( PHY_MaintenanceComposanteState, "PHY_MaintenanceComposanteState" )

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState::PHY_MaintenanceComposanteState( MIL_AgentPion& pion, PHY_ComposantePion& composante )
    : nID_                ( MIL_IDManager::maintenanceComposanteStates_.GetFreeSimID() )
    , pPion_              ( &pion )
    , pComposante_        ( &composante )
    , pConsign_           ( 0 )
    , vComposantePosition_( pion.GetRole< PHY_RolePion_Location >().GetPosition() )
    , bHasChanged_        ( true )
    , bDiagnosed_         ( false )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState::PHY_MaintenanceComposanteState()
    : nID_                ()
    , pPion_              ( 0 )
    , pComposante_        ( 0 )
    , pConsign_           ( 0 )
    , vComposantePosition_()
    , bHasChanged_        ( true )
    , bDiagnosed_         ( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState::~PHY_MaintenanceComposanteState()
{
    SendMsgDestruction();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::load
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< uint& >( nID_ )
         >> pPion_
         >> pComposante_
         >> pConsign_
         >> vComposantePosition_
         >> bDiagnosed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::save
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << nID_
         << pPion_
         << pComposante_
         << pConsign_
         << vComposantePosition_
         << bDiagnosed_;
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::GetPionPosition
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_MaintenanceComposanteState::GetPionPosition() const
{
    assert( pPion_ );
    return pPion_->GetRole< PHY_RolePion_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::GetComposanteBreakdown
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
const PHY_Breakdown& PHY_MaintenanceComposanteState::GetComposanteBreakdown() const
{
    assert( pComposante_ && pComposante_->GetBreakdown() );
    return *pComposante_->GetBreakdown();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::ApproximateTravelTime
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
uint PHY_MaintenanceComposanteState::ApproximateTravelTime( const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos ) const
{
    assert( pComposante_ );
    return pComposante_->ApproximateTravelTime( vSourcePos, vTargetPos );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::NotifyHandledByMaintenance
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::NotifyHandledByMaintenance()
{   
    assert( pComposante_ );
    pComposante_->NotifyHandledByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::NotifyRepaired
// Created: NLD 2005-01-05
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::NotifyRepaired()
{
    assert( pComposante_ );
    pComposante_->NotifyRepairedByMaintenance();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::Cancel
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::Cancel()
{
    if( pConsign_ )
    {
        pConsign_->Cancel();
        pConsign_ = 0;
    }
}


// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::GetAutomate
// Created: JVT 2005-05-04
// -----------------------------------------------------------------------------
const MIL_Automate& PHY_MaintenanceComposanteState::GetAutomate() const
{
    assert( pPion_ );
    return pPion_->GetAutomate();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SendFullState() const
{
    assert( pPion_ );

    SendMsgCreation();
    NET_ASN_MsgLogMaintenanceTraitementEquipementUpdate asn;
    asn().oid_consigne          = nID_;
    asn().oid_pion              = pPion_->GetID();

    asn().m.diagnostique_effectuePresent = 1;
    asn().m.diagnostique_effectuePresent = bDiagnosed_;

    if( pConsign_ )
        pConsign_->SendFullState( asn );
    else
    {
        asn().oid_pion_log_traitant = 0;
        asn().m.etatPresent         = 1;
        asn().etat                  = EnumLogMaintenanceTraitementEtat::termine;        
    }
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SendChangedState() const
{
    if( !( bHasChanged_ || ( pConsign_ && pConsign_->HasChanged() ) ) )
        return;

    assert( pPion_ );

    NET_ASN_MsgLogMaintenanceTraitementEquipementUpdate asn;
    asn().oid_consigne          = nID_;
    asn().oid_pion              = pPion_->GetID();

    asn().m.diagnostique_effectuePresent = 1;
    asn().m.diagnostique_effectuePresent = bDiagnosed_;

    if( pConsign_ )
        pConsign_->SendChangedState( asn );
    else
    {
        asn().oid_pion_log_traitant = 0;
        asn().m.etatPresent         = 1;
        asn().etat                  = EnumLogMaintenanceTraitementEtat::termine;
    }
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::Clean
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::Clean()
{
    bHasChanged_ = false;
    if( pConsign_ )
        pConsign_->Clean();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SendMsgCreation() const
{
    assert( pPion_ );
    assert( pComposante_ );

    NET_ASN_MsgLogMaintenanceTraitementEquipementCreation asn;
    asn().oid_consigne    = nID_;
    asn().oid_pion        = pPion_->GetID();
    asn().type_equipement = pComposante_->GetType().GetMosID();
    asn().type_panne      = GetComposanteBreakdown().GetID();
    asn.Send();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SendMsgDestruction() const
{
    assert( pPion_ );

    NET_ASN_MsgLogMaintenanceTraitementEquipementDestruction asn;
    asn().oid_consigne    = nID_;
    asn().oid_pion        = pPion_->GetID();
    asn.Send();
}
