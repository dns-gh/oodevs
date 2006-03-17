// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Logistic/Medical/PHY_MedicalHumanState.cpp $
// $Author: Jvt $
// $Modtime: 29/04/05 11:13 $
// $Revision: 7 $
// $Workfile: PHY_MedicalHumanState.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "PHY_MedicalHumanState.h"
#include "PHY_MedicalConsign_ABC.h"
#include "Entities/Agents/Units/Humans/PHY_Human.h"
#include "Entities/Agents/Units/Composantes/PHY_ComposantePion.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Specialisations/LOG/MIL_AgentPionLOG_ABC.h"
#include "Network/NET_ASN_Messages.h"
#include "Entities/Agents/Units/Humans/PHY_HumanRank.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"
#include "Tools/MIL_IDManager.h"

BOOST_CLASS_EXPORT_GUID( PHY_MedicalHumanState, "PHY_MedicalHumanState" )

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState constructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalHumanState::PHY_MedicalHumanState( MIL_AgentPion& pion, PHY_Human& human, bool bEvacuatedByThirdParty )
    : nID_                   ( MIL_IDManager::medicalHumanStates_.GetFreeSimID() )
    , pPion_                 ( &pion )
    , pHuman_                ( &human )
    , pConsign_              ( 0 )
    , vHumanPosition_        ( pion.GetRole< PHY_RolePion_Location >().GetPosition() )
    , bHasChanged_           ( true )
    , bHumanStateHasChanged_ ( false )
    , bDiagnosed_            ( false )
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
    : nID_                   ()
    , pPion_                 ( 0 )
    , pHuman_                ( 0 )
    , pConsign_              ( 0 )
    , vHumanPosition_        ()
    , bHasChanged_           ( true )
    , bHumanStateHasChanged_ ( false )
    , bDiagnosed_            ( false )
    , bHandledByMedical_     ( false )
    , bShouldGoBackToWar_    ( false )
    , bEvacuatedByThirdParty_( false )
{
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState destructor
// Created: NLD 2004-12-23
// -----------------------------------------------------------------------------
PHY_MedicalHumanState::~PHY_MedicalHumanState()
{
    SendMsgDestruction();    
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::load
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> nID_
         >> pPion_
         >> pHuman_
         >> pConsign_
         >> vHumanPosition_
         >> bDiagnosed_
         >> bShouldGoBackToWar_
         >> bHandledByMedical_
         >> bEvacuatedByThirdParty_;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::save
// Created: JVT 2005-04-11
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    file << nID_
         << pPion_
         << pHuman_
         << pConsign_
         << vHumanPosition_
         << bDiagnosed_
         << bShouldGoBackToWar_
         << bHandledByMedical_
         << bEvacuatedByThirdParty_;
}


// =============================================================================
// ACCESSORS
// =============================================================================
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
    return pPion_->GetRole< PHY_RolePion_Location >().GetPosition();
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
void PHY_MedicalHumanState::GoBackToWar()
{
    if( bShouldGoBackToWar_ )
        pHuman_->NotifyBackToWar();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::Cancel
// Created: NLD 2004-12-24
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::Cancel()
{
    if ( pConsign_ )
    {
        pConsign_->Cancel();
        pConsign_ = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::Heal
// Created: NLD 2005-01-12
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::Heal( const PHY_ComposantePion& doctor )
{
    assert( pHuman_ );
    doctor.Heal( *pHuman_ );
    bHumanStateHasChanged_ = true;
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::IsInAmbulance
// Created: NLD 2005-04-25
// -----------------------------------------------------------------------------
bool PHY_MedicalHumanState::IsInAmbulance() const
{
    return pConsign_ && pConsign_->IsATransportConsign();
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::SendFullState
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::SendFullState() const
{
    assert( pHuman_ );
    assert( pPion_ );

    SendMsgCreation();
    NET_ASN_MsgLogSanteTraitementHumainUpdate asn;
    asn.GetAsnMsg().oid_consigne          = nID_;
    asn.GetAsnMsg().oid_pion              = pPion_->GetID();
    if( pConsign_ )
        pConsign_->SendFullState( asn );
    else
    {
        asn.GetAsnMsg().m.oid_pion_log_traitantPresent = 1;
        asn.GetAsnMsg().m.etatPresent                  = 1;
        asn.GetAsnMsg().oid_pion_log_traitant          = 0;
        asn.GetAsnMsg().etat                           = EnumLogSanteTraitementEtat::termine;
    }

    asn.GetAsnMsg().m.blesse_mentalPresent = 1;
    asn.GetAsnMsg().m.blessurePresent      = 1;
    asn.GetAsnMsg().m.contamine_nbcPresent = 1;        
    asn.GetAsnMsg().blessure               = pHuman_->GetWound().GetAsnID();
    asn.GetAsnMsg().blesse_mental          = pHuman_->IsMentalDiseased();
    asn.GetAsnMsg().contamine_nbc          = pHuman_->IsContaminated();

    asn.Send();
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

    NET_ASN_MsgLogSanteTraitementHumainUpdate asn;
    asn.GetAsnMsg().oid_consigne = nID_;
    asn.GetAsnMsg().oid_pion     = pPion_->GetID();
    if( pConsign_ )
        pConsign_->SendChangedState( asn );
    else
    {
        asn.GetAsnMsg().m.oid_pion_log_traitantPresent = 1;
        asn.GetAsnMsg().m.etatPresent                  = 1;
        asn.GetAsnMsg().oid_pion_log_traitant          = 0;
        asn.GetAsnMsg().etat                           = EnumLogSanteTraitementEtat::termine;
    }
    if( bHumanStateHasChanged_ )
    {
        asn.GetAsnMsg().m.blesse_mentalPresent = 1;
        asn.GetAsnMsg().m.blessurePresent      = 1;
        asn.GetAsnMsg().m.contamine_nbcPresent = 1;        
        asn.GetAsnMsg().blessure               = pHuman_->GetWound().GetAsnID();
        asn.GetAsnMsg().blesse_mental          = pHuman_->IsMentalDiseased();
        asn.GetAsnMsg().contamine_nbc          = pHuman_->IsContaminated();
    }
    asn.Send();
}

// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::Clean
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::Clean()
{
    bHasChanged_           = false;
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

    NET_ASN_MsgLogSanteTraitementHumainCreation asn;
    asn.GetAsnMsg().oid_consigne    = nID_;
    asn.GetAsnMsg().oid_pion        = pPion_->GetID();
    asn.GetAsnMsg().rang            = pHuman_->GetRank ().GetAsnID();
    asn.GetAsnMsg().blessure        = pHuman_->GetWound().GetAsnID();
    asn.GetAsnMsg().blesse_mental   = pHuman_->IsMentalDiseased();
    asn.GetAsnMsg().contamine_nbc   = pHuman_->IsContaminated();
    asn.Send();
}
    
// -----------------------------------------------------------------------------
// Name: PHY_MedicalHumanState::SendMsgDestruction
// Created: NLD 2004-12-29
// -----------------------------------------------------------------------------
void PHY_MedicalHumanState::SendMsgDestruction() const
{
    assert( pPion_ );
    
    NET_ASN_MsgLogSanteTraitementHumainDestruction asn;
    asn.GetAsnMsg().oid_consigne    = nID_;
    asn.GetAsnMsg().oid_pion        = pPion_->GetID();
    asn.Send();
}
