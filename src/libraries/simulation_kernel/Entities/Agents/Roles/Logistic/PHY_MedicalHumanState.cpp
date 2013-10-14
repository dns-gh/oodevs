// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/PHY_MedicalHumanState.cpp $
// $Author: Jvt $
// $Modtime: 29/04/05 11:13 $
// $Revision: 7 $
// $Workfile: PHY_MedicalHumanState.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "PHY_MedicalHumanState.h"
#include "MIL_Time_ABC.h"
#include "PHY_MedicalConsign_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "Entities/Agents/Units/Humans/PHY_HumanWound.h"
#include "Network/NET_Publisher_ABC.h"
#include "Tools/MIL_IDManager.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( PHY_MedicalHumanState )

namespace
{
    MIL_IDManager idManager;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalHumanState::PHY_MedicalHumanState( MIL_AgentPion& pion, Human_ABC& human, bool bEvacuatedByThirdParty )
    : nID_                   ( idManager.GetId() )
    , nCreationTick_         ( MIL_Time_ABC::GetTime().GetCurrentTimeStep() )
    , pPion_                 ( &pion )
    , pHuman_                ( &human )
    , pConsign_              ( 0 )
    , vHumanPosition_        ( pion.GetRole< PHY_RoleInterface_Location >().GetPosition() )
    , bHasChanged_           ( true )
    , bHumanStateHasChanged_ ( false )
    , bDiagnosed_            ( false )
    , bSorted_               ( false )
    , bHandledByMedical_     ( false )
    , bShouldGoBackToWar_    ( false )
    , bEvacuatedByThirdParty_( bEvacuatedByThirdParty )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState constructor
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
PHY_MedicalHumanState::PHY_MedicalHumanState()
    : nID_                   ( 0 )
    , nCreationTick_         ( 0 )
    , pPion_                 ( 0 )
    , pHuman_                ( 0 )
    , pConsign_              ( 0 )
    , bHasChanged_           ( true )
    , bHumanStateHasChanged_ ( false )
    , bDiagnosed_            ( false )
    , bSorted_               ( false )
    , bHandledByMedical_     ( false )
    , bShouldGoBackToWar_    ( false )
    , bEvacuatedByThirdParty_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalHumanState::~PHY_MedicalHumanState()
{
    SendMsgDestruction();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::load
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> const_cast< unsigned int& >( nID_ )
         >> const_cast< unsigned int& >( nCreationTick_ )
         >> pPion_
         >> pHuman_
         >> pConsign_
         >> vHumanPosition_
         >> bDiagnosed_
         >> bSorted_
         >> bShouldGoBackToWar_
         >> bHandledByMedical_
         >> bEvacuatedByThirdParty_;
    idManager.GetId( nID_, true );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::save
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << nID_
         << nCreationTick_
         << pPion_
         << pHuman_
         << pConsign_
         << vHumanPosition_
         << bDiagnosed_
         << bSorted_
         << bShouldGoBackToWar_
         << bHandledByMedical_
         << bEvacuatedByThirdParty_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::IsAnEmergency
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
bool PHY_MedicalHumanState::IsAnEmergency() const
{
    assert( pHuman_ );
    return pHuman_->IsAnEmergency();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::GetAutomate
// Created: NLD 2005-01-11
// -----------------------------------------------------------------------------
const MIL_Automate& PHY_MedicalHumanState::GetAutomate() const
{
    assert( pPion_ );
    return pPion_->GetAutomate();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::GetPionPosition
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_MedicalHumanState::GetPionPosition() const
{
    assert( pPion_ );
    return pPion_->GetRole< PHY_RoleInterface_Location >().GetPosition();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::NotifyHandledByMedical
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::NotifyHandledByMedical()
{
    assert( pHuman_ );
    pHuman_->NotifyHandledByMedical();
    bShouldGoBackToWar_ = !( pHuman_->IsWounded() || pHuman_->IsContaminated() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::GoBackToWar
// Created: JVT 2005-04-27
// -----------------------------------------------------------------------------
bool PHY_MedicalHumanState::GoBackToWar()
{
    if( !bShouldGoBackToWar_ )
        return false;
    return pHuman_->NotifyBackToWar();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::Cancel
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::Cancel()
{
    if( pConsign_ )
    {
        pConsign_->Cancel();
        pConsign_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::Heal
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
unsigned int PHY_MedicalHumanState::Heal( const PHY_ComposantePion& doctor )
{
    assert( pHuman_ );
    bHumanStateHasChanged_ = true;
    return doctor.Heal( *pHuman_ );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::IsInAmbulance
// Created: NLD 2005-04-25
// -----------------------------------------------------------------------------
bool PHY_MedicalHumanState::IsInAmbulance() const
{
    return pConsign_ && pConsign_->IsATransportConsign();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::SendFullState( unsigned int context ) const
{
    assert( pHuman_ );
    assert( pPion_ );

    SendMsgCreation();
    client::LogMedicalHandlingUpdate asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_unit()->set_id( pPion_->GetID() );

    if( pConsign_ )
        pConsign_->SendFullState( asn );
    else
    {
        asn().mutable_provider()->set_id( 0 );
        asn().set_state( sword::LogMedicalHandlingUpdate::finished );
    }

    asn().set_wound( pHuman_->GetWound().GetAsnID() );
    asn().set_mental_wound( pHuman_->IsMentalDiseased() );
    asn().set_nbc_contaminated( pHuman_->IsContaminated() );
    asn().set_diagnosed( bDiagnosed_ );

    asn.Send( NET_Publisher_ABC::Publisher(), context );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::SendChangedState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::SendChangedState() const
{
    if( !( bHasChanged_ || bHumanStateHasChanged_ || ( pConsign_ && pConsign_->HasChanged() ) ) )
        return;
    assert( pPion_ );
    assert( pHuman_ );
    client::LogMedicalHandlingUpdate asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_unit()->set_id( pPion_->GetID() );
    if( pConsign_ )
        pConsign_->SendChangedState( asn );
    else
    {
        asn().mutable_provider()->set_id( 0 );
        asn().set_state( sword::LogMedicalHandlingUpdate::finished );
    }
    if( bHumanStateHasChanged_ )
    {
        asn().set_wound( pHuman_->GetWound().GetAsnID() );
        asn().set_mental_wound( pHuman_->IsMentalDiseased() );
        asn().set_nbc_contaminated( pHuman_->IsContaminated() );
    }
    asn().set_diagnosed( bDiagnosed_ );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::Clean
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::Clean()
{
    bHasChanged_ = false;
    bHumanStateHasChanged_ = false;
    if( pConsign_ )
        pConsign_->Clean();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::SendMsgCreation
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::SendMsgCreation() const
{
    assert( pPion_ );
    assert( pHuman_ );
    client::LogMedicalHandlingCreation asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_unit()->set_id( pPion_->GetID() );
    asn().set_tick( nCreationTick_ );
    asn().set_rank( pHuman_->GetRank ().GetAsnID() );
    asn().set_wound( pHuman_->GetWound().GetAsnID() );
    asn().set_mental_wound( pHuman_->IsMentalDiseased() );
    asn().set_nbc_contaminated( pHuman_->IsContaminated() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::SendMsgDestruction() const
{
    assert( pPion_ );
    client::LogMedicalHandlingDestruction asn;
    asn().mutable_request()->set_id( nID_ );
    asn().mutable_unit()->set_id( pPion_->GetID() );
    asn.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::GetHuman
// Created: NLD 2005-01-10
// -----------------------------------------------------------------------------
const Human_ABC& PHY_MedicalHumanState::GetHuman() const
{
    assert( pHuman_ );
    return *pHuman_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::SetConsign
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::SetConsign( PHY_MedicalConsign_ABC* pConsign )
{
    if( pConsign == pConsign_ )
        return;
    pConsign_    = pConsign;
    bHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::SetHumanPosition
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::SetHumanPosition( const MT_Vector2D& vPosition )
{
    vHumanPosition_ = vPosition;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::GetHumanPosition
// Created: NLD 2004-12-27
// -----------------------------------------------------------------------------
const MT_Vector2D& PHY_MedicalHumanState::GetHumanPosition() const
{
    return vHumanPosition_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::NeedDiagnosis
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
bool PHY_MedicalHumanState::NeedDiagnosis() const
{
    return !bDiagnosed_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::NotifyDiagnosed
// Created: NLD 2004-12-28
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::NotifyDiagnosed()
{
    bDiagnosed_ = true;

    assert( pHuman_ );
    if( pHuman_ )
        pHuman_->NotifyDiagnosed();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::NeedSorting
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
bool PHY_MedicalHumanState::NeedSorting() const
{
    return !bSorted_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::NotifySorted
// Created: NLD 2006-04-04
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::NotifySorted()
{
    bSorted_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::NotifyHumanChanged
// Created: NLD 2005-01-14
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::NotifyHumanChanged()
{
    bHumanStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::ShouldGoBackToWar
// Created: JVT 2005-04-28
// -----------------------------------------------------------------------------
bool PHY_MedicalHumanState::ShouldGoBackToWar() const
{
    return bShouldGoBackToWar_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::EvacuatedByThirdParty
// Created: NLD 2005-08-01
// -----------------------------------------------------------------------------
bool PHY_MedicalHumanState::EvacuatedByThirdParty() const
{
    return bEvacuatedByThirdParty_;
}
