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

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_PopulationConcentration.h"

#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationConcentrationPerception.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationAttitude.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionLevel.h"
#include "Network/NET_ASN_Messages.h"
#include "Network/NET_ASN_Tools.h"
#include "CheckPoints/MIL_CheckPointSerializationHelpers.h"
#include "Tools/MIL_IDManager.h"
#include "MIL_AgentServer.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_PopulationConcentration, "DEC_Knowledge_PopulationConcentration" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentration::DEC_Knowledge_PopulationConcentration( DEC_Knowledge_Population& populationKnowledge, const MIL_PopulationConcentration& concentrationKnown )
    : pPopulationKnowledge_     ( &populationKnowledge    )
    , pConcentrationKnown_      ( &concentrationKnown )
    , nID_                      ( MIL_IDManager::GetFreeId() )
    , nTimeLastUpdate_          ( 0 )
    , position_                 ( concentrationKnown.GetPosition() )
    , rNbrAliveHumans_          ( 0. )
    , rNbrDeadHumans_           ( 0. )
    , pAttitude_                ( 0 ) 
    , bHumansUpdated_           ( true )
    , bAttitudeUpdated_         ( true )
    , bRealConcentrationUpdated_( true )
    , bRelevanceUpdated_        ( true )
    , rRelevance_               ( 1. )
    , pPreviousPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pCurrentPerceptionLevel_  ( &PHY_PerceptionLevel::notSeen_ )
    , bReconAttributesValid_    ( false )
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
    , nTimeLastUpdate_          ( 0 )
    , position_                 ( 0., 0. )
    , rNbrAliveHumans_          ( 0. )
    , rNbrDeadHumans_           ( 0. )
    , pAttitude_                ( 0 ) //$$
    , bHumansUpdated_           ( true )
    , bAttitudeUpdated_         ( true )
    , bRealConcentrationUpdated_( true )
    , bRelevanceUpdated_        ( true )
    , rRelevance_               ( 1. )
    , pPreviousPerceptionLevel_ ( &PHY_PerceptionLevel::notSeen_ )
    , pCurrentPerceptionLevel_  ( &PHY_PerceptionLevel::notSeen_ )
    , bReconAttributesValid_    ( false )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentration::~DEC_Knowledge_PopulationConcentration()
{
    SendMsgDestruction();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::load
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> const_cast< DEC_Knowledge_Population*&    >( pPopulationKnowledge_ )
         >> const_cast< MIL_PopulationConcentration*& >( pConcentrationKnown_  )
         >> const_cast< uint&                         >( nID_                  )
         >> nTimeLastUpdate_
         >> position_
         >> rNbrAliveHumans_
         >> rNbrDeadHumans_
         >> bReconAttributesValid_;

    uint nTmpID;
    bool bAttitudeValid;
    file >> bAttitudeValid;
    if( bAttitudeValid )
    {
        file >> nTmpID;
        pAttitude_ = MIL_PopulationAttitude::Find( nTmpID );
        assert( pAttitude_ );
    }

    file >> rRelevance_;

    file >> nTmpID;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nTmpID );
    assert( pCurrentPerceptionLevel_ );

    file >> nTmpID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nTmpID );
    assert( pPreviousPerceptionLevel_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::save
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    bool attitude = ( pAttitude_ != 0 );
    file << pPopulationKnowledge_
         << pConcentrationKnown_
         << nID_
         << nTimeLastUpdate_
         << position_
         << rNbrAliveHumans_
         << rNbrDeadHumans_
         << bReconAttributesValid_
         << attitude;

    
    if( pAttitude_ )
    {
        unsigned attitude = pAttitude_->GetID();
        file << attitude;
    }
    unsigned current  = pCurrentPerceptionLevel_->GetID(),
             previous = pPreviousPerceptionLevel_->GetID();
    file << rRelevance_
         << current
         << previous;
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
    nTimeLastUpdate_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();

    pCurrentPerceptionLevel_ = &perception.GetCurrentPerceptionLevel();

    assert( pPopulationKnowledge_ );
    if( pPopulationKnowledge_->IsRecon() )
    {
        bReconAttributesValid_ = true;
        if( rNbrAliveHumans_ != perception.GetNbrAliveHumans() )
        {
            rNbrAliveHumans_ = perception.GetNbrAliveHumans();
            bHumansUpdated_  = true;
        }
        if( rNbrDeadHumans_ != perception.GetNbrDeadHumans() )
        {
            rNbrDeadHumans_  = perception.GetNbrDeadHumans();
            bHumansUpdated_  = true;
        }

        if( !pAttitude_ || *pAttitude_ != perception.GetAttitude() )
        {
            pAttitude_        = &perception.GetAttitude();
            bAttitudeUpdated_ = true;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::Update
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::Update( const DEC_Knowledge_PopulationCollision& /*collision*/ )
{
    nTimeLastUpdate_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();    
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
        ChangeRelevance( 1. );
        return;
    }

    assert( rRelevance_ >= 0. && rRelevance_ <= 1. );

    // Magic move de la population percue
    if( pConcentrationKnown_ && pConcentrationKnown_->GetPopulation().HasDoneMagicMove() ) 
        ChangeRelevance( 0. );
   
    // Degradation : effacement au bout de X minutes
    const MT_Float rTimeRelevanceDegradation = ( MIL_AgentServer::GetWorkspace().GetCurrentTimeStep() - nTimeLastUpdate_ ) / pPopulationKnowledge_->GetKnowledgeGroup().GetType().GetKnowledgePopulationMaxLifeTime();
    const MT_Float rRelevance                = std::max( 0., rRelevance_ - rTimeRelevanceDegradation );
    ChangeRelevance( rRelevance );
    nTimeLastUpdate_ = MIL_AgentServer::GetWorkspace().GetCurrentTimeStep();

    // L'objet réel va être détruit
    if( pConcentrationKnown_ && !pConcentrationKnown_->IsValid() )
    {
        pConcentrationKnown_       = 0;
        bRealConcentrationUpdated_ = true;
    }
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

    asnMsg().oid_connaissance_concentration = nID_;
    asnMsg().oid_connaissance_population    = pPopulationKnowledge_->GetID();
    asnMsg().oid_groupe_possesseur          = pPopulationKnowledge_->GetKnowledgeGroup().GetID();
    asnMsg().oid_concentration_reelle       = pConcentrationKnown_ ? pConcentrationKnown_->GetID() : 0;

    NET_ASN_Tools::WritePoint( position_, asnMsg().position );

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

    asnMsg().oid_connaissance_concentration = nID_;
    asnMsg().oid_connaissance_population    = pPopulationKnowledge_->GetID();
    asnMsg().oid_groupe_possesseur          = pPopulationKnowledge_->GetKnowledgeGroup().GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::SendFullState
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::SendFullState() 
{
    assert( pPopulationKnowledge_ );

    NET_ASN_MsgPopulationConcentrationKnowledgeUpdate asnMsg;

    asnMsg().oid_connaissance_concentration = nID_;
    asnMsg().oid_connaissance_population    = pPopulationKnowledge_->GetID();
    asnMsg().oid_groupe_possesseur          = pPopulationKnowledge_->GetKnowledgeGroup().GetID();
    
    asnMsg().m.est_percuPresent                = 1;
    asnMsg().m.oid_concentration_reellePresent = 1;
    asnMsg().m.pertinencePresent               = 1;

    asnMsg().est_percu                = ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ );
    asnMsg().oid_concentration_reelle = pConcentrationKnown_ ? pConcentrationKnown_->GetID() : 0;
    asnMsg().pertinence               = (uint)( rRelevance_ * 100. );
    rLastRelevanceSent_ = rRelevance_;

    if( bReconAttributesValid_ )
    {
        assert( pAttitude_ );
        asnMsg().m.nb_humains_mortsPresent   = 1;
        asnMsg().m.nb_humains_vivantsPresent = 1;
        asnMsg().m.attitudePresent           = 1;
    
        asnMsg().nb_humains_morts         = uint( floor( rNbrDeadHumans_  + 0.5f ) );
        asnMsg().nb_humains_vivants       = uint( floor( rNbrAliveHumans_ + 0.5f ) );
        asnMsg().attitude                 = pAttitude_->GetAsnID();
    }

    asnMsg.Send();    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::UpdateOnNetwork()
{
    assert( pPreviousPerceptionLevel_ );
    assert( pCurrentPerceptionLevel_  );

    if( *pPreviousPerceptionLevel_ == *pCurrentPerceptionLevel_ && !bHumansUpdated_ && !bAttitudeUpdated_ && !bRealConcentrationUpdated_ && !bRelevanceUpdated_ )
        return;

    NET_ASN_MsgPopulationConcentrationKnowledgeUpdate asnMsg;

    asnMsg().oid_connaissance_concentration = nID_;
    asnMsg().oid_connaissance_population    = pPopulationKnowledge_->GetID();
    asnMsg().oid_groupe_possesseur          = pPopulationKnowledge_->GetKnowledgeGroup().GetID();

    if( *pPreviousPerceptionLevel_ != *pCurrentPerceptionLevel_ )
    {
        asnMsg().m.est_percuPresent = 1;
        asnMsg().est_percu          = ( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ );
    }

    if( bRealConcentrationUpdated_ )
    {
        asnMsg().m.oid_concentration_reellePresent = 1;
        asnMsg().oid_concentration_reelle = pConcentrationKnown_ ? pConcentrationKnown_->GetID() : 0;
    }

    if( bRelevanceUpdated_ )
    {
        asnMsg().m.pertinencePresent = 1;
        asnMsg().pertinence          = (uint)( rRelevance_ * 100. );
        rLastRelevanceSent_                    = rRelevance_;
    }

    if( bReconAttributesValid_ )
    {
        assert( pAttitude_ );
        if( bHumansUpdated_ )
        {
            asnMsg().m.nb_humains_mortsPresent   = 1;
            asnMsg().m.nb_humains_vivantsPresent = 1;
            asnMsg().nb_humains_morts            = uint( floor( rNbrDeadHumans_  + 0.5f ) );
            asnMsg().nb_humains_vivants          = uint( floor( rNbrAliveHumans_ + 0.5f ) );
        }

        if( bAttitudeUpdated_ )
        {
            asnMsg().m.attitudePresent = 1;
            asnMsg().attitude          = pAttitude_->GetAsnID();
        }
    }

    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_PopulationConcentration::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_PopulationConcentration::SendStateToNewClient()
{
    SendMsgCreation();
    SendFullState  ();
}
