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
#include "Entities/MIL_EntityVisitor_ABC.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/MIL_Army.h"
#include "Network/NET_Publisher_ABC.h"
#include "protocol/ClientSenders.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_Knowledge_Population )

MIL_IDManager DEC_Knowledge_Population::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::DEC_Knowledge_Population( const MIL_KnowledgeGroup& group, MIL_Population& populationKnown )
    : pPopulationKnown_            ( &populationKnown )
    , pHackedPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
    , criticalIntelligence_        ( "" )
    , nID_                         ( idManager_.GetId() )
    , groupId_                     ( group.GetId() )
    , rDominationState_            ( 0. )
    , maxLifetime_                 ( group.GetType().GetKnowledgePopulationMaxLifeTime() )
    , bIsRecon_                    ( false )
    , bReconAttributesValid_       ( false )
    , bDecStateUpdated_            ( false )
    , bCriticalIntelligenceUpdated_( false )
    , locked_                      ( 0 )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population constructor
// Created: LGY 2012-05-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::DEC_Knowledge_Population( const DEC_Knowledge_Population& knowledge, const MIL_KnowledgeGroup& group )
    : pPopulationKnown_            ( knowledge.pPopulationKnown_ )
    , pHackedPerceptionLevel_      ( knowledge.pHackedPerceptionLevel_ )
    , criticalIntelligence_        ( knowledge.criticalIntelligence_ )
    , nID_                         ( knowledge.idManager_.GetId() )
    , groupId_                     ( group.GetId() )
    , rDominationState_            ( knowledge.rDominationState_ )
    , maxLifetime_                 ( group.GetType().GetKnowledgePopulationMaxLifeTime() )
    , bIsRecon_                    ( knowledge.bIsRecon_ )
    , bReconAttributesValid_       ( knowledge.bReconAttributesValid_ )
    , bDecStateUpdated_            ( knowledge.bDecStateUpdated_ )
    , bCriticalIntelligenceUpdated_( knowledge.bCriticalIntelligenceUpdated_ )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::DEC_Knowledge_Population()
    : pPopulationKnown_            ( 0 )
    , pHackedPerceptionLevel_      ( &PHY_PerceptionLevel::notSeen_ )
    , criticalIntelligence_        ( "" )
    , nID_                         ( 0 )
    , groupId_                     ( 0 )
    , rDominationState_            ( 0 )
    , maxLifetime_                 ( 0 )
    , bIsRecon_                    ( false )
    , bReconAttributesValid_       ( false )
    , bDecStateUpdated_            ( false )
    , bCriticalIntelligenceUpdated_( false )
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
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file >> pPopulationKnown_;
    file >> nID_;
    file >> groupId_;
    file >> concentrations_;
    file >> flows_;
    file >> bIsRecon_;
    file >> bReconAttributesValid_;
    file >> rDominationState_;
    file >> maxLifetime_;
    file >> criticalIntelligence_;
    idManager_.GetId( nID_, true );
    assert( pPopulationKnown_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::save
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::save( MIL_CheckPointOutArchive& file, const unsigned int ) const
{
    file << boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
    file << pPopulationKnown_;
    file << nID_;
    file << groupId_;
    file << concentrations_;
    file << flows_;
    file << bIsRecon_;
    file << bReconAttributesValid_;
    file << rDominationState_;
    file << maxLifetime_;
    file << criticalIntelligence_;
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
// Name: DEC_Knowledge_Population::WriteKnowledges
// Created: NPT 2012-08-09
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::WriteKnowledges( xml::xostream& xos ) const
{
    xos << xml::start( "population-knowledge" );
    if( pPopulationKnown_ )
        xos << xml::attribute( "population-known", pPopulationKnown_->GetID() );

    if( pHackedPerceptionLevel_ && pHackedPerceptionLevel_ != &PHY_PerceptionLevel::notSeen_ )
        xos << xml::attribute( "hacked", pHackedPerceptionLevel_->GetID() );
    if( !criticalIntelligence_.empty() )
        xos << xml::attribute( "critical-intelligence", criticalIntelligence_ );
    xos     << xml::attribute( "id", nID_ );

    for( auto it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->WriteKnowledges( xos );
    for( auto it = flows_.begin(); it != flows_.end(); ++it )
        it->second->WriteKnowledges( xos );

    if( rDominationState_ )
        xos << xml::attribute( "domination-state", rDominationState_ );
    if( bIsRecon_ )
        xos << xml::attribute( "is-recon", bIsRecon_ );
    if( bReconAttributesValid_ )
        xos << xml::attribute( "recon-attributes-valid", bReconAttributesValid_ );
    xos << xml::end;
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
// Created: LDC 2014-02-27
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Update( const DEC_Knowledge_Population& pKnowledge, int /*currentTimeStep*/ )
{
    CopyFrom( pKnowledge );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::UpdateFromCrowdPerception
// Created: JSR 2013-07-11
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateFromCrowdPerception( const MIL_PopulationConcentration& concentration, int currentTimeStep )
{
    GetKnowledge( concentration ).UpdateFromCrowdPerception( currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::UpdateFromCrowdPerception
// Created: JSR 2013-07-11
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateFromCrowdPerception( const MIL_PopulationFlow& flow, int currentTimeStep )
{
    GetKnowledge( flow ).UpdateFromCrowdPerception( currentTimeStep );
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
    for( auto it = concentrations_.begin(); it != concentrations_.end(); )
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
    for( auto it = flows_.begin(); it != flows_.end(); )
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
boost::shared_ptr< MT_Vector2D > DEC_Knowledge_Population::GetSecuringPoint( const MIL_Agent_ABC& securingAgent ) const
{
    assert( pPopulationKnown_ );
    return pPopulationKnown_->GetSecuringPoint( securingAgent );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetSafetyPosition
// Created: SBO 2005-12-16
// -----------------------------------------------------------------------------
boost::shared_ptr< MT_Vector2D > DEC_Knowledge_Population::GetSafetyPosition( const MIL_AgentPion& agent, double rMinDistance ) const
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
    asnMsg().mutable_knowledge_group()->set_id( groupId_ );
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
    asnMsg().mutable_knowledge_group()->set_id( groupId_ );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateOnNetwork() const
{
    if( bCriticalIntelligenceUpdated_ || bReconAttributesValid_ && bDecStateUpdated_ )
    {
        client::CrowdKnowledgeUpdate asnMsg;
        asnMsg().mutable_knowledge()->set_id( nID_ );
        asnMsg().mutable_knowledge_group()->set_id( groupId_ );
        if( bDecStateUpdated_ )
            asnMsg().set_domination( static_cast< unsigned int >( rDominationState_ * 100. ) );
        if( bCriticalIntelligenceUpdated_ )
            asnMsg().set_critical_intelligence( criticalIntelligence_ );
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
        asnMsg().mutable_knowledge_group()->set_id( groupId_ );
        if( bReconAttributesValid_)
            asnMsg().set_domination( static_cast< unsigned int>( rDominationState_ * 100. ) );
        if( !criticalIntelligence_.empty() )
            asnMsg().set_critical_intelligence( criticalIntelligence_ );
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
// Name: DEC_Knowledge_Population::GetMaxLifeTime
// Created: LDC 2012-08-28
// -----------------------------------------------------------------------------
double DEC_Knowledge_Population::GetMaxLifeTime() const
{
    return maxLifetime_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetKnowledgeGroupId
// Created: LDC 2012-08-28
// -----------------------------------------------------------------------------
const unsigned int DEC_Knowledge_Population::GetKnowledgeGroupId() const
{
    return groupId_;
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
    for( auto it = knowledge.flows_.begin(); it != knowledge.flows_.end(); ++it )
        flows_[ it->first ] = new DEC_Knowledge_PopulationFlow( *this, *it->second );
    bIsRecon_ = knowledge.bIsRecon_;
    bReconAttributesValid_ = knowledge.bReconAttributesValid_;
    rDominationState_ = knowledge.rDominationState_;
    criticalIntelligence_ = knowledge.criticalIntelligence_;
    bDecStateUpdated_ = true;
    bCriticalIntelligenceUpdated_ = true;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Merge
// Created: LGY 2013-04-02
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Merge( const DEC_Knowledge_Population& knowledge )
{
    if( pHackedPerceptionLevel_ > knowledge.pHackedPerceptionLevel_ )
        pHackedPerceptionLevel_ = knowledge.pHackedPerceptionLevel_;
    if( knowledge.criticalIntelligence_ != "" )
        criticalIntelligence_ = knowledge.criticalIntelligence_;
    if( knowledge.bIsRecon_ )
        bIsRecon_ = knowledge.bIsRecon_;

    for( auto it = knowledge.concentrations_.begin(); it != knowledge.concentrations_.end(); ++it )
    {
        auto current = concentrations_.find( it->first );
        if( current == concentrations_.end() )
            concentrations_[ it->first ] = new DEC_Knowledge_PopulationConcentration( *this, *it->second );
        else if( it->second->GetRelevance() > current->second->GetRelevance() )
        {
            concentrations_.erase( it->first );
            concentrations_[ it->first ] = new DEC_Knowledge_PopulationConcentration( *this, *it->second );
        }
    }
    for( auto it = knowledge.flows_.begin(); it != knowledge.flows_.end(); ++it )
    {
        auto current = flows_.find( it->first );
        if( current == flows_.end() )
            flows_[ it->first ] = new DEC_Knowledge_PopulationFlow( *this, *it->second );
        else if( it->second->GetRelevance() > current->second->GetRelevance() )
        {
            flows_.erase( it->first );
            flows_[ it->first ] = new DEC_Knowledge_PopulationFlow( *this, *it->second );
        }
    }
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

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetConcentrationMap
// Created: LGY 2013-04-02
// -----------------------------------------------------------------------------
const DEC_Knowledge_Population::T_ConcentrationMap& DEC_Knowledge_Population::GetConcentrationMap() const
{
    return concentrations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetFlowMap
// Created: LGY 2013-04-02
// -----------------------------------------------------------------------------
const DEC_Knowledge_Population::T_FlowMap& DEC_Knowledge_Population::GetFlowMap() const
{
    return flows_;
}
