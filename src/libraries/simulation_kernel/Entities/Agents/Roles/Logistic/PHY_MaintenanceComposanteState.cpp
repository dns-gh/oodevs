// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MaintenanceComposanteState.cpp $
// $Author: Jvt $
// $Modtime: 4/05/05 10:42 $
// $Revision: 5 $
// $Workfile: PHY_MaintenanceComposanteState.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MaintenanceComposanteState.h"
#include "ConsignHelper.h"
#include "PHY_MaintenanceConsign_ABC.h"
#include "MIL_Time_ABC.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Units/Logistic/PHY_Breakdown.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MaintenanceComposanteState )

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState::PHY_MaintenanceComposanteState( MIL_Agent_ABC& pion, PHY_ComposantePion& composante )
    : nID_                ( logistic::NewConsignId() )
    , nCreationTick_      ( MIL_Time_ABC::GetTime().GetCurrentTimeStep() )
    , pPion_              ( &pion )
    , pComposante_        ( &composante )
    , pConsign_           ( 0 )
    , vComposantePosition_( pion.GetRole< PHY_RoleInterface_Location >().GetPosition() )
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
    , nCreationTick_      ()
    , pPion_              ( 0 )
    , pComposante_        ( 0 )
    , pConsign_           ( 0 )
    , vComposantePosition_()
    , bHasChanged_        ( true )
    , bDiagnosed_         ( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MaintenanceComposanteState::~PHY_MaintenanceComposanteState()
{
    SendMsgDestruction();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::load
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> const_cast< unsigned int& >( nID_ )
         >> const_cast< unsigned int& >( nCreationTick_ )
         >> pPion_
         >> pComposante_
         >> pConsign_
         >> vComposantePosition_
         >> bDiagnosed_;
    logistic::RegisterConsignId( nID_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::save
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << nID_
         << nCreationTick_
         << pPion_
         << pComposante_
         << pConsign_
         << vComposantePosition_
         << bDiagnosed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::GetPionPosition
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_MaintenanceComposanteState::GetPionPosition() const
{
    assert( pPion_ );
    return pPion_->GetRole< PHY_RoleInterface_Location >().GetPosition();
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
unsigned int PHY_MaintenanceComposanteState::ApproximateTravelTime( const MT_Vector2D& vSourcePos, const MT_Vector2D& vTargetPos ) const
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
        pConsign_->Cancel();
    pConsign_ = 0;
}

unsigned int PHY_MaintenanceComposanteState::GetID() const
{
    return nID_;
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

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SendFullState( unsigned int context ) const
{
    assert( pPion_ );
    SendMsgCreation();
    client::LogMaintenanceHandlingUpdate asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_unit()->set_id( pPion_->GetID() );
    asn().set_diagnosed( bDiagnosed_ );
    if( pConsign_ )
        pConsign_->SendFullState( asn );
    else
    {
        asn().mutable_provider()->set_id( 0 );
        asn().set_state( sword::LogMaintenanceHandlingUpdate::finished );
    }
    asn.Send( NET_Publisher_ABC::Publisher(), context );
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
    client::LogMaintenanceHandlingUpdate asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_unit()->set_id( pPion_->GetID() );
    asn().set_diagnosed( bDiagnosed_ );
    if( pConsign_ )
        pConsign_->SendChangedState( asn );
    else
    {
        asn().mutable_provider()->set_id( 0 );
        asn().set_state( sword::LogMaintenanceHandlingUpdate::finished );
    }
    asn.Send( NET_Publisher_ABC::Publisher() );
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
    client::LogMaintenanceHandlingCreation asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_unit()->set_id( pPion_->GetID() );
    asn().set_tick( nCreationTick_ );
    asn().mutable_equipement()->set_id( pComposante_->GetType().GetMosID().id() );
    asn().mutable_breakdown()->set_id( GetComposanteBreakdown().GetID() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SendMsgDestruction() const
{
    assert( pPion_ );
    client::LogMaintenanceHandlingUpdate update;
    update().mutable_request()->set_id( nID_ );
    update().mutable_unit()->set_id( pPion_->GetID() );
    update().mutable_provider()->set_id( 0 );
    update().set_diagnosed( bDiagnosed_ );
    update().set_state( sword::LogMaintenanceHandlingUpdate::finished );
    update.Send( NET_Publisher_ABC::Publisher() );
    client::LogMaintenanceHandlingDestruction asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_unit()->set_id( pPion_->GetID() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SetConsign
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SetConsign( PHY_MaintenanceConsign_ABC* pConsign )
{
    if( pConsign == pConsign_ )
        return;
    pConsign_ = pConsign;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::GetConsign
// Created: MMC 2012-11-15
// -----------------------------------------------------------------------------
const PHY_MaintenanceConsign_ABC* PHY_MaintenanceComposanteState::GetConsign() const
{
    return pConsign_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SetComposantePosition
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SetComposantePosition( const MT_Vector2D& vPosition )
{
    vComposantePosition_ = vPosition;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::GetComposante
// Created: NLD 2006-03-29
// -----------------------------------------------------------------------------
const PHY_ComposantePion& PHY_MaintenanceComposanteState::GetComposante() const
{
    assert( pComposante_ );
    return *pComposante_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::GetComposantePosition
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_MaintenanceComposanteState::GetComposantePosition() const
{
    return vComposantePosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::NeedDiagnosis
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_MaintenanceComposanteState::NeedDiagnosis() const
{
    return bDiagnosed_ == false;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::NotifyDiagnosed
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::NotifyDiagnosed()
{
    bDiagnosed_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SelectNewState
// Created: MCO 2014-01-30
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SelectNewState()
{
    pConsign_->SelectNewState();
}

void PHY_MaintenanceComposanteState::TransferToLogisticSuperior()
{
    pConsign_->TransferToLogisticSuperior();
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SelectMaintenanceTransporter
// Created: SLI 2014-01-30
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SelectMaintenanceTransporter( const PHY_ComposanteTypePion& type )
{
    pConsign_->SelectMaintenanceTransporter( type );
}

void PHY_MaintenanceComposanteState::SelectDiagnosisTeam( const PHY_ComposanteTypePion& type )
{
    pConsign_->SelectDiagnosisTeam( type );
}

// -----------------------------------------------------------------------------
// Name: PHY_MaintenanceComposanteState::SelectRepairTeam
// Created: ABR 2014-02-04
// -----------------------------------------------------------------------------
void PHY_MaintenanceComposanteState::SelectRepairTeam( const PHY_ComposanteTypePion& type )
{
    pConsign_->SelectRepairTeam( type );
}
