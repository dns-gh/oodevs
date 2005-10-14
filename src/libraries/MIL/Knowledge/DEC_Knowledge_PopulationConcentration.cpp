// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationConcentration.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_PopulationConcentration.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_Knowledge_PopulationConcentration.h"

#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationConcentrationPerception.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"

MIL_MOSIDManager DEC_Knowledge_PopulationConcentration::idManager_;

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationConcentration, "DEC_Knowledge_PopulationConcentration" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentration::DEC_Knowledge_PopulationConcentration( DEC_Knowledge_Population& populationKnowledge, const MIL_PopulationConcentration& concentrationKnown )
    : pPopulationKnowledge_     ( &populationKnowledge    )
    , pConcentrationKnown_      ( &concentrationKnown )
    , nID_                      ( idManager_.GetFreeSimID() )
    , position_                 ( concentrationKnown.GetPosition() )
    , nNbrAliveHumans_          ( 0 )
    , nNbrDeadHumans_           ( 0 )
    , pAttitude_                ( 0 ) //$$
    , bHumansUpdated_           ( true )
    , bAttitudeUpdated_         ( true )
    , bRealConcentrationUpdated_( true )
    , bRelevanceUpdated_        ( true )
    , rRelevance_               ( 1. )
    , pPreviousPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pCurrentPerceptionLevel_  ( &PHY_PerceptionLevel::notSeen_ )
{
    SendMsgCreation();       
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentration::DEC_Knowledge_PopulationConcentration()
    : pPopulationKnowledge_     ( 0 )
    , pConcentrationKnown_      ( 0 )
    , nID_                      ( 0 )
    , position_                 ( 0., 0. )
    , nNbrAliveHumans_          ( 0 )
    , nNbrDeadHumans_           ( 0 )
    , pAttitude_                ( 0 ) //$$
    , bHumansUpdated_           ( true )
    , bAttitudeUpdated_         ( true )
    , bRealConcentrationUpdated_( true )
    , bRelevanceUpdated_        ( true )
    , rRelevance_               ( 1. )
    , pPreviousPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pCurrentPerceptionLevel_  ( &PHY_PerceptionLevel::notSeen_ )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentration::~DEC_Knowledge_PopulationConcentration()
{
    SendMsgDestruction();
    idManager_.ReleaseSimID( nID_ );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::load( MIL_CheckPointInArchive& file, const uint )
{
    assert( false );
//    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
//
//    file >> const_cast< MIL_AgentPion*& >( pAgentPerceiving_ ) 
//         >> pPopulationPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    assert( false );
//    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
//         << const_cast< MIL_AgentPion*& >( pAgentPerceiving_ ) 
//         << pPopulationPerceived_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::Prepare
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::Prepare()
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_  = &PHY_PerceptionLevel::notSeen_; 
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::Update
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::Update( const DEC_Knowledge_PopulationConcentrationPerception& perception )
{
    pCurrentPerceptionLevel_ = &perception.GetCurrentPerceptionLevel();

    if( nNbrAliveHumans_ != perception.GetNbrAliveHumans() )
    {
        nNbrAliveHumans_ = perception.GetNbrAliveHumans();
        bHumansUpdated_  = true;
    }
    if( nNbrDeadHumans_ != perception.GetNbrDeadHumans() )
    {
        nNbrDeadHumans_  = perception.GetNbrDeadHumans();
        bHumansUpdated_  = true;
    }

    if( !pAttitude_ || *pAttitude_ != perception.GetAttitude() )
    {
        pAttitude_        = &perception.GetAttitude();
        bAttitudeUpdated_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::UpdateRelevance
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::UpdateRelevance()
{
    assert( rRelevance_ > 0. );

    if( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ )
    {
        assert( pConcentrationKnown_ && pConcentrationKnown_->IsValid() );
        if( rRelevance_ != 1. )
        {
            rRelevance_        = 1.;
            bRelevanceUpdated_ = true;
        }
        return;
    }

    // L'objet réel va être détruit
    if( pConcentrationKnown_ && !pConcentrationKnown_->IsValid() )
    {
        pConcentrationKnown_       = 0;
        bRealConcentrationUpdated_ = true;
    }

    // Si plus d'objet réel associé est si l'emplacement de l'objet est vu
//    assert( pArmyKnowing_ );
//    if ( !pConcentrationKnown_ && pArmyKnowing_->IsPerceived( *this ) )
//    {
//        rRelevance_ = 0.;
//        NotifyAttributeUpdated( eAttr_Relevance );
//        return;
//    }   
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::Clean
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
bool DEC_Knowledge_PopulationConcentration::Clean()
{
    bHumansUpdated_            = false;
    bAttitudeUpdated_          = false;  
    bRealConcentrationUpdated_ = false;
    bRelevanceUpdated_         = false;
    return false;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::SendMsgCreation
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::SendMsgCreation() const
{
    assert( pPopulationKnowledge_ );

    NET_ASN_MsgPopulationConcentrationKnowledgeCreation asnMsg;

    asnMsg.GetAsnMsg().oid_connaissance_concentration = nID_;
    asnMsg.GetAsnMsg().oid_connaissance_population    = pPopulationKnowledge_->GetID();
    asnMsg.GetAsnMsg().oid_groupe_possesseur          = pPopulationKnowledge_->GetKnowledgeGroup().GetID();
    asnMsg.GetAsnMsg().oid_concentration_reelle       = pConcentrationKnown_ ? pConcentrationKnown_->GetID() : 0;

    NET_ASN_Tools::WritePoint( position_, asnMsg.GetAsnMsg().position );

    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::SendMsgDestruction
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::SendMsgDestruction() const
{
    assert( pPopulationKnowledge_ );

    NET_ASN_MsgPopulationConcentrationKnowledgeDestruction asnMsg;

    asnMsg.GetAsnMsg().oid_connaissance_concentration = nID_;
    asnMsg.GetAsnMsg().oid_connaissance_population    = pPopulationKnowledge_->GetID();
    asnMsg.GetAsnMsg().oid_groupe_possesseur          = pPopulationKnowledge_->GetKnowledgeGroup().GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::SendFullState
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::SendFullState() const
{
    assert( pPopulationKnowledge_ );
    assert( pAttitude_ );

    NET_ASN_MsgPopulationConcentrationKnowledgeUpdate asnMsg;

    asnMsg.GetAsnMsg().oid_connaissance_concentration = nID_;
    asnMsg.GetAsnMsg().oid_connaissance_population    = pPopulationKnowledge_->GetID();
    asnMsg.GetAsnMsg().oid_groupe_possesseur          = pPopulationKnowledge_->GetKnowledgeGroup().GetID();
    
    asnMsg.GetAsnMsg().m.nb_humains_mortsPresent         = 1;
    asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent       = 1;
    asnMsg.GetAsnMsg().m.attitudePresent                 = 1;
    asnMsg.GetAsnMsg().m.est_percuPresent                = 1;
    asnMsg.GetAsnMsg().m.oid_concentration_reellePresent = 1;
    asnMsg.GetAsnMsg().m.pertinencePresent               = 1;

    asnMsg.GetAsnMsg().nb_humains_morts         = nNbrDeadHumans_;
    asnMsg.GetAsnMsg().nb_humains_vivants       = nNbrAliveHumans_;
    asnMsg.GetAsnMsg().attitude                 = pAttitude_->GetAsnID();
    asnMsg.GetAsnMsg().est_percu                = ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ );
    asnMsg.GetAsnMsg().oid_concentration_reelle = pConcentrationKnown_ ? pConcentrationKnown_->GetID() : 0;
    asnMsg.GetAsnMsg().pertinence               = (uint)( rRelevance_ * 100. );

    asnMsg.Send();    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::UpdateOnNetwork() const
{
    assert( pPreviousPerceptionLevel_ );
    assert( pCurrentPerceptionLevel_  );
    assert( pAttitude_ );

    if( *pPreviousPerceptionLevel_ == *pCurrentPerceptionLevel_ && !bHumansUpdated_ && !bAttitudeUpdated_ && !bRealConcentrationUpdated_ && !bRelevanceUpdated_ )
        return;

    NET_ASN_MsgPopulationConcentrationKnowledgeUpdate asnMsg;

    asnMsg.GetAsnMsg().oid_connaissance_concentration = nID_;
    asnMsg.GetAsnMsg().oid_connaissance_population    = pPopulationKnowledge_->GetID();
    asnMsg.GetAsnMsg().oid_groupe_possesseur          = pPopulationKnowledge_->GetKnowledgeGroup().GetID();

    if( *pPreviousPerceptionLevel_ != *pCurrentPerceptionLevel_ )
    {
        asnMsg.GetAsnMsg().m.est_percuPresent = 1;
        asnMsg.GetAsnMsg().est_percu          = ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ );
    }

    if( bHumansUpdated_ )
    {
        asnMsg.GetAsnMsg().m.nb_humains_mortsPresent   = 1;
        asnMsg.GetAsnMsg().m.nb_humains_vivantsPresent = 1;
        asnMsg.GetAsnMsg().nb_humains_morts            = nNbrDeadHumans_;
        asnMsg.GetAsnMsg().nb_humains_vivants          = nNbrAliveHumans_;
    }

    if( bAttitudeUpdated_ )
    {
        asnMsg.GetAsnMsg().m.attitudePresent = 1;
        asnMsg.GetAsnMsg().attitude           = pAttitude_->GetAsnID();
    }

    if( bRealConcentrationUpdated_ )
    {
        asnMsg.GetAsnMsg().m.oid_concentration_reellePresent = 1;
        asnMsg.GetAsnMsg().oid_concentration_reelle = pConcentrationKnown_ ? pConcentrationKnown_->GetID() : 0;
    }

    if( bRelevanceUpdated_ )
    {
        asnMsg.GetAsnMsg().m.pertinencePresent = 1;
        asnMsg.GetAsnMsg().pertinence          = (uint)( rRelevance_ * 100. );
    }

    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::SendStateToNewClient() const
{
    SendMsgCreation();
    SendFullState  ();
}
