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
#include "Entities/Agents/MIL_Agent_ABC.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/MIL_AgentType_ABC.h"
#include "Entities/Agents/Roles/Location/PHY_RoleInterface_Location.h"
#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Units/Sensors/PHY_SensorTypeAgent.h"
#include "Entities/Agents/Units/Dotations/PHY_DotationCategory.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Effects/MIL_Effect_KillOfficers.h"
#include "Entities/Effects/MIL_EffectManager.h"
#include "Entities/MIL_Army.h"
#include "Knowledge/MIL_KnowledgeGroup.h"
#include "Knowledge/DEC_Knowledge_Urban.h"
#include "Network/NET_ASN_Tools.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include "Tools/MIL_IDManager.h"
#include "UrbanType.h"
#include <urban/StaticModel.h>
#include <urban/Architecture.h>
#include <urban/TerrainObject_ABC.h>
#include <urban/MaterialCompositionType.h>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_Agent )

MT_Float DEC_Knowledge_Agent::rMaxDangerosityDegradationByRelevance_        = 0.2; // 20%
MT_Float DEC_Knowledge_Agent::rMaxDangerosityDegradationByOpState_          = 0.2; // 20%
MT_Float DEC_Knowledge_Agent::rMaxDangerosityDegradationByNeutralizedState_ = 0.8; // 80%
MIL_IDManager DEC_Knowledge_Agent::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent::DEC_Knowledge_Agent( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Agent_ABC& agentKnown, MT_Float rRelevance )
    : DEC_Knowledge_ABC()
    , pKnowledgeGroup_               ( &knowledgeGroup )
    , pAgentKnown_                   ( &agentKnown )
    , nID_                           ( idManager_.GetFreeId() )
    , pCurrentPerceptionLevel_       ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_           ( &PHY_PerceptionLevel::notSeen_ )
    , nTimeLastUpdate_               ( 0 )
    , rRelevance_                    ( rRelevance )
    , nTimeExtrapolationEnd_         ( -1 )
    , bLocked_                       ( false )
    , bValid_                        ( true )
    , bCreatedOnNetwork_             ( !pAgentKnown_->BelongsTo( *pKnowledgeGroup_ ) )
    , bRelevanceUpdated_             ( false )
    , bCurrentPerceptionLevelUpdated_( false )
    , bMaxPerceptionLevelUpdated_    ( false )
    , rLastRelevanceSent_            ( 0. )
{
    if( bCreatedOnNetwork_ )
        SendMsgCreation();
    if( rRelevance_ < 0. || rRelevance_ > 1. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Relevance: major-component not in [0..1]" );

}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_Agent::DEC_Knowledge_Agent()
    : DEC_Knowledge_ABC()
    , nID_                           ()
    , pKnowledgeGroup_               ()
    , pAgentKnown_                   ()
    , dataDetection_                 ()
    , dataRecognition_               ()
    , dataIdentification_            ()
    , nTimeLastUpdate_               ()
    , pCurrentPerceptionLevel_       ( &PHY_PerceptionLevel::notSeen_ )
    , pPreviousPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
    , pMaxPerceptionLevel_           ( &PHY_PerceptionLevel::notSeen_ )
    , rRelevance_                    ()
    , nTimeExtrapolationEnd_         ( -1 )
    , bLocked_                       ( false )
    , bValid_                        ( true )
    , bCreatedOnNetwork_             ( true )
    , bRelevanceUpdated_             ( true )
    , bCurrentPerceptionLevelUpdated_( true )
    , bMaxPerceptionLevelUpdated_    ( true )
    , rLastRelevanceSent_            ( 0. )
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

namespace boost
{
    namespace serialization
    {
        template< typename Archive >
        inline void serialize( Archive& file, DEC_Knowledge_Agent::T_PerceptionAutomateSourceMap& map, const unsigned int nVersion )
        {
            split_free( file, map, nVersion);
        }
        template < typename Archive >
        void save( Archive& file, const DEC_Knowledge_Agent::T_PerceptionAutomateSourceMap& map, const unsigned int )
        {
            unsigned size = map.size();
            file << size;
            for ( DEC_Knowledge_Agent::CIT_PerceptionAutomateSourceMap it = map.begin(); it != map.end(); ++it )
            {
                file << it->first;
                unsigned id = it->second->GetID();
                file << id;
            }

        }
        template < typename Archive >
        void load( Archive& file, DEC_Knowledge_Agent::T_PerceptionAutomateSourceMap& g, const unsigned int )
        {
            unsigned int nNbr;
            file >> nNbr;
            while ( nNbr-- )
            {
                 MIL_Automate* pAutomate;
                const PHY_PerceptionLevel* pLevel;
                file >> pAutomate;
                unsigned int nID;
                file >> nID;
                pLevel = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
                g.insert( std::make_pair( pAutomate, pLevel ) );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::load
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         >> const_cast< MIL_KnowledgeGroup*& >( pKnowledgeGroup_ )
         >> pAgentKnown_
         >> const_cast< unsigned int& >( nID_ )
         >> dataDetection_
         >> dataRecognition_
         >> dataIdentification_
         >> nTimeLastUpdate_;
    idManager_.Lock( nID_ );
    unsigned int nID;
    file >> nID;
    pCurrentPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
    file >> nID;
    pPreviousPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
    file >> nID;
    pMaxPerceptionLevel_ = &PHY_PerceptionLevel::FindPerceptionLevel( nID );
    file >> perceptionLevelPerAutomateMap_
         >> previousPerceptionLevelPerAutomateMap_
         >> rRelevance_
         >> bCreatedOnNetwork_
         >> bRelevanceUpdated_
         >> bCurrentPerceptionLevelUpdated_
         >> bMaxPerceptionLevelUpdated_
         >> nTimeExtrapolationEnd_;
    if( rRelevance_ < 0. || rRelevance_ > 1. )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Relevance: major-component not in [0..1]" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::save
// Created: JVT 2005-03-24
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    unsigned current = pCurrentPerceptionLevel_->GetID(),
             previous = pPreviousPerceptionLevel_->GetID(),
             max = pMaxPerceptionLevel_->GetID();
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << pKnowledgeGroup_
         << pAgentKnown_
         << nID_
         << dataDetection_
         << dataRecognition_
         << dataIdentification_
         << nTimeLastUpdate_
         << current
         << previous
         << max
         << perceptionLevelPerAutomateMap_
         << previousPerceptionLevelPerAutomateMap_
         << rRelevance_
         << bCreatedOnNetwork_
         << bRelevanceUpdated_
         << bCurrentPerceptionLevelUpdated_
         << bMaxPerceptionLevelUpdated_
         << nTimeExtrapolationEnd_;
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
    if( nTimeExtrapolationEnd_ > 0 || bLocked_ )
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
inline
void DEC_Knowledge_Agent::UpdatePerceptionSources( const DEC_Knowledge_AgentPerception& perception )
{
    if( perception.GetCurrentPerceptionLevel() == PHY_PerceptionLevel::notSeen_ )
        return;
    const MIL_Agent_ABC& pionSource = perception.GetAgentPerceiving();
    const MIL_Automate& automateSource = pionSource.GetAutomate();
    // On ne garde que les sources provenant d'autres knowledge groupes
    assert( pAgentKnown_ );
    if( pAgentKnown_->BelongsTo( automateSource.GetKnowledgeGroup() ) )
        return;
    IT_PerceptionAutomateSourceMap it = perceptionLevelPerAutomateMap_.find( &automateSource );
    if( it == perceptionLevelPerAutomateMap_.end() )
        perceptionLevelPerAutomateMap_.insert( std::make_pair( &automateSource, &perception.GetCurrentPerceptionLevel() ) );
    else
        it->second = &std::min( *it->second, perception.GetCurrentPerceptionLevel() );
    IT_PerceptionAgentSourceMap it2 = perceptionLevelPerAgentMap_.find( &pionSource );
    if( it2 == perceptionLevelPerAgentMap_.end() )
        perceptionLevelPerAgentMap_.insert( std::make_pair( &pionSource, &perception.GetCurrentPerceptionLevel() ) );
    else
        it2->second = &std::min( *it2->second, perception.GetCurrentPerceptionLevel() );
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
    assert( pKnowledgeGroup_ );
    dataDetection_.Update( perception.GetDetectionData() );
    dataRecognition_.Update( perception.GetRecognitionData() );
    dataIdentification_.Update( perception.GetIdentificationData() );
    UpdatePerceptionSources( perception );
    nTimeExtrapolationEnd_ = static_cast< int >( pKnowledgeGroup_->GetType().GetKnowledgeAgentExtrapolationTime() );
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
    ChangeRelevance( std::max( rRelevance_, knowledge.GetRelevance() ) );
    pMaxPerceptionLevel_ = &std::max( *pMaxPerceptionLevel_, knowledge.GetMaxPerceptionLevel() );
    bRelevanceUpdated_ = true;
    bMaxPerceptionLevelUpdated_ = true;
    bCurrentPerceptionLevelUpdated_ = true;
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::ChangeRelevance
// Created: NLD 2005-08-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::ChangeRelevance( MT_Float rNewRelevance )
{
    if( rRelevance_ == rNewRelevance )
        return;
    static const MT_Float rDeltaForNetwork = 0.05;
    if( fabs( rLastRelevanceSent_ - rNewRelevance ) > rDeltaForNetwork || rNewRelevance == 0. || rNewRelevance == 1. )
        bRelevanceUpdated_ = true;
    rRelevance_ = rNewRelevance;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::UpdateRelevance
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::UpdateRelevance(int currentTimeStep)
{
    // L'agent est percu
    if( *pCurrentPerceptionLevel_ != PHY_PerceptionLevel::notSeen_ )
    {
        assert( rRelevance_ == 1. );
        return;
    }
    assert( rRelevance_ >= 0. && rRelevance_ <= 1. );
    // La connaissance est invalid�e quand l'unit� r�elle a fait un magic move
    // ou quand on voit la position � laquelle l'unit� devrait se trouver <==== ou pas
    assert( pAgentKnown_ );
    assert( pKnowledgeGroup_ );
    if( pAgentKnown_->GetRole< PHY_RoleInterface_Location >().HasDoneMagicMove() /*|| pKnowledgeGroup_->IsPerceived( *this )*/ )
    {
        ChangeRelevance( 0. );
        return;
    }
    // Degradation : effacement au bout de X minutes
    const MT_Float rTimeRelevanceDegradation = ( currentTimeStep - nTimeLastUpdate_ ) / pKnowledgeGroup_->GetType().GetKnowledgeAgentMaxLifeTime();
    // Degradation : effacement quand l'unit� r�elle et l'unit� connnue sont distantes de X metres
    const MT_Float rDistanceBtwKnowledgeAndKnown = dataDetection_.GetPosition().Distance( pAgentKnown_->GetRole< PHY_RoleInterface_Location >().GetPosition() );
    const MT_Float rDistRelevanceDegradation     = rDistanceBtwKnowledgeAndKnown / pKnowledgeGroup_->GetType().GetKnowledgeAgentMaxDistBtwKnowledgeAndRealUnit();
    ChangeRelevance( std::max( 0., rRelevance_ - rTimeRelevanceDegradation - rDistRelevanceDegradation ) );
    nTimeLastUpdate_ = currentTimeStep;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::WriteMsgPerceptionSources
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::WriteMsgPerceptionSources( MsgsSimToClient::MsgUnitKnowledgeUpdate& asnMsg ) const
{
    if( !perceptionLevelPerAutomateMap_.empty() )
        for( CIT_PerceptionAutomateSourceMap it = perceptionLevelPerAutomateMap_.begin(); it != perceptionLevelPerAutomateMap_.end(); ++it )
        {
            MsgsSimToClient::AutomatPerception& perception = *asnMsg.mutable_perception_par_compagnie()->add_elem();
            perception.set_oid_compagnie( it->first->GetID() );
            MsgsSimToClient::EnumUnitIdentificationLevel level;
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
    if( !( bCurrentPerceptionLevelUpdated_ || bMaxPerceptionLevelUpdated_ || bRelevanceUpdated_  ||  bPerceptionPerAutomateUpdated
         || dataDetection_.HasChanged() || dataRecognition_.HasChanged() || dataIdentification_.HasChanged() ) )
        return;
    assert( pKnowledgeGroup_ );
    client::UnitKnowledgeUpdate asnMsg;
    asnMsg().set_oid( nID_ );
    asnMsg().set_oid_groupe_possesseur( pKnowledgeGroup_->GetId() );
    if( bRelevanceUpdated_ )
    {
        asnMsg().set_pertinence( (int)( rRelevance_ * 100. ) );
        rLastRelevanceSent_ = rRelevance_;
    }
    if( bCurrentPerceptionLevelUpdated_ )
    {
        MsgsSimToClient::EnumUnitIdentificationLevel level( asnMsg().identification_level() );
        pCurrentPerceptionLevel_->Serialize( level );
        asnMsg().set_identification_level( level );
    }
    if( bMaxPerceptionLevelUpdated_ )
    {
        MsgsSimToClient::EnumUnitIdentificationLevel level( asnMsg().max_identification_level() );
        pMaxPerceptionLevel_->Serialize( level );
        asnMsg().set_max_identification_level( level );
    }
    if( bPerceptionPerAutomateUpdated )
        WriteMsgPerceptionSources( asnMsg() );

    dataDetection_.SendChangedState( asnMsg() );
    dataRecognition_.SendChangedState( asnMsg() );
    dataIdentification_.SendChangedState( asnMsg() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
    if( asnMsg().has_perception_par_compagnie()  && asnMsg().perception_par_compagnie().elem_size() > 0 )
        asnMsg().mutable_perception_par_compagnie()->Clear(); //$$$ RAM
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::SendFullState
// Created: NLD 2004-11-10
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::SendFullState()
{
    assert( pKnowledgeGroup_ );
    client::UnitKnowledgeUpdate asnMsg;
    asnMsg().set_oid( nID_ );
    asnMsg().set_oid_groupe_possesseur( pKnowledgeGroup_->GetId() );
    asnMsg().set_pertinence( static_cast< int >( rRelevance_ * 100. ) );
    rLastRelevanceSent_ = rRelevance_;
    MsgsSimToClient::EnumUnitIdentificationLevel level( asnMsg().identification_level() );
    pCurrentPerceptionLevel_->Serialize( level );
    asnMsg().set_identification_level( level );
    MsgsSimToClient::EnumUnitIdentificationLevel maxlevel( asnMsg().max_identification_level() );
    pMaxPerceptionLevel_->Serialize( maxlevel );
    asnMsg().set_max_identification_level( maxlevel );
    WriteMsgPerceptionSources( asnMsg() );
    dataDetection_.SendFullState( asnMsg() );
    dataRecognition_ .SendFullState( asnMsg() );
    dataIdentification_.SendFullState( asnMsg() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
    if( asnMsg().has_perception_par_compagnie()  && asnMsg().perception_par_compagnie().elem_size() > 0 )
        asnMsg().mutable_perception_par_compagnie()->Clear(); //$$$ RAM
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::UpdateOnNetwork
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::UpdateOnNetwork()
{
    assert( pAgentKnown_ );
    assert( pKnowledgeGroup_ );
    if( pAgentKnown_->BelongsTo( *pKnowledgeGroup_ ) )
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
    assert( pKnowledgeGroup_ );
    assert( pAgentKnown_ );
    client::UnitKnowledgeCreation asnMsg;
    asnMsg().set_oid( nID_ );
    asnMsg().set_oid_groupe_possesseur( pKnowledgeGroup_->GetId() );
    asnMsg().set_oid_unite_reelle( pAgentKnown_->GetID() );
    asnMsg().mutable_type_unite()->set_type( pAgentKnown_->GetType().GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::SendMsgDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::SendMsgDestruction() const
{
    if( pKnowledgeGroup_ )
    {
        client::UnitKnowledgeDestruction asnMsg;
        asnMsg().set_oid( nID_ );
        asnMsg().set_oid_groupe_possesseur( pKnowledgeGroup_->GetId() );
        asnMsg.Send( NET_Publisher_ABC::Publisher() );
    }
}

// ----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::DegradeDangerosity
// Created: NLD 2004-05-26
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::DegradeDangerosity( MT_Float& rDangerosity ) const
{
    // Pertinence
    rDangerosity *= ( 1 - ( (-rMaxDangerosityDegradationByRelevance_ * rRelevance_) + rMaxDangerosityDegradationByRelevance_ ) );
    // Etat op�rationel
    const MT_Float rOpState = dataRecognition_.GetOperationalState();
    if( rOpState == 0. ) // L'unit� est morte
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
MT_Float DEC_Knowledge_Agent::GetDangerosity( const MIL_Agent_ABC& target ) const
{
    if( *pMaxPerceptionLevel_ < PHY_PerceptionLevel::recognized_
        ||  IsAFriend( target.GetArmy() ) == eTristate_True
        ||  dataDetection_.IsSurrendered() )
        return 0.;
    // Target is dead ....
    const PHY_ComposantePion* pTargetMajorComposante = target.GetRole< PHY_RolePion_Composantes >().GetMajorComposante();
    if( !pTargetMajorComposante )
        return 0.;
    MT_Float rDangerosity = 0.;
    // Fight score
    const PHY_RoleInterface_Location& targetLocation = target.GetRole< PHY_RoleInterface_Location >();
    const MT_Vector3D vTargetPosition( targetLocation.GetPosition().rX_, targetLocation.GetPosition().rY_, targetLocation.GetAltitude() );
    const MT_Vector3D vDataPosition  ( dataDetection_.GetPosition().rX_, dataDetection_.GetPosition().rY_, dataDetection_.GetAltitude() );
    const MT_Float    rDistBtwSourceAndTarget = vTargetPosition.Distance( vDataPosition );
    const T_KnowledgeComposanteVector& composantes = dataRecognition_.GetComposantes();
    for( CIT_KnowledgeComposanteVector itComposante = composantes.begin(); itComposante != composantes.end(); ++itComposante )
        rDangerosity = std::max( rDangerosity, itComposante->GetDangerosity( *pAgentKnown_, *pTargetMajorComposante, rDistBtwSourceAndTarget ) );
    DegradeDangerosity( rDangerosity );
    return rDangerosity;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetDangerosity
// Created: NLD 2004-05-07
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Agent::GetDangerosity( const DEC_Knowledge_Agent& target ) const
{
    if( *pMaxPerceptionLevel_ < PHY_PerceptionLevel::recognized_ )
        return 0.;
        // Same team ...
//    if( GetArmy() && GetArmy().IsAFriend( target ) == eTristate_True )
//        return 0.;
    MT_Float rDangerosity = 0.;
    // Target is dead ....
    const DEC_Knowledge_AgentComposante* pTargetMajorComposante = target.GetMajorComposante();
    if( !pTargetMajorComposante )
        return 0.;
    // Fight score
    const MT_Vector3D vTargetPosition( target.GetPosition().rX_, target.GetPosition().rY_, target.GetAltitude() );
    const MT_Vector3D vDataPosition  ( dataDetection_.GetPosition().rX_, dataDetection_.GetPosition().rY_, dataDetection_.GetAltitude() );
    const MT_Float rDistBtwSourceAndTarget = vTargetPosition.Distance( vDataPosition );
    const T_KnowledgeComposanteVector& composantes = dataRecognition_.GetComposantes();
    for( CIT_KnowledgeComposanteVector itComposante = composantes.begin(); itComposante != composantes.end(); ++itComposante )
        rDangerosity = std::max( rDangerosity, itComposante->GetDangerosity( *pAgentKnown_, *pTargetMajorComposante, rDistBtwSourceAndTarget ) );
    DegradeDangerosity( rDangerosity );
    return rDangerosity;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetMaxRangeToFireOn
// Created: NLD 2004-04-13
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Agent::GetMaxRangeToFireOn( const MIL_Agent_ABC& target, MT_Float rWantedPH ) const
{
    // Get back the most dangerous composante type of the target (from our point of view ...)
    const PHY_ComposantePion* pTargetComposante = target.GetRole< PHY_RolePion_Composantes >().GetMajorComposante();
    if( !pTargetComposante )
        return 0.;
    MT_Float rRange = 0;
    const T_KnowledgeComposanteVector& composantes = dataRecognition_.GetComposantes();
    for( CIT_KnowledgeComposanteVector itComposante = composantes.begin(); itComposante != composantes.end(); ++itComposante )
    {
        const DEC_Knowledge_AgentComposante& composante = *itComposante;
        rRange = std::max( rRange, composante.GetMaxRangeToFireOn( *pAgentKnown_, *pTargetComposante, rWantedPH ) );
    }
    return rRange;
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
    for( CIT_KnowledgeComposanteVector it = composantes.begin(); it != composantes.end(); ++it )
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
double DEC_Knowledge_Agent::GetMaterialComposantesAttritionLevel( boost::shared_ptr< DEC_Knowledge_Urban > urbanKnowledge ) const
{
    const urban::Architecture* architecture = urbanKnowledge->GetTerrainObjectKnown().RetrievePhysicalFeature< urban::Architecture >();
    if( architecture )
    {
        const PHY_RolePion_Composantes& role = GetAgentKnown().GetRole< PHY_RolePion_Composantes >();
        unsigned materialID = UrbanType::GetUrbanType().GetStaticModel().FindType< urban::MaterialCompositionType >( architecture->GetMaterial() )->GetId();

        if (GetMaxPerceptionLevel() == PHY_PerceptionLevel::identified_)
            return role.GetAttritionIndexComposante(materialID);
        else if ((GetMaxPerceptionLevel() == PHY_PerceptionLevel::recognized_) || (GetMaxPerceptionLevel() == PHY_PerceptionLevel::detected_) )
            return PHY_DotationCategory::FindUrbanProtection( materialID );
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
    bLocked_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::Unlock
// Created: NLD 2005-04-21
// -----------------------------------------------------------------------------
void DEC_Knowledge_Agent::Unlock()
{
    bLocked_ = false;
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
    MT_Float rSignificantVolumeFactor     = 0.;
    const T_ComposanteVolumeSet& visionVolumes = dataDetection_.GetVisionVolumes();
    for( CIT_ComposanteVolumeSet it = visionVolumes.begin(); it != visionVolumes.end(); ++it )
    {
        const PHY_Volume& volume = **it;
        MT_Float rVolumeFactor = sensorType.GetFactor( volume );
        if( rVolumeFactor > rSignificantVolumeFactor )
        {
            pSignificantVolume = &volume;
            rSignificantVolumeFactor =  rVolumeFactor;
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
// Name: DEC_Knowledge_Agent::GetPosition
// Created: MGD 2010-05-05
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Agent::IsInUrbanBlock( const urban::TerrainObject_ABC& urban ) const
{
    const MT_Vector2D& pos = GetPosition();
    return urban.GetFootprint()->IsInside( geometry::Point2f( static_cast< float >( pos.rX_ ), static_cast< float >( pos.rY_ ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetDirection
// Created: JVT 2005-02-17
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_Knowledge_Agent::GetDirection() const
{
    return dataDetection_.GetDirection();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetAltitude
// Created: NLD 2004-06-07
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Agent::GetAltitude() const
{
    return dataDetection_.GetAltitude();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetSpeed
// Created: NLD 2004-03-25
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Agent::GetSpeed() const
{
    return dataDetection_.GetSpeed();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetRelevance
// Created: NLD 2004-03-22
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Agent::GetRelevance() const
{
    return rRelevance_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetPostureCompletionPercentage
// Created: NLD 2004-04-01
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Agent::GetPostureCompletionPercentage() const
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
    CIT_PerceptionAgentSourceMap  itPerceptionLevel = perceptionLevelPerAgentMap_.find( &pion );
    if( itPerceptionLevel != perceptionLevelPerAgentMap_.end() )
        return *( itPerceptionLevel->second );
    else
        return PHY_PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetOperationalState
// Created: NLD 2004-04-14
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Agent::GetOperationalState() const
{
    return dataRecognition_.GetOperationalState();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetMajorOperationalState
// Created: NLD 2005-11-30
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Agent::GetMajorOperationalState() const
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
    bLocked_ = agent.bLocked_;
    bValid_ = agent.bValid_;
}
