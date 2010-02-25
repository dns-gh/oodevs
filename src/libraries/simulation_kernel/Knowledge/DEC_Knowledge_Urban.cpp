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
#include "Network/NET_Publisher_ABC.h"
#include "simulation_kernel/Knowledge/DEC_Knowledge_UrbanPerception.h"
#include "simulation_kernel/Entities/MIL_Army_ABC.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "simulation_kernel/Entities/Automates/MIL_Automate.h"
#include "simulation_kernel/Entities/MIL_EntityManager.h"
#include "simulation_kernel/MIL_AgentServer.h"
#include "urban/TerrainObject_ABC.h"
#include "protocol/ClientSenders.h"
#include "urban/Soil.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_Urban )

MIL_IDManager DEC_Knowledge_Urban::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban constructor
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
DEC_Knowledge_Urban::DEC_Knowledge_Urban( const MIL_Army_ABC& army, const urban::TerrainObject_ABC& object )
    : DEC_Knowledge_ABC              ()
    , army_                          ( army )
    , object_                        ( object )
    , nID_                           ( idManager_.GetFreeId() )
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


 // $$$$ _RC_ SLG 2010-02-11: CHECKPOINTS mis en commentaire car pas de fonction de serialization dans URBAN, 
// A decommenter des que c'est fait dans la lib urban
// =============================================================================
// CHECKPOINTS
// =============================================================================
// $$$$ LDC - RC Serialization is commented because urban lib doesn't offer serialisation yet.
/*
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
*/
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
    float complexity = object_.ComputeComplexity(); // ALGO TEMPORAIRE
    float progress = level.GetID() * 100 / complexity;//@TODO MGD Add true physical in ADN
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
    const MIL_Agent_ABC& pionSource = perception.GetPerceiver();


    IT_PerceptionSourceMap it = perceptionLevelPerAutomateMap_.find( pAutomateSource );
    if( it == perceptionLevelPerAutomateMap_.end() )
        perceptionLevelPerAutomateMap_.insert( std::pair< const MIL_Automate*, const PHY_PerceptionLevel* >( pAutomateSource, &perception.GetCurrentPerceptionLevel() ) );

    IT_PerceptionAgentSourceMap it2 = perceptionLevelPerAgentMap_.find( &pionSource );
    if( it2 == perceptionLevelPerAgentMap_.end() )
        perceptionLevelPerAgentMap_.insert( std::pair< const MIL_Agent_ABC*, const PHY_PerceptionLevel* >( &pionSource, &perception.GetCurrentPerceptionLevel() ) );

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
void DEC_Knowledge_Urban::WriteMsgPerceptionSources( MsgsSimToClient::MsgUrbanKnowledgeUpdate& message ) const
{
// $$$$ FDS 2010-01-13: Modif à faire valider avant suppression du commentaire
/*
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
*/
    if( !perceptionLevelPerAutomateMap_.empty() )
        for( CIT_PerceptionSourceMap it = perceptionLevelPerAutomateMap_.begin(); it != perceptionLevelPerAutomateMap_.end(); ++it )
            message.mutable_automat_perception()->add_elem( it->first->GetID() );
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendChangedState
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendChangedState()
{
    client::UrbanKnowledgeUpdate message;
    bool bMustSend = false;

    if( std::abs( rLastRelevanceSent_ - rRelevance_ ) > 0.05 )
    {
        message().set_relevance( (int)( rRelevance_ * 100. ) );
        rLastRelevanceSent_ = rRelevance_;
        bMustSend = true;
    }

    if( pCurrentPerceptionLevel_ != pPreviousPerceptionLevel_ )
    {
        MsgsSimToClient::EnumUnitIdentificationLevel level( message().identification_level() );
        pCurrentPerceptionLevel_->Serialize( level );
        bMustSend = true;
    }

    if( std::abs( rLastProgressSent_ - rProgressPercent_ ) >= 0.01 )
    {
        message().set_progress( (int)( rProgressPercent_ * 100. ) );
        rLastProgressSent_ = rProgressPercent_;
        bMustSend = true;
    }

    if( perceptionLevelPerAutomateMap_.size() == 0 && bLastPerceived_ )
    {
        message().set_perceived( 0 );
        bLastPerceived_ = false;
        bMustSend = true;
    }
    else if( perceptionLevelPerAutomateMap_.size() > 0 && !bLastPerceived_ )
    {
        message().set_perceived( true ) ;
        bLastPerceived_ = true;
        bMustSend = true;
    }

    if( bMustSend )
    {
        nTimeLastUpdate_ = GetCurrentTimeStep();
        message().set_oid( nID_ );
        message().set_team( army_.GetID() );
        message().set_real_urban( object_.GetId() );
        message.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendFullState
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendFullState()
{
    client::UrbanKnowledgeUpdate message;
    message().set_oid( nID_ );
    message().set_team( army_.GetID() );
    message().set_real_urban( object_.GetId() );

    message().set_relevance( int( rRelevance_ * 100. ) );
    rLastRelevanceSent_ = rRelevance_;

    MsgsSimToClient::EnumUnitIdentificationLevel level( message().identification_level() );
    pCurrentPerceptionLevel_->Serialize( level );
    message().set_identification_level( level );

    message().set_progress( int( rProgressPercent_ * 100. ) );
    rLastProgressSent_ = rProgressPercent_;

    message().set_perceived( true );
    bLastPerceived_ = true;

    WriteMsgPerceptionSources( message() );

    message.Send( NET_Publisher_ABC::Publisher() );
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
    client::UrbanKnowledgeCreation message;
    message().set_oid( nID_ );
    message().set_team( army_.GetID() );
    message().set_real_urban( object_.GetId() );

    message.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::SendMsgDestruction
// Created: MGD 2009-11-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Urban::SendMsgDestruction() const
{
    client::UrbanKnowledgeDestruction message;
    message().set_oid( nID_ );
    message().set_team( army_.GetID() );

    message.Send( NET_Publisher_ABC::Publisher() );
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
// Name: DEC_Knowledge_Urban::GetLevel
// Created: MGD 2009-12-01
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_Urban::GetCurrentPerceptionLevel( const MIL_Agent_ABC& pion ) const
{
    CIT_PerceptionAgentSourceMap  itPerceptionLevel = perceptionLevelPerAgentMap_.find( &pion );
    if( itPerceptionLevel != perceptionLevelPerAgentMap_.end() )
        return *( itPerceptionLevel->second );
    else
        return PHY_PerceptionLevel::notSeen_;
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
// Name: DEC_Knowledge_Urban::GetProgress
// Created: MGD 2010-02-01
// -----------------------------------------------------------------------------
const geometry::Point2f DEC_Knowledge_Urban::GetBarycenter() const
{
    return object_.GetFootprint()->Barycenter();
}

const urban::TerrainObject_ABC& DEC_Knowledge_Urban::GetTerrainObjectKnown() const
{
    return object_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::IsValid
// Created: MGD 2009-12-04
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Urban::IsValid() const
{
    return true; //@TODO MGD
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Urban::GetTrafficability
// Created: LMT 2010-02-19
// -----------------------------------------------------------------------------
const float DEC_Knowledge_Urban::GetTrafficability() const
{
    return object_.RetrievePhysicalFeature< urban::Soil >()->GetTrafficability();
}