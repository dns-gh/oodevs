// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_Population.cpp $
// $Author: Jvt $
// $Modtime: 6/04/05 12:59 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_Population.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationConcentration.h"
#include "DEC_Knowledge_PopulationFlow.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_Knowledge_PopulationConcentrationPerception.h"
#include "DEC_Knowledge_PopulationFlowPerception.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "MIL_KnowledgeGroup.h"
#include "Checkpoints/SerializationTools.h"
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/MIL_Army.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"
#include <boost/serialization/map.hpp>

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_Population )

MIL_IDManager DEC_Knowledge_Population::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::DEC_Knowledge_Population( boost::shared_ptr< MIL_KnowledgeGroup >& knowledgeGroup, MIL_Population& populationKnown )
    : DEC_Knowledge_ABC()
    , pPopulationKnown_            ( &populationKnown )
    , pKnowledgeGroup_             ( knowledgeGroup )
    , pHackedPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
    , criticalIntelligence_        ( "" )
    , nID_                         ( idManager_.GetFreeId() )
    , rDominationState_            ( 0. )
    , armedIndividuals_            ( 0. )
    , bIsRecon_                    ( false )
    , bReconAttributesValid_       ( false )
    , bDecStateUpdated_            ( false )
    , bCriticalIntelligenceUpdated_( false )
    , bLastCriticalIntelligenceUpdated_( false )
    , locked_                      ( 0 )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::DEC_Knowledge_Population()
    : DEC_Knowledge_ABC()
    , pPopulationKnown_            ( 0 )
    , pHackedPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
    , criticalIntelligence_        ( "" )
    , nID_                         ( 0 )
    , rDominationState_            ( 0. )
    , armedIndividuals_            ( 0. )
    , bIsRecon_                    ( false )
    , bReconAttributesValid_       ( false )
    , bDecStateUpdated_            ( false )
    , bCriticalIntelligenceUpdated_( false )
    , bLastCriticalIntelligenceUpdated_( false )
    , locked_                      ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::~DEC_Knowledge_Population()
{
    SendMsgDestruction();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::load
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         >> pPopulationKnown_
         >> const_cast< unsigned int& >( nID_ )
         >> pKnowledgeGroup_;
    file >> concentrations_
         >> flows_
         >> bIsRecon_
         >> bReconAttributesValid_
         >> rDominationState_
         >> armedIndividuals_
         >> criticalIntelligence_;
    idManager_.Lock( nID_ );
    assert( pPopulationKnown_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::save
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         << pPopulationKnown_
         << nID_
         << pKnowledgeGroup_
         << concentrations_
         << flows_
         << bIsRecon_
         << bReconAttributesValid_
         << rDominationState_
         << armedIndividuals_
         << criticalIntelligence_;
}

namespace
{
    class HackVisitor : public MIL_EntityVisitor_ABC< MIL_PopulationElement_ABC >
    {
    public:
        HackVisitor( std::vector< const MIL_PopulationElement_ABC* >& elements )
            : elements_( elements )
        {

        }
        virtual ~HackVisitor() {}
        virtual void Visit( const MIL_PopulationElement_ABC& element )
        {
            elements_.push_back( &element );
        }
    private:
        std::vector< const MIL_PopulationElement_ABC* >& elements_;
    };
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Prepare()
{
    if( pHackedPerceptionLevel_ != &PHY_PerceptionLevel::notSeen_ && pPopulationKnown_ )
    {
        std::vector< const MIL_PopulationElement_ABC* > elements;
        HackVisitor visitor( elements );
        pPopulationKnown_->Apply( visitor );
        for( auto it = elements.begin(); it != elements.end(); ++it )
        {
            if( const MIL_PopulationConcentration* concentration = dynamic_cast< const MIL_PopulationConcentration* >( *it ) )
                GetKnowledge( *concentration ).HackPerceptionLevel( pHackedPerceptionLevel_ );
            if( const MIL_PopulationFlow* flow = dynamic_cast< const MIL_PopulationFlow* >( *it ) )
                GetKnowledge( *flow ).HackPerceptionLevel( pHackedPerceptionLevel_ );
        }
    }
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->Prepare();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        it->second->Prepare();
    bDecStateUpdated_ = false;
    bLastCriticalIntelligenceUpdated_ = bCriticalIntelligenceUpdated_;
    bCriticalIntelligenceUpdated_ = false;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::UpdateReconAttributes
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateReconAttributes()
{
    if( IsRecon() )
        bReconAttributesValid_ = true;
    assert( pPopulationKnown_ );
    if( rDominationState_ != pPopulationKnown_->GetDecision().GetDominationState() )
    {
        rDominationState_ = pPopulationKnown_->GetDecision().GetDominationState();
        bDecStateUpdated_ = true;
    }
    if( armedIndividuals_ != pPopulationKnown_->GetArmedIndividuals() )
    {
        armedIndividuals_ = pPopulationKnown_->GetArmedIndividuals();
        bDecStateUpdated_ = true;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Update
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Update( const DEC_Knowledge_PopulationCollision& collision )
{
    UpdateReconAttributes();
    collision.PublishKnowledges( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Update
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Update( const DEC_Knowledge_PopulationCollision& collision, const MIL_PopulationFlow& flow )
{
    GetKnowledge( flow ).Update( collision );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Update
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Update( const DEC_Knowledge_PopulationCollision& collision, const MIL_PopulationConcentration& concentration )
{
    GetKnowledge( concentration ).Update( collision );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Update
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Update( const DEC_Knowledge_PopulationPerception& perception )
{
    UpdateReconAttributes();
    perception.PublishKnowledges( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Update
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Update( const DEC_Knowledge_PopulationConcentrationPerception& perception )
{
    GetKnowledge( perception.GetConcentrationPerceived() ).Update( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Update
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Update( const DEC_Knowledge_PopulationFlowPerception& perception )
{
    GetKnowledge( perception.GetFlowPerceived() ).Update( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Update
// Created: LGY 2013-08-06
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Update( const DEC_Knowledge_Population& pKnowledge, int /*currentTimeStep*/ )
{
    CopyFrom( pKnowledge );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::UpdateRelevance
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateRelevance()
{
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->UpdateRelevance();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        it->second->UpdateRelevance();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Population::Clean()
{
    bDecStateUpdated_ = false;
    for( IT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); )
    {
        DEC_Knowledge_PopulationConcentration* pKnowledge = it->second;
        if( pKnowledge->Clean() )
        {
            delete pKnowledge;
            it = concentrations_.erase( it );
        }
        else
            ++ it;
    }
    for( IT_FlowMap it = flows_.begin(); it != flows_.end(); )
    {
        DEC_Knowledge_PopulationFlow* pKnowledge = it->second;
        if( pKnowledge->Clean() )
        {
            delete pKnowledge;
            it = flows_.erase( it );
        }
        else
            ++ it;
    }
    return concentrations_.empty() && flows_.empty() && !locked_;
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Secure
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Secure( const MIL_AgentPion& pionSecuring )
{
    assert( pPopulationKnown_ );
    pPopulationKnown_->Secure( pionSecuring );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Exterminate
// Created: SBO 2005-12-22
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Exterminate( const MIL_AgentPion& exterminator, double rSurface )
{
    assert( pPopulationKnown_ );
    pPopulationKnown_->Exterminate( exterminator, rSurface );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetKnowledge
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationConcentration& DEC_Knowledge_Population::GetKnowledge( const MIL_PopulationConcentration& concentration )
{
    DEC_Knowledge_PopulationConcentration*& pKnowledge = concentrations_[ concentration.GetID() ];
    if( !pKnowledge )
        pKnowledge = new DEC_Knowledge_PopulationConcentration( *this, concentration );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetKnowledge
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
DEC_Knowledge_PopulationFlow& DEC_Knowledge_Population::GetKnowledge( const MIL_PopulationFlow& flow )
{
    DEC_Knowledge_PopulationFlow*& pKnowledge = flows_[ flow.GetID() ];
    if( !pKnowledge )
        pKnowledge = new DEC_Knowledge_PopulationFlow( *this, flow );
    return *pKnowledge;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetDangerosity
// Created: NLD 2005-11-10
// -----------------------------------------------------------------------------
double DEC_Knowledge_Population::GetDangerosity( const MIL_AgentPion& target ) const
{
    assert( pPopulationKnown_ );
    return pPopulationKnown_->GetDangerosity( target );
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetAttitude
// Created: SBO 2006-04-05
// -----------------------------------------------------------------------------
const MIL_PopulationAttitude& DEC_Knowledge_Population::GetAttitude() const
{
    assert( pPopulationKnown_ );
    return pPopulationKnown_->GetAttitude();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Agent::GetArmy
// Created: HME 2005-12-29
// -----------------------------------------------------------------------------
const MIL_Army_ABC& DEC_Knowledge_Population::GetArmy() const
{
    assert( pPopulationKnown_ );
    return pPopulationKnown_->GetArmy();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::IsInZone
// Created: NLD 2005-12-02
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Population::IsInZone( const TER_Localisation& loc ) const
{
    assert( pPopulationKnown_ );
    return pPopulationKnown_->IsInZone( loc );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetClosestPoint
// Created: NLD 2005-12-05
// -----------------------------------------------------------------------------
MT_Vector2D DEC_Knowledge_Population::GetClosestPoint( const MT_Vector2D& refPos ) const
{
    assert( pPopulationKnown_ );
    return pPopulationKnown_->GetClosestPoint( refPos );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetSecuringPoint
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
MT_Vector2D DEC_Knowledge_Population::GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const
{
    assert( pPopulationKnown_ );
    return pPopulationKnown_->GetSecuringPoint( securingAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetSafetyPosition
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
MT_Vector2D DEC_Knowledge_Population::GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance ) const
{
    assert( pPopulationKnown_ );
    return pPopulationKnown_->GetSafetyPosition( agent, rMinDistance );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::SendMsgCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::SendMsgCreation() const
{
    assert( pPopulationKnown_ );
    client::CrowdKnowledgeCreation asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_knowledge_group()->set_id( pKnowledgeGroup_->GetId() );
    asnMsg().mutable_crowd()->set_id( pPopulationKnown_->GetID() );
    asnMsg().mutable_party()->set_id( GetArmy().GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::SendMsgDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::SendMsgDestruction() const
{
    client::CrowdKnowledgeDestruction asnMsg;
    asnMsg().mutable_knowledge()->set_id( nID_ );
    asnMsg().mutable_knowledge_group()->set_id( pKnowledgeGroup_->GetId() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateOnNetwork() const
{
    if( bLastCriticalIntelligenceUpdated_ || bReconAttributesValid_ && bDecStateUpdated_ )
    {
        client::CrowdKnowledgeUpdate asnMsg;
        asnMsg().mutable_knowledge()->set_id( nID_ );
        asnMsg().mutable_knowledge_group()->set_id( pKnowledgeGroup_->GetId() );
        if( bDecStateUpdated_ )
            asnMsg().set_domination( static_cast< unsigned int >( rDominationState_ * 100. ) );
        if( bLastCriticalIntelligenceUpdated_ )
            asnMsg().set_critical_intelligence( criticalIntelligence_ );
        asnMsg().set_armed_individuals( armedIndividuals_ );
        asnMsg.Send( NET_Publisher_ABC::Publisher() );
    }
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->UpdateOnNetwork();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        it->second->UpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::SendStateToNewClient() const
{
    SendMsgCreation();
    if( bReconAttributesValid_ || !criticalIntelligence_.empty() )
    {
        client::CrowdKnowledgeUpdate asnMsg;
        asnMsg().mutable_knowledge()->set_id( nID_ );
        asnMsg().mutable_knowledge_group()->set_id( pKnowledgeGroup_->GetId() );
        if( bReconAttributesValid_)
            asnMsg().set_domination( static_cast< unsigned int>( rDominationState_ * 100. ) );
        if( !criticalIntelligence_.empty() )
            asnMsg().set_critical_intelligence( criticalIntelligence_ );
        asnMsg().set_armed_individuals( armedIndividuals_ );
        asnMsg.Send( NET_Publisher_ABC::Publisher() );
    }
    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->SendStateToNewClient();
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        it->second->SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetDominationState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
double DEC_Knowledge_Population::GetDominationState() const
{
    return rDominationState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetID
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
unsigned int DEC_Knowledge_Population::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetKnowledgeGroupType
// Created: LDC 2012-08-28
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroupType& DEC_Knowledge_Population::GetKnowledgeGroupType() const
{
    return pKnowledgeGroup_->GetType();
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetKnowledgeGroupId
// Created: LDC 2012-08-28
// -----------------------------------------------------------------------------
const unsigned int DEC_Knowledge_Population::GetKnowledgeGroupId() const
{
    return pKnowledgeGroup_->GetId();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetPopulationKnown
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
MIL_Population& DEC_Knowledge_Population::GetPopulationKnown() const
{
    assert( pPopulationKnown_ );
    return *pPopulationKnown_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::IsRecon
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Population::IsRecon() const
{
    return bIsRecon_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Recon
// Created: NLD 2005-11-03
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Recon()
{
    bIsRecon_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetCriticalIntelligence
// Created: ABR 2011-03-16
// -----------------------------------------------------------------------------
const std::string& DEC_Knowledge_Population::GetCriticalIntelligence() const
{
    return criticalIntelligence_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::SetCriticalIntelligenceFromPopulationKnown
// Created: ABR 2011-03-16
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::SetCriticalIntelligenceFromPopulationKnown()
{
    criticalIntelligence_ = pPopulationKnown_->GetCriticalIntelligence();
    bCriticalIntelligenceUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::CopyFrom
// Created: LDC 2010-04-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::CopyFrom( const DEC_Knowledge_Population& knowledge )
{
    for( auto it = knowledge.concentrations_.begin(); it != knowledge.concentrations_.end(); ++it )
        concentrations_[ it->first ] = new DEC_Knowledge_PopulationConcentration( *this, *it->second );
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        flows_[ it->first ] = new DEC_Knowledge_PopulationFlow( *this, *it->second );
    bIsRecon_ = knowledge.bIsRecon_;
    bReconAttributesValid_ = knowledge.bReconAttributesValid_;
    rDominationState_ = knowledge.rDominationState_;
    armedIndividuals_ = knowledge.armedIndividuals_;
    criticalIntelligence_ = knowledge.criticalIntelligence_;
    bDecStateUpdated_ = true;
    bCriticalIntelligenceUpdated_ = true;
}


// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::HackPerceptionLevel
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::HackPerceptionLevel( const PHY_PerceptionLevel* pPerceptionLevel )
{
    if( *pPerceptionLevel > *pHackedPerceptionLevel_ )
    {
        pHackedPerceptionLevel_ = pPerceptionLevel;
        for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
            it->second->HackPerceptionLevel( pPerceptionLevel );
        for( auto it = flows_.begin(); it != flows_.end(); ++it )
            it->second->HackPerceptionLevel( pPerceptionLevel );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::IsPerceptionDistanceHacked
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Population::IsPerceptionDistanceHacked() const
{
    return ( pHackedPerceptionLevel_!= &PHY_PerceptionLevel::notSeen_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetHackedPerceptionLevel
// Created: MMC 2011-06-14
// -----------------------------------------------------------------------------
const PHY_PerceptionLevel* DEC_Knowledge_Population::GetHackedPerceptionLevel() const
{
    return pHackedPerceptionLevel_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Lock
// Created: LDC 2013-05-22
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Lock()
{
    ++locked_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Unlock
// Created: LDC 2013-05-22
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Unlock()
{
    --locked_;
}
