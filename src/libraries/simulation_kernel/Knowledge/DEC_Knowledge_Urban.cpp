// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_Urban.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_ASN_Messages.h"
#include "Tools/MIL_IDManager.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_UrbanPerception.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "urban/TerrainObject_ABC.h"


BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_Urban, "DEC_Knowledge_Urban" )

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban constructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_Knowledge_Urban::DEC_Knowledge_Urban( const MIL_Army_ABC& army, const urban::TerrainObject_ABC& object )
    : DEC_Knowledge_ABC              ()
    , army_                         ( army )
    , object_                       ( object )
    , nID_                           ( MIL_IDManager::GetFreeId() )
    , pCurrentPerceptionLevel_       ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_           ( &PHY_PerceptionLevel::notSeen_ )
    , nTimeLastUpdate_               ( 0 )
    , rRelevance_                    ( 1. )
    , rProgressPercent_              ( 0. )
    , bCreatedOnNetwork_             ( false )
    , bRelevanceUpdated_             ( false )
    , bCurrentPerceptionLevelUpdated_( false )
    , bMaxPerceptionLevelUpdated_    ( false )
    , bLastPerceived_                ( false )
    , rLastRelevanceSent_            ( 0. )
    , rLastProgressSent_              ( 0. )
{
    if ( bCreatedOnNetwork_ )
        SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban destructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_Knowledge_Urban::~DEC_Knowledge_Urban()
{
    if( bCreatedOnNetwork_ )
        SendMsgDestruction();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::load
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::load( MIL_CheckPointInArchive& file, const uint )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         >> const_cast< uint& >( nID_ )
         >> nTimeLastUpdate_;
    
    uint nID;
    file >> nID;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
    
    file >> nID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
    
    file >> nID;
    pMaxPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );

    file >> rRelevance_
         >> bCreatedOnNetwork_
         >> bRelevanceUpdated_
         >> bCurrentPerceptionLevelUpdated_
         >> bMaxPerceptionLevelUpdated_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::save
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::save( MIL_CheckPointOutArchive& file, const uint ) const
{
    unsigned current  = pCurrentPerceptionLevel_->GetID(),
             previous = pPreviousPerceptionLevel_->GetID(),
             max      = pMaxPerceptionLevel_->GetID();
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << nID_
         << nTimeLastUpdate_
         << current
         << previous
         << max
         << rRelevance_
         << bCreatedOnNetwork_
         << bRelevanceUpdated_
         << bCurrentPerceptionLevelUpdated_
         << bMaxPerceptionLevelUpdated_;
}

// =============================================================================
// INTERNAL UPDATERS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::Prepare
// Created: MGD 2009-12-02
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::Prepare()
{
//@TODO MGD
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::Update
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::Update( const DEC_Knowledge_UrbanPerception& perception )
{
    const PHY_PerceptionLevel& level = perception.GetCurrentPerceptionLevel();
    
    float area = object_.GetFootprint()->ComputeArea();
    float progress = level.GetID() * 100 / object_.GetFootprint()->ComputeArea();//@TODO MGD Add true physical in ADN
    rProgressPercent_ = rProgressPercent_ + progress;
    if( rProgressPercent_ > 1.0 )
    {
        pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
        if( pCurrentPerceptionLevel_ != &PHY_PerceptionLevel::identified_ )
        {
            pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( pCurrentPerceptionLevel_->GetID() + 1 );
            rProgressPercent_ -= 1.0;
        }
        else
        {
            rProgressPercent_ = 1.0;
        }
    }
    else if( pCurrentPerceptionLevel_ == &PHY_PerceptionLevel::notSeen_ )
    {
        pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
        pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::detected_;
    }

    UpdatePerceptionSources( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SetPerceptionLevel
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SetPerceptionLevel( const PHY_PerceptionLevel& level )
{
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_ = &level;
    rProgressPercent_ = 0.0;
}

// =============================================================================
// RELEVANCE
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::ChangeRelevance
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::ChangeRelevance( MT_Float rNewRelevance )
{
    if( rRelevance_ == rNewRelevance )
        return;

    static const MT_Float rDeltaForNetwork = 0.05;
    if( fabs( rLastRelevanceSent_ - rNewRelevance ) > rDeltaForNetwork || rNewRelevance == 0. || rNewRelevance == 1. )
        bRelevanceUpdated_ = true;

    rRelevance_ = rNewRelevance;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Object::UpdatePerceptionSources
// Created: MGD 2009-12-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::UpdatePerceptionSources( const DEC_Knowledge_UrbanPerception& perception )
{
    if( perception.GetCurrentPerceptionLevel() == PHY_PerceptionLevel::notSeen_ )
        return;

    const MIL_Automate* pAutomateSource = &perception.GetPerceiver().GetAutomate();   

    IT_PerceptionSourceMap it = perceptionLevelPerAutomateMap_.find( pAutomateSource );
    if( it == perceptionLevelPerAutomateMap_.end() )
        perceptionLevelPerAutomateMap_.insert( std::pair< const MIL_Automate*, const PHY_PerceptionLevel* >( pAutomateSource, &perception.GetCurrentPerceptionLevel() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::UpdateRelevance
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::UpdateRelevance()
{
    // L'agent est percu
    if( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ )
    {
        assert( rRelevance_ == 1. );
        return;
    }

    assert( rRelevance_ >= 0. && rRelevance_ <= 1. );

    //@TODO MGD
    // Degradation : effacement au bout de X minutes
    //const MT_Float rTimeRelevanceDegradation = ( GetCurrentTimeStep() - nTimeLastUpdate_ ) / pKnowledgeGroup_->GetType().GetKnowledgeAgentMaxLifeTime();

    //ChangeRelevance( std::max( 0., rRelevance_ - rTimeRelevanceDegradation ) );

    nTimeLastUpdate_ = GetCurrentTimeStep();
}

// =============================================================================
// NETWORK UPDATERS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::WriteMsgPerceptionSources
// Created: MGD 2009-12-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::WriteMsgPerceptionSources( ASN1T_MsgUrbanKnowledgeUpdate& asnMsg ) const
{
    asnMsg.m.automat_perceptionPresent = 1;

    asnMsg.automat_perception.n    = perceptionLevelPerAutomateMap_.size();
    asnMsg.automat_perception.elem = 0;

    if( !perceptionLevelPerAutomateMap_.empty() )
    {
        ASN1T_OID* pPerceptions = new ASN1T_OID[ perceptionLevelPerAutomateMap_.size() ]; //$$ RAM
        uint i = 0;
        for( CIT_PerceptionSourceMap it = perceptionLevelPerAutomateMap_.begin(); it != perceptionLevelPerAutomateMap_.end(); ++it )
        {
            pPerceptions[i] = it->first->GetID();
        }
        asnMsg.automat_perception.elem = pPerceptions;
    }
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendChangedState
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendChangedState()
{
    NET_ASN_MsgUrbanKnowledgeUpdate asnMsg;
    bool bMustSend = false;

    if( std::abs( rLastRelevanceSent_ - rRelevance_ ) > 0.05 )
    {
        asnMsg().m.relevancePresent = 1;
        asnMsg().relevance = (int)( rRelevance_ * 100. );
        rLastRelevanceSent_ = rRelevance_;
        bMustSend = true;
    }

    if( pCurrentPerceptionLevel_ != pPreviousPerceptionLevel_ )
    {
        asnMsg().m.identification_levelPresent = 1;
        pCurrentPerceptionLevel_->Serialize( asnMsg().identification_level );
        bMustSend = true;
    }

    if( std::abs( rLastProgressSent_ - rProgressPercent_ ) >= 0.01 )
    {
        asnMsg().m.progressPresent = 1;
        asnMsg().progress = (int)( rProgressPercent_ * 100. );
        rLastProgressSent_ = rProgressPercent_;
        bMustSend = true;
    }

    if( perceptionLevelPerAutomateMap_.size() == 0 && bLastPerceived_ )
    {
        asnMsg().m.perceivedPresent = 1;
        asnMsg().perceived = 0;
        bLastPerceived_ = false;
        bMustSend = true;
    }
    else if( perceptionLevelPerAutomateMap_.size() > 0 && !bLastPerceived_ )
    {
        asnMsg().m.perceivedPresent = 1;
        asnMsg().perceived = 1;
        bLastPerceived_ = true;
        bMustSend = true;
    }

    if( bMustSend )
    {
        nTimeLastUpdate_ = GetCurrentTimeStep();
        asnMsg().oid = nID_;
        asnMsg().team = army_.GetID();
        asnMsg().real_urban = object_.GetId();
        asnMsg.Send();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendFullState
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendFullState()
{
    NET_ASN_MsgUrbanKnowledgeUpdate asnMsg;
    asnMsg().oid = nID_;
    asnMsg().team = army_.GetID();
    asnMsg().real_urban = object_.GetId();


    asnMsg().m.relevancePresent = 1;
    asnMsg().relevance = (int)( rRelevance_ * 100. );
    rLastRelevanceSent_ = rRelevance_;

    asnMsg().m.identification_levelPresent = 1;
    pCurrentPerceptionLevel_->Serialize( asnMsg().identification_level );

    asnMsg().m.relevancePresent = 1;
    asnMsg().relevance = (int)( rRelevance_ * 100. );

    asnMsg().m.progressPresent = 1;
    asnMsg().progress = (int)( rProgressPercent_ * 100. );
    rLastProgressSent_ = rProgressPercent_;

    asnMsg().m.perceivedPresent = 1;
    asnMsg().perceived = 1;
    bLastPerceived_ = true;

    WriteMsgPerceptionSources( asnMsg() );

    asnMsg.Send();

    if( asnMsg().m.automat_perceptionPresent && asnMsg().automat_perception.n > 0 )
        delete [] asnMsg().automat_perception.elem; //$$$ RAM
}
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::UpdateOnNetwork
// Created:MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::UpdateOnNetwork()
{
    if( !bCreatedOnNetwork_ )
    {
        SendMsgCreation();
        bCreatedOnNetwork_  = true;
        SendFullState();
        return;
    }
    SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendStateToNewClient
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendStateToNewClient()
{
    if( bCreatedOnNetwork_ )
    {
        SendMsgCreation();
        SendFullState  ();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendMsgCreation
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendMsgCreation() const
{
    NET_ASN_MsgUrbanKnowledgeCreation asnMsg;
    asnMsg().oid = nID_;
    asnMsg().team = army_.GetID();
    asnMsg().real_urban = object_.GetId();

    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendMsgDestruction
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendMsgDestruction() const
{
    NET_ASN_MsgUrbanKnowledgeDestruction asnMsg;
    asnMsg().oid = nID_;
    asnMsg().team = army_.GetID();

    asnMsg.Send();
}

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::GetRelevance
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Urban::GetRelevance() const
{
    return rRelevance_;    
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::Clean
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Urban::Clean() const
{
    return GetRelevance() <= 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::IsValid
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
unsigned DEC_Knowledge_Urban::GetId  () const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::GetLevel
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_Urban::GetCurrentPerceptionLevel() const
{
    return *pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::GetProgress
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
double DEC_Knowledge_Urban::GetProgress() const
{
    return rProgressPercent_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::IsValid
// Created: MGD 2009-12-04
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Urban::IsValid() const
{
    return true; //@TODO MGD
}
