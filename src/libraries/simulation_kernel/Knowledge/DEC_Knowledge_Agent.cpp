// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_Agent.cpp $
// $Author: Nld $
// $Modtime: 9/05/05 14:51 $
// $Revision: 21 $
// $Workfile: DEC_Knowledge_Agent.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_AgentComposante.h"
#include "MIL_Time_ABC.h"
#include "CheckPoints/SerializationTools.h"
#include "Entities/MIL_Army.h"
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Posture/PHY_RoleInterface_Posture.h"
#include "Entities/Agents/Roles/Surrender/PHY_RoleInterface_Surrender.h"
#include "Entities/Agents/Roles/Transported/PHY_RoleInterface_Transported.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Agents/Units/Postures/PHY_Posture.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Effects/MIL_Effect_KillOfficers.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/Objects/MaterialAttribute.h"
#include "Entities/Objects/MIL_Object_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "Entities/Populations/MIL_Population.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_IDManager.h"
#include "Urban/MIL_UrbanObject_ABC.h"
#include "Urban/PHY_MaterialCompositionType.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_Agent )

double DEC_Knowledge_Agent::rMaxDangerosityDegradationByRelevance_        = 0.2; // 20%
double DEC_Knowledge_Agent::rMaxDangerosityDegradationByOpState_          = 0.2; // 20%
double DEC_Knowledge_Agent::rMaxDangerosityDegradationByNeutralizedState_ = 0.8; // 80%
bool DEC_Knowledge_Agent::detectDestroyedUnits_ = true;
MIL_IDManager DEC_Knowledge_Agent::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent::DEC_Knowledge_Agent( const MIL_KnowledgeGroup& group, MIL_Agent_ABC& agentKnown, double rRelevance )
    : pArmyKnowing_                  ( &group.GetArmy() )
    , pAgentKnown_                   ( &agentKnown )
    , nID_                           ( idManager_.GetId() )
    , groupId_                       ( group.GetId() )
    , pCurrentPerceptionLevel_       ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_           ( &PHY_PerceptionLevel::notSeen_ )
    , nTimeLastUpdate_               ( 0 )
    , rRelevance_                    ( rRelevance )
    , nTimeExtrapolationEnd_         ( -1 )
    , locked_                        ( 0 )
    , bValid_                        ( true )
    , bCreatedOnNetwork_             ( !pAgentKnown_->BelongsTo( group ) )
    , bRelevanceUpdated_             ( false )
    , bCurrentPerceptionLevelUpdated_( false )
    , bMaxPerceptionLevelUpdated_    ( false )
    , bCriticalIntelligenceUpdated_  ( false )
    , bPerceptionDistanceHacked_     ( false )
    , rLastRelevanceSent_            ( 0 )
    , maxLifetime_                   ( group.GetType().GetKnowledgeAgentMaxLifeTime() )
    , maxDistance_                   ( group.GetType().GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() )
    , extrapolationTime_             ( static_cast< int >( group.GetType().GetKnowledgeAgentExtrapolationTime() ) )
{
    if( bCreatedOnNetwork_ )
        SendMsgCreation();
    if( rRelevance_ < 0. || rRelevance_ > 1. )
        throw MASA_EXCEPTION( "Relevance: major-component not in [0..1]" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent constructor
// Created: LGY 2012-05-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent::DEC_Knowledge_Agent( const DEC_Knowledge_Agent& knowledge, const MIL_KnowledgeGroup& group )
    : pArmyKnowing_                  ( &group.GetArmy() )
    , pAgentKnown_                   ( knowledge.pAgentKnown_ )
    , nID_                           ( idManager_.GetId() )
    , groupId_                       ( group.GetId() )
    , pCurrentPerceptionLevel_       ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_           ( &PHY_PerceptionLevel::notSeen_ )
    , nTimeLastUpdate_               ( 0 )
    , rRelevance_                    ( 0 )
    , nTimeExtrapolationEnd_         ( -1 )
    , locked_                        ( 0 )
    , bValid_                        ( true )
    , bCreatedOnNetwork_             ( !pAgentKnown_->BelongsTo( group ) )
    , bRelevanceUpdated_             ( false )
    , bCurrentPerceptionLevelUpdated_( false )
    , bMaxPerceptionLevelUpdated_    ( false )
    , bCriticalIntelligenceUpdated_  ( false )
    , bPerceptionDistanceHacked_     ( false )
    , rLastRelevanceSent_            ( 0 )
    , maxLifetime_                   ( group.GetType().GetKnowledgeAgentMaxLifeTime() )
    , maxDistance_                   ( group.GetType().GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit() )
    , extrapolationTime_             ( static_cast< int >( group.GetType().GetKnowledgeAgentExtrapolationTime() ) )
{
    CopyFrom( knowledge );
    if( bCreatedOnNetwork_ )
    {
        SendMsgCreation();
        SendFullState();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent::DEC_Knowledge_Agent()
    : DEC_Knowledge_ABC()
    , nID_                           ( 0 )
    , groupId_                       ( 0 )
    , pArmyKnowing_                  ( 0 )
    , pAgentKnown_                   ( 0 )
    , nTimeLastUpdate_               ( 0 )
    , pCurrentPerceptionLevel_       ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_           ( &PHY_PerceptionLevel::notSeen_ )
    , rRelevance_                    ( 0 )
    , nTimeExtrapolationEnd_         ( -1 )
    , locked_                        ( 0 )
    , bValid_                        ( true )
    , bCreatedOnNetwork_             ( true )
    , bRelevanceUpdated_             ( true )
    , bCurrentPerceptionLevelUpdated_( true )
    , bMaxPerceptionLevelUpdated_    ( true )
    , bCriticalIntelligenceUpdated_  ( false )
    , bPerceptionDistanceHacked_     ( false )
    , rLastRelevanceSent_            ( 0 )
    , maxLifetime_                   ( 0 )
    , maxDistance_                   ( 0 )
    , extrapolationTime_             ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent::~DEC_Knowledge_Agent()
{
    if( bCreatedOnNetwork_ )
        SendMsgDestruction();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file >> nID_;
    file >> groupId_;
    file >> pArmyKnowing_;
    file >> pAgentKnown_;
    file >> dataDetection_;
    file >> dataRecognition_;
    file >> dataIdentification_;
    file >> nTimeLastUpdate_;
    idManager_.GetId( nID_, true );
    file >> pCurrentPerceptionLevel_;
    file >> pPreviousPerceptionLevel_;
    file >> pMaxPerceptionLevel_;
    file >> perceptionLevelPerAutomateMap_;
    file >> previousPerceptionLevelPerAutomateMap_;
    file >> rRelevance_;
    file >> bCreatedOnNetwork_;
    file >> bRelevanceUpdated_;
    file >> bCurrentPerceptionLevelUpdated_;
    file >> bMaxPerceptionLevelUpdated_;
    file >> nTimeExtrapolationEnd_;
    file >> criticalIntelligence_;
    file >> locked_;
    file >> bValid_;
    file >> bCriticalIntelligenceUpdated_;
    file >> bPerceptionDistanceHacked_;
    file >> maxLifetime_;
    file >> maxDistance_;
    file >> extrapolationTime_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file << nID_;
    file << groupId_;
    file << pArmyKnowing_;
    file << pAgentKnown_;
    file << dataDetection_;
    file << dataRecognition_;
    file << dataIdentification_;
    file << nTimeLastUpdate_;
    file << pCurrentPerceptionLevel_;
    file << pPreviousPerceptionLevel_;
    file << pMaxPerceptionLevel_;
    file << perceptionLevelPerAutomateMap_;
    file << previousPerceptionLevelPerAutomateMap_;
    file << rRelevance_;
    file << bCreatedOnNetwork_;
    file << bRelevanceUpdated_;
    file << bCurrentPerceptionLevelUpdated_;
    file << bMaxPerceptionLevelUpdated_;
    file << nTimeExtrapolationEnd_;
    file << criticalIntelligence_;
    file << locked_;
    file << bValid_;
    file << bCriticalIntelligenceUpdated_;
    file << bPerceptionDistanceHacked_;
    file << maxLifetime_;
    file << maxDistance_;
    file << extrapolationTime_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::WriteKnowledges
// Created: NPT 2012-08-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::WriteKnowledges( xml::xostream& xos ) const
{
    xos << xml::start( "agent-knowledge" )
            << xml::attribute( "id", GetID() );
    if( pAgentKnown_ )
    {
        xos << xml::attribute( "real-unit", pAgentKnown_->GetID() )
            << xml::attribute( "real-unit-type", pAgentKnown_->GetType().GetID() );
    }
    xos     << xml::attribute( "max-level", GetMaxPerceptionLevel().GetID() )
            << xml::attribute( "relevance", GetRelevance() );
            dataDetection_.WriteKnowledges( xos );
            dataRecognition_.WriteKnowledges( xos );
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::Prepare()
{
    bRelevanceUpdated_  = false;
    bMaxPerceptionLevelUpdated_ = false;
    bCurrentPerceptionLevelUpdated_ = false;
    if( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ )
        bCurrentPerceptionLevelUpdated_ = true;
    pPreviousPerceptionLevel_ = pCurrentPerceptionLevel_;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::notSeen_;
    previousPerceptionLevelPerAutomateMap_.clear();
    perceptionLevelPerAutomateMap_.swap( previousPerceptionLevelPerAutomateMap_ );
    dataDetection_.Prepare();
    dataRecognition_.Prepare();
    dataIdentification_.Prepare();
    if( nTimeExtrapolationEnd_ > 0 )
        --nTimeExtrapolationEnd_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Extrapolate
// Created: JVT 2004-11-29
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::Extrapolate()
{
    assert( pAgentKnown_ );
    if( nTimeExtrapolationEnd_ > 0 || locked_ )
    {
        ChangeRelevance( 1. );
        dataDetection_.Extrapolate( *pAgentKnown_ );
        dataRecognition_.Extrapolate( *pAgentKnown_ );
        dataIdentification_.Extrapolate( *pAgentKnown_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent PerceptionSources
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::UpdatePerceptionSources( const DEC_Knowledge_AgentPerception& perception )
{
    if( perception.GetCurrentPerceptionLevel() == PHY_PerceptionLevel::notSeen_ )
        return;
    const MIL_Agent_ABC& pionSource = perception.GetAgentPerceiving();
    const MIL_Automate& automateSource = pionSource.GetAutomate();
    // On ne garde que les sources provenant d'autres knowledge groupes
    assert( pAgentKnown_ );
    if( pAgentKnown_->BelongsTo( *automateSource.GetKnowledgeGroup() ) )
        return;
    auto it = perceptionLevelPerAutomateMap_.find( &automateSource );
    if( it == perceptionLevelPerAutomateMap_.end() )
        perceptionLevelPerAutomateMap_.insert( std::make_pair( &automateSource, &perception.GetCurrentPerceptionLevel() ) );
    else
        it->second = &std::max( *it->second, perception.GetCurrentPerceptionLevel() );
    auto it2 = perceptionLevelPerAgentMap_.find( &pionSource );
    if( it2 == perceptionLevelPerAgentMap_.end() )
        perceptionLevelPerAgentMap_.insert( std::make_pair( &pionSource, &perception.GetCurrentPerceptionLevel() ) );
    else
        it2->second = &std::max( *it2->second, perception.GetCurrentPerceptionLevel() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Update
// Created: NLD 2004-05-03
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::Update( const DEC_Knowledge_AgentPerception& perception, int currentTimeStep )
{
    nTimeLastUpdate_ = currentTimeStep;
    ChangeRelevance( 1. );
    if( perception.GetCurrentPerceptionLevel() > *pCurrentPerceptionLevel_ )
    {
        pCurrentPerceptionLevel_ = &perception.GetCurrentPerceptionLevel();
        bCurrentPerceptionLevelUpdated_ = ( *pCurrentPerceptionLevel_ != *pPreviousPerceptionLevel_ );
    }
    if( perception.GetMaxPerceptionLevel() > *pMaxPerceptionLevel_ )
    {
        pMaxPerceptionLevel_= &perception.GetMaxPerceptionLevel();
        bMaxPerceptionLevelUpdated_ = true;
        bCurrentPerceptionLevelUpdated_ = true;
    }
    dataDetection_.Update( perception.GetDetectionData() );
    dataRecognition_.Update( perception.GetRecognitionData() );
    dataIdentification_.Update( perception.GetIdentificationData() );
    UpdatePerceptionSources( perception );
    const MIL_Agent_ABC& agentPerceived = perception.GetAgentPerceived();
    if( bMaxPerceptionLevelUpdated_ && !IsDead() && &perception.GetAgentPerceiving() != &agentPerceived )
    {
        if( perception.GetMaxPerceptionLevel() == PHY_PerceptionLevel::detected_ )
            MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_DetectedUnit );
        else if( const MIL_Army_ABC* army = GetArmy() )
        {
            const PHY_PerceptionLevel& maxPerception = perception.GetMaxPerceptionLevel();
            if( pArmyKnowing_->IsAFriend( *army ) == eTristate_True )
            {
                if( maxPerception == PHY_PerceptionLevel::recognized_ )
                    MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_FriendUnitRecognized );
                else if( maxPerception == PHY_PerceptionLevel::identified_ )
                    MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_FriendUnitIdentified );
            }
            else if( pArmyKnowing_->IsAnEnemy( *army ) == eTristate_True )
            {
                if( agentPerceived.GetRole< surrender::PHY_RoleInterface_Surrender >().IsSurrendered() )
                {
                    if( maxPerception == PHY_PerceptionLevel::recognized_ )
                        MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_SurrenderedUnitRecognized );
                    else if( maxPerception == PHY_PerceptionLevel::identified_ )
                        MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_SurrenderedUnitIdentified );
                }
                else
                {
                    if( maxPerception == PHY_PerceptionLevel::recognized_ )
                        MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_EnemyUnitRecognized );
                    else if( maxPerception == PHY_PerceptionLevel::identified_ )
                        MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_EnemyUnitIdentified );
                }
            }
            else if( pArmyKnowing_->IsNeutral( *army ) == eTristate_True )
            {
                if( maxPerception == PHY_PerceptionLevel::recognized_ )
                    MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_NeutralUnitRecognized );
                else if( maxPerception == PHY_PerceptionLevel::identified_ )
                    MIL_Report::PostEvent( perception.GetAgentPerceiving(), report::eRC_NeutralUnitIdentified );
            }
        }
    }
    nTimeExtrapolationEnd_ = extrapolationTime_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Update
// Created: NLD 2004-11-08
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::Update( const DEC_Knowledge_Agent& knowledge, int currentTimeStep )
{
    nTimeLastUpdate_ = currentTimeStep;
    dataDetection_.Update( knowledge.dataDetection_ );
    dataRecognition_.Update( knowledge.dataRecognition_ );
    dataIdentification_.Update( knowledge.dataIdentification_ );
    if( criticalIntelligence_.empty() && !knowledge.GetCriticalIntelligence().empty() )
    {
        const std::string& intelligence = knowledge.GetCriticalIntelligence();
        if( intelligence != criticalIntelligence_ )
        {
            criticalIntelligence_ = intelligence;
            bCriticalIntelligenceUpdated_ = true;
        }
    }
    ChangeRelevance( std::max( rRelevance_, knowledge.GetRelevance() ) );
    const PHY_PerceptionLevel* newLevel = &std::max( *pMaxPerceptionLevel_, knowledge.GetMaxPerceptionLevel() );
    if( newLevel != pMaxPerceptionLevel_)
    {
        pMaxPerceptionLevel_ = newLevel;
        bMaxPerceptionLevelUpdated_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::UpdateFromCrowdPerception
// Created: JSR 2013-07-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::UpdateFromCrowdPerception( const MIL_Population& crowd, int currentTimeStep )
{
    nTimeLastUpdate_ = currentTimeStep;
    ChangeRelevance( 1. );
    if( PHY_PerceptionLevel::recognized_ > *pCurrentPerceptionLevel_ )
    {
        pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::recognized_;
        bCurrentPerceptionLevelUpdated_ = ( *pCurrentPerceptionLevel_ != *pPreviousPerceptionLevel_ );
    }
    if( PHY_PerceptionLevel::recognized_ > *pMaxPerceptionLevel_ )
    {
        pMaxPerceptionLevel_= &PHY_PerceptionLevel::recognized_;
        bMaxPerceptionLevelUpdated_ = true;
        bCurrentPerceptionLevelUpdated_ = true;
    }
    if( bMaxPerceptionLevelUpdated_ && pCurrentPerceptionLevel_ == &PHY_PerceptionLevel::recognized_ && pAgentKnown_ && !IsDead() )
    {
        if( pArmyKnowing_->IsAFriend( pAgentKnown_->GetArmy() ) == eTristate_True )
            MIL_Report::PostEvent( crowd, report::eRC_FriendUnitRecognized );
        else if( pArmyKnowing_->IsAnEnemy( pAgentKnown_->GetArmy() ) == eTristate_True )
            MIL_Report::PostEvent( crowd, report::eRC_EnemyUnitRecognized );
        else if( pArmyKnowing_->IsNeutral( pAgentKnown_->GetArmy() ) == eTristate_True )
            MIL_Report::PostEvent( crowd, report::eRC_NeutralUnitRecognized );
    }
    nTimeExtrapolationEnd_ = extrapolationTime_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::ChangeRelevance
// Created: NLD 2005-08-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::ChangeRelevance( double rNewRelevance )
{
    if( rRelevance_ == rNewRelevance )
        return;
    static const double rDeltaForNetwork = 0.05;
    if( fabs( rLastRelevanceSent_ - rNewRelevance ) > rDeltaForNetwork || rNewRelevance == 0. || rNewRelevance == 1. )
        bRelevanceUpdated_ = true;
    rRelevance_ = rNewRelevance;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::UpdateRelevance
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::UpdateRelevance( int currentTimeStep )
{
    if( bPerceptionDistanceHacked_ )
    {
        dataDetection_.Extrapolate( *pAgentKnown_ );
        ChangeRelevance( 1. );
        nTimeLastUpdate_ = currentTimeStep;
        return;
    }

    // L'agent est percu
    if( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ )
    {
        assert( rRelevance_ == 1. );
        return;
    }
    assert( rRelevance_ >= 0. && rRelevance_ <= 1. );
    // La connaissance est invalidée quand l'unité réelle a fait un magic move
    // ou quand on voit la position à laquelle l'unité devrait se trouver <==== ou pas
    assert( pAgentKnown_ );
    if( pAgentKnown_->GetRole< PHY_RoleInterface_Location >().HasDoneMagicMove() )
    {
        ChangeRelevance( 0. );
        return;
    }    
    auto transportedRole = pAgentKnown_->RetrieveRole< transport::PHY_RoleInterface_Transported >();
    const MIL_Agent_ABC* transporter = transportedRole ? transportedRole->GetTransporter() : 0;
    if( transporter && transporter->GetKnowledgeGroup()->GetId() == groupId_ )
    {
        ChangeRelevance( 1. );
        nTimeLastUpdate_ = currentTimeStep;
        return;
    }
    // Degradation : effacement au bout de X minutes
    const double rTimeRelevanceDegradation = ( currentTimeStep - nTimeLastUpdate_ ) / maxLifetime_;
    // Degradation : effacement quand l'unité réelle et l'unité connnue sont distantes de X metres
    const double rDistanceBtwKnowledgeAndKnown = dataDetection_.GetPosition().Distance( pAgentKnown_->GetRole< PHY_RoleInterface_Location >().GetPosition() );
    const double rDistRelevanceDegradation     = bPerceptionDistanceHacked_? 0.0 : rDistanceBtwKnowledgeAndKnown / maxDistance_;

    ChangeRelevance( std::max( 0., rRelevance_ - rTimeRelevanceDegradation - rDistRelevanceDegradation ) );
    nTimeLastUpdate_ = currentTimeStep;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::WriteMsgPerceptionSources
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::WriteMsgPerceptionSources( sword::UnitKnowledgeUpdate& asnMsg ) const
{
    asnMsg.mutable_perceptions();
    for( auto it = perceptionLevelPerAutomateMap_.begin(); it != perceptionLevelPerAutomateMap_.end(); ++it )
    {
        sword::AutomatPerception& perception = *asnMsg.mutable_perceptions()->add_elem();
        perception.mutable_automat()->set_id( it->first->GetID() );
        sword::UnitIdentification::Level level;
        it->second->Serialize( level );
        perception.set_identification_level( level );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::SendChangedState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::SendChangedState()
{
    const bool bPerceptionPerAutomateUpdated = ( previousPerceptionLevelPerAutomateMap_ != perceptionLevelPerAutomateMap_ );
    if( !( bCurrentPerceptionLevelUpdated_ || bMaxPerceptionLevelUpdated_
         || bRelevanceUpdated_ || bPerceptionPerAutomateUpdated || bCriticalIntelligenceUpdated_
         || dataDetection_.HasChanged() || dataRecognition_.HasChanged() || dataIdentification_.HasChanged() ) )
        return;
    client::UnitKnowledgeUpdate asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_knowledge_group()->set_id( groupId_ );
    if( bRelevanceUpdated_ )
    {
        asnMsg().set_pertinence( (int)( rRelevance_ * 100. ) );
        rLastRelevanceSent_ = rRelevance_;
    }
    if( bCurrentPerceptionLevelUpdated_ )
    {
        sword::UnitIdentification::Level level( asnMsg().identification_level() );
        pCurrentPerceptionLevel_->Serialize( level );
        asnMsg().set_identification_level( level );
    }
    if( bMaxPerceptionLevelUpdated_ )
    {
        sword::UnitIdentification::Level level( asnMsg().max_identification_level() );
        pMaxPerceptionLevel_->Serialize( level );
        asnMsg().set_max_identification_level( level );
    }
    if( bPerceptionPerAutomateUpdated )
        WriteMsgPerceptionSources( asnMsg() );
    if( bCriticalIntelligenceUpdated_ )
    {
        asnMsg().set_critical_intelligence( criticalIntelligence_ );
        bCriticalIntelligenceUpdated_ = false;
    }
    if( pAgentKnown_ )
        asnMsg().set_posture( pAgentKnown_->GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture().GetAsnID() );
    dataDetection_.SendChangedState( asnMsg() );
    dataRecognition_.SendChangedState( asnMsg() );
    dataIdentification_.SendChangedState( asnMsg() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::SendFullState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::SendFullState()
{
    client::UnitKnowledgeUpdate asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_knowledge_group()->set_id( groupId_ );
    asnMsg().set_pertinence( static_cast< int >( rRelevance_ * 100. ) );
    rLastRelevanceSent_ = rRelevance_;
    sword::UnitIdentification::Level level( asnMsg().identification_level() );
    pCurrentPerceptionLevel_->Serialize( level );
    asnMsg().set_identification_level( level );
    sword::UnitIdentification::Level maxlevel( asnMsg().max_identification_level() );
    pMaxPerceptionLevel_->Serialize( maxlevel );
    asnMsg().set_max_identification_level( maxlevel );
    if( !perceptionLevelPerAutomateMap_.empty() )
        WriteMsgPerceptionSources( asnMsg() );
    asnMsg().set_critical_intelligence( criticalIntelligence_ );
    dataDetection_.SendFullState( asnMsg() );
    dataRecognition_ .SendFullState( asnMsg() );
    dataIdentification_.SendFullState( asnMsg() );
    if( pAgentKnown_ )
        asnMsg().set_posture( pAgentKnown_->GetRole< PHY_RoleInterface_Posture >().GetCurrentPosture().GetAsnID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::UpdateOnNetwork
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::UpdateOnNetwork()
{
    assert( pAgentKnown_ );
    if( pAgentKnown_->GetKnowledgeGroup()->GetId() == groupId_ )
    {
        if( bCreatedOnNetwork_ )
        {
            SendMsgDestruction();
            bCreatedOnNetwork_ = false;
        }
        return;
    }
    if( !bCreatedOnNetwork_ )
    {
        SendMsgCreation();
        bCreatedOnNetwork_ = true;
        SendFullState();
        return;
    }
    SendChangedState();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::SendStateToNewClient()
{
    if( bCreatedOnNetwork_ )
    {
        SendMsgCreation();
        SendFullState();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::SendMsgCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::SendMsgCreation() const
{
    assert( pAgentKnown_ );
    client::UnitKnowledgeCreation asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_knowledge_group()->set_id( groupId_ );
    asnMsg().mutable_unit()->set_id( pAgentKnown_->GetID() );
    asnMsg().mutable_type()->set_id( pAgentKnown_->GetType().GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::SendMsgDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::SendMsgDestruction() const
{
    client::UnitKnowledgeDestruction asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_knowledge_group()->set_id( groupId_ );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// ----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::DegradeDangerosity
// Created: NLD 2004-05-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::DegradeDangerosity( double& rDangerosity ) const
{
    // Pertinence
    rDangerosity *= ( 1 - ( (-rMaxDangerosityDegradationByRelevance_ * rRelevance_) + rMaxDangerosityDegradationByRelevance_ ) );
    // Etat opérationel
    const double rOpState = dataRecognition_.GetOperationalState();
    if( rOpState == 0. ) // L'unité est morte
        rDangerosity = 0;
    else
        rDangerosity *= ( 1 - ( (-rMaxDangerosityDegradationByOpState_ * rOpState ) + rMaxDangerosityDegradationByOpState_) );
    // Source is neutralized
    assert( pAgentKnown_ );
    if( pAgentKnown_->IsNeutralized() )
        rDangerosity *= 1 - rMaxDangerosityDegradationByNeutralizedState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetDangerosity
// Created: NLD 2004-04-02
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetDangerosity( const MIL_Agent_ABC& target, bool bUseAmmo, const PHY_MaterialCompositionType* material ) const
{
    if( *pMaxPerceptionLevel_ < PHY_PerceptionLevel::recognized_
        ||  IsAFriend( target.GetArmy() ) == eTristate_True
        ||  dataDetection_.IsSurrendered() )
        return 0.;
    const PHY_RoleInterface_Location& targetLocation = target.GetRole< PHY_RoleInterface_Location >();
    const MT_Vector2D& position2d = targetLocation.GetPosition();
    if( dataDetection_.GetPosition().SquareDistance( position2d ) > dataRecognition_.GetMaxSquareRange() )
        return 0;
    // Target is dead ....
    const PHY_ComposantePion* pTargetMajorComposante = target.GetRole< PHY_RolePion_Composantes >().GetMajorComposante();
    if( !pTargetMajorComposante )
        return 0.;
    const MT_Vector3D vTargetPosition( position2d.rX_, position2d.rY_, targetLocation.GetAltitude() );
    const PHY_ComposanteType_ABC& targetType = pTargetMajorComposante->GetType();
    return ComputeDangerosity( vTargetPosition, targetType, bUseAmmo, material );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetDangerosity
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetDangerosity( const DEC_Knowledge_Agent& target, bool bUseAmmo, const PHY_MaterialCompositionType* material ) const
{
    if( *pMaxPerceptionLevel_ < PHY_PerceptionLevel::recognized_ )
        return 0.;
    const MT_Vector2D& position2d = target.GetPosition();
    if( dataDetection_.GetPosition().SquareDistance( position2d ) > dataRecognition_.GetMaxSquareRange() )
        return 0;
    // Target is dead ....
    const DEC_Knowledge_AgentComposante* pTargetMajorComposante = target.GetMajorComposante();
    if( !pTargetMajorComposante )
        return 0.;
    // Fight score
    const MT_Vector3D vTargetPosition( position2d.rX_, position2d.rY_, target.GetAltitude() );
    const PHY_ComposanteType_ABC& targetType = pTargetMajorComposante->GetType();
    return ComputeDangerosity( vTargetPosition, targetType, bUseAmmo, material );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetDangerosity
// Created: JSR 2011-09-09
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::ComputeDangerosity( const MT_Vector3D& vTargetPosition, const PHY_ComposanteType_ABC& targetMajorComposante, bool bUseAmmo, const PHY_MaterialCompositionType* material ) const
{
    double rDangerosity = 0.;
    // Fight score
    const MT_Vector2D& position2d = dataDetection_.GetPosition();
    const MT_Vector3D vDataPosition( position2d.rX_, position2d.rY_, dataDetection_.GetAltitude() );
    const double rDistBtwSourceAndTarget = vTargetPosition.Distance( vDataPosition );
    const T_KnowledgeComposanteVector& composantes = dataRecognition_.GetComposantes();
    if ( bValid_ == false )
        return 0.;
    for( auto itComposante = composantes.begin(); itComposante != composantes.end(); ++itComposante )
        if( itComposante->IsMajor() )
            rDangerosity = std::max( rDangerosity, itComposante->GetDangerosity( *pAgentKnown_, targetMajorComposante, rDistBtwSourceAndTarget, bUseAmmo, material ) );
    DegradeDangerosity( rDangerosity );
    return rDangerosity;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetMaxRangeToFireOn
// Created: NLD 2004-04-13
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetMaxRangeToFireOn( const MIL_Agent_ABC& target, double rWantedPH ) const
{
    // Get back the most dangerous composante type of the target (from our point of view ...)
    const PHY_ComposantePion* pTargetComposante = target.GetRole< PHY_RolePion_Composantes >().GetMajorComposante();
    if( !pTargetComposante )
        return 0.;
    double rRange = 0;
    const T_KnowledgeComposanteVector& composantes = dataRecognition_.GetComposantes();
    if ( bValid_ == false )
        return 0;
    for( auto itComposante = composantes.begin(); itComposante != composantes.end(); ++itComposante )
    {
        const DEC_Knowledge_AgentComposante& composante = *itComposante;
        rRange = std::max( rRange, composante.GetMaxRangeToFireOn( *pAgentKnown_, *pTargetComposante, rWantedPH ) );
    }
    return rRange;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetComposantes
// Created: MCO 2012-05-29
// -----------------------------------------------------------------------------
const T_KnowledgeComposanteVector& DEC_Knowledge_Agent::GetComposantes() const
{
    return dataRecognition_.GetComposantes();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetMajorComposante
// Created: NLD 2004-04-15
// -----------------------------------------------------------------------------
const DEC_Knowledge_AgentComposante* DEC_Knowledge_Agent::GetMajorComposante() const
{
    const DEC_Knowledge_AgentComposante* pMajorComposante = 0;
    unsigned int nMajorScore = 0;
    const T_KnowledgeComposanteVector& composantes = dataRecognition_.GetComposantes();
    for( auto it = composantes.begin(); it != composantes.end(); ++it )
    {
        const DEC_Knowledge_AgentComposante& composante = *it;
        if( composante.GetMajorScore() >= nMajorScore )
        {
            pMajorComposante = &composante;
            nMajorScore = composante.GetMajorScore();
        }
    }
    return pMajorComposante;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetMaterialComposantesAttritionLevel
// Created: DDA 2010-04-23
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetMaterialComposantesAttritionLevel( MIL_UrbanObject_ABC* pUrbanBlock ) const
{
    if( pUrbanBlock )
        if( const MaterialAttribute* attribute = pUrbanBlock->RetrieveAttribute< MaterialAttribute >() )
        {
            if( GetMaxPerceptionLevel() == PHY_PerceptionLevel::identified_ )
                return GetAgentKnown().GetRole< PHY_RolePion_Composantes >().GetAttritionIndexComposante( attribute->GetMaterial() );
            else if( ( GetMaxPerceptionLevel() == PHY_PerceptionLevel::recognized_ ) || ( GetMaxPerceptionLevel() == PHY_PerceptionLevel::detected_ ) )
                return PHY_DotationCategory::FindUrbanAttritionScore( attribute->GetMaterial() );
            else
                return -1;
        }
    return -1;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Lock
// Created: NLD 2005-04-21
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::Lock()
{
    ++locked_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Unlock
// Created: NLD 2005-04-21
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::Unlock()
{
    --locked_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::KillOfficers
// Created: SBO 2005-12-21
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::KillOfficers()
{
    assert( pAgentKnown_ );
    MIL_Effect_KillOfficers* pEffect = new MIL_Effect_KillOfficers( *pAgentKnown_ );
    MIL_EffectManager::GetEffectManager().Register( *pEffect );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetSignificantVolume
// Created: NLD 2004-08-30
// -----------------------------------------------------------------------------
const PHY_Volume* DEC_Knowledge_Agent::GetSignificantVolume( const PHY_SensorTypeAgent& sensorType ) const
{
    const PHY_Volume* pSignificantVolume = 0;
    double rSignificantVolumeFactor = 0;
    const auto& volumes = dataDetection_.GetVisibleVolumes();
    for( auto it = volumes.begin(); it != volumes.end(); ++it )
    {
        double rVolumeFactor = sensorType.GetVolumeFactor( **it );
        if( rVolumeFactor > rSignificantVolumeFactor )
        {
            pSignificantVolume = &**it;
            rSignificantVolumeFactor = rVolumeFactor;
        }
    }
    return pSignificantVolume;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsRefugee
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsRefugee() const
{
    assert( pAgentKnown_ );
    return pAgentKnown_->GetType().IsRefugee();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsMilitia
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsMilitia() const
{
    assert( pAgentKnown_ );
    return pAgentKnown_->GetType().IsMilitia();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsCivilian
// Created: LGY 2012-12-20
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsCivilian() const
{
    assert( pAgentKnown_ );
    return pAgentKnown_->IsCivilian();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsTerrorist
// Created: NLD 2006-02-23
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsTerrorist() const
{
    assert( pAgentKnown_ );
    return pAgentKnown_->GetType().IsTerrorist();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsAnEnemy
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
E_Tristate DEC_Knowledge_Agent::IsAnEnemy( const MIL_Army_ABC& army ) const
{
    const MIL_Army_ABC* pArmy = GetArmy();
    if( !pArmy )
        return eTristate_DontKnow;
    return army.IsAnEnemy( *pArmy );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsAFriend
// Created: LDC 2009-03-06
// -----------------------------------------------------------------------------
E_Tristate DEC_Knowledge_Agent::IsAFriend( const MIL_Army_ABC& army ) const
{
    const MIL_Army_ABC* pArmy = GetArmy();
    if( !pArmy )
        return eTristate_DontKnow;
    return army.IsAFriend( *pArmy );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetID
// Created: NLD 2004-03-24
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_Agent::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetAgentPerceived
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
MIL_Agent_ABC& DEC_Knowledge_Agent::GetAgentKnown() const
{
    assert( pAgentKnown_ );
    return *pAgentKnown_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetPosition
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_Agent::GetPosition() const
{
    return dataDetection_.GetPosition();
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_Knowledge_Agent::GetPositionPtr
// Created: JSR 2012-09-14
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_Knowledge_Agent::GetPositionPtr() const
{
    return dataDetection_.GetPositionPtr();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetPosition
// Created: MGD 2010-05-05
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsInUrbanBlock( const MIL_Object_ABC& urban ) const
{
    return urban.GetLocalisation().IsInside( GetPosition(), 0 );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetDirection
// Created: JVT 2005-02-17
// -----------------------------------------------------ivi------------------------
const MT_Vector2D& DEC_Knowledge_Agent::GetDirection() const
{
    return dataDetection_.GetDirection();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetAltitude
// Created: NLD 2004-06-07
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetAltitude() const
{
    return dataDetection_.GetAltitude();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetSpeed
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetSpeed() const
{
    return dataDetection_.GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetRelevance
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetRelevance() const
{
    return rRelevance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetPostureCompletionPercentage
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetPostureCompletionPercentage() const
{
    return dataDetection_.GetPostureCompletionPercentage();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetOldPosture
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
const PHY_Posture& DEC_Knowledge_Agent::GetLastPosture() const
{
    return dataDetection_.GetLastPosture();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetCurrentPosture
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
const PHY_Posture& DEC_Knowledge_Agent::GetCurrentPosture() const
{
    return dataDetection_.GetCurrentPosture();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetPopulationDensity
// Created: LDC 2011-04-07
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetPopulationDensity() const
{
    return dataDetection_.GetPopulationDensity();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetMaxPerceptionLevel
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_Agent::GetMaxPerceptionLevel() const
{
    assert( pMaxPerceptionLevel_ );
    return *pMaxPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetCurrentPerceptionLevel
// Created: NLD 2004-04-28
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_Agent::GetCurrentPerceptionLevel() const
{
    assert( pCurrentPerceptionLevel_ );
    return *pCurrentPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetCurrentPerceptionLevel
// Created: SLG 2010-01-27
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel& DEC_Knowledge_Agent::GetCurrentPerceptionLevel( const MIL_Agent_ABC& pion ) const
{
    auto it = perceptionLevelPerAgentMap_.find( &pion );
    if( it != perceptionLevelPerAgentMap_.end() )
        return *it->second;
    return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetOperationalState
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetOperationalState() const
{
    return dataRecognition_.GetOperationalState();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetMajorOperationalState
// Created: NLD 2005-11-30
// -----------------------------------------------------------------------------
double DEC_Knowledge_Agent::GetMajorOperationalState() const
{
    return dataRecognition_.GetMajorOperationalState();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsDead
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsDead() const
{
    return dataDetection_.IsDead();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsWounded
// Created: LGY 2012-12-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsWounded() const
{
    return dataDetection_.IsWounded();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsInCrowd
// Created: DDA 2011-05-13
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsInCrowd() const
{
    return bValid_ ? pAgentKnown_->GetKnowledge().HasCollision() : false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsSurrendered
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsSurrendered() const
{
    return dataDetection_.IsSurrendered();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsPrisoner
// Created: NLD 2005-03-04
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsPrisoner() const
{
    return dataDetection_.IsPrisoner();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsRefugeeManaged
// Created: NLD 2005-03-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsRefugeeManaged() const
{
    return dataDetection_.IsRefugeeManaged();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetArmy
// Created: NLD 2004-04-06
// -----------------------------------------------------------------------------
const MIL_Army_ABC* DEC_Knowledge_Agent::GetArmy() const
{
    return dataRecognition_.GetArmy();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetNatureAtlas
// Created: NLD 2004-03-31
// Modified: JVT 2004-12-09
// -----------------------------------------------------------------------------
const PHY_NatureAtlas& DEC_Knowledge_Agent::GetNatureAtlas() const
{
    return dataRecognition_.GetNatureAtlas();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Clean
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::Clean() const
{
    return GetRelevance() <= 0.;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsHuman
// Created: NLD 2007-04-19
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsHuman() const
{
    return dataRecognition_.IsHuman();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Invalidate
// Created: LDC 2009-10-20
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::Invalidate()
{
    bValid_ = false;
    if( bCreatedOnNetwork_ )
    {
        SendMsgDestruction();
        bCreatedOnNetwork_ = false;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsValid
// Created: LDC 2009-10-20
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsValid() const
{
    return bValid_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetCriticalIntelligence
// Created: ABR 2011-03-16
// -----------------------------------------------------------------------------
const std::string& DEC_Knowledge_Agent::GetCriticalIntelligence() const
{
    return criticalIntelligence_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::SetCriticalIntelligenceFromAgentKnown
// Created: ABR 2011-03-16
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::SetCriticalIntelligenceFromAgentKnown()
{
    if ( bValid_ == false )
        return;
    criticalIntelligence_ = pAgentKnown_->GetCriticalIntelligence();
    bCriticalIntelligenceUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::CopyFrom
// Created: LDC 2010-04-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::CopyFrom( const DEC_Knowledge_Agent& agent )
{
    dataDetection_.Update( agent.dataDetection_ );
    dataRecognition_.Update( agent.dataRecognition_ );
    dataIdentification_.Update( agent.dataIdentification_ );
    nTimeLastUpdate_ = agent.nTimeLastUpdate_;
    pCurrentPerceptionLevel_ = agent.pCurrentPerceptionLevel_;
    pPreviousPerceptionLevel_ = agent.pPreviousPerceptionLevel_;
    pMaxPerceptionLevel_ = agent.pMaxPerceptionLevel_;
    rRelevance_ = agent.rRelevance_;
    nTimeExtrapolationEnd_ = agent.nTimeExtrapolationEnd_;
    locked_ = agent.locked_;
    bValid_ = agent.bValid_;
    criticalIntelligence_ = agent.criticalIntelligence_;
    bRelevanceUpdated_ = true;
    bMaxPerceptionLevelUpdated_ = true;
    bCurrentPerceptionLevelUpdated_ = true;
    bCriticalIntelligenceUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::HackPerceptionLevel
// Created: MMC 2011-06-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::HackPerceptionLevel( const PHY_PerceptionLevel* pPerceptionLevel )
{
    if( *pPerceptionLevel > *pCurrentPerceptionLevel_ )
    {
        ChangeRelevance( 1. );
        bPerceptionDistanceHacked_ = true;
        pCurrentPerceptionLevel_ = pPerceptionLevel;
        bCurrentPerceptionLevelUpdated_ = ( *pCurrentPerceptionLevel_ != *pPreviousPerceptionLevel_ );
        nTimeLastUpdate_ = MIL_Time_ABC::GetTime().GetCurrentTimeStep();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsPerceptionDistanceHacked
// Created: MMC 2011-06-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsPerceptionDistanceHacked() const
{
    return bPerceptionDistanceHacked_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::IsTransported
// Created: DDA 2012-01-10
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsTransported() const
{
    return bValid_ ? pAgentKnown_->GetRole< transport::PHY_RoleInterface_Transported >().IsTransported() : false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetGroupID
// Created: MCO 2012-08-22
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_Agent::GetGroupID() const
{
    return groupId_;
}
