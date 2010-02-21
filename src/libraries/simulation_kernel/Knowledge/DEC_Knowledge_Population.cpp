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
DEC_Knowledge_Population::DEC_Knowledge_Population( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Population& populationKnown )
    : DEC_Knowledge_ABC     ()
    , nID_                  ( idManager_.GetFreeId() )
    , pKnowledgeGroup_      ( &knowledgeGroup )
    , pPopulationKnown_     ( &populationKnown )
    , concentrations_       ()
    , flows_                ()
    , bIsRecon_             ( false )
    , bReconAttributesValid_( false )
    , bDecStateUpdated_     ( false )
    , rDominationState_     ( 0. )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::DEC_Knowledge_Population()
    : DEC_Knowledge_ABC     ()
    , nID_                  ( 0 )
    , pKnowledgeGroup_      ( 0 )
    , pPopulationKnown_     ( 0 )
    , concentrations_       ()
    , flows_                ()
    , bIsRecon_             ( false )
    , bReconAttributesValid_( false )
    , bDecStateUpdated_     ( false )
    , rDominationState_     ( 0. )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::~DEC_Knowledge_Population()
{
    SendMsgDestruction();
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::load
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::load( MIL_CheckPointInArchive& file, const unsigned int )
{
    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this )
         >> const_cast< MIL_KnowledgeGroup*& >( pKnowledgeGroup_ )
         >> pPopulationKnown_
         >> const_cast< unsigned int& >( nID_ )
         >> concentrations_
         >> flows_
         >> bIsRecon_
         >> bReconAttributesValid_
         >> rDominationState_;

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
         << pKnowledgeGroup_
         << pPopulationKnown_
         << nID_
         << concentrations_
         << flows_
         << bIsRecon_
         << bReconAttributesValid_
         << rDominationState_;
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Prepare()
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->Prepare();

    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
        it->second->Prepare();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::UpdateReconAttributes
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateReconAttributes()
{
    if( !IsRecon() )
        return;

    assert( pPopulationKnown_ );

    bReconAttributesValid_ = true;
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
// Name: DEC_Knowledge_Population::UpdateRelevance
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateRelevance()
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->UpdateRelevance();

    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
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
    return concentrations_.empty() && flows_.empty();
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
void DEC_Knowledge_Population::Exterminate( const MIL_AgentPion& exterminator, MT_Float rSurface )
{
    assert( pPopulationKnown_ );
    pPopulationKnown_->Exterminate( exterminator, rSurface );
}

// =============================================================================
// TOOLS
// =============================================================================

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
MT_Float DEC_Knowledge_Population::GetDangerosity( const MIL_AgentPion& target ) const
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
MT_Vector2D DEC_Knowledge_Population::GetSafetyPosition( const MIL_AgentPion& agent, MT_Float rMinDistance ) const
{
    assert( pPopulationKnown_ );
    return pPopulationKnown_->GetSafetyPosition( agent, rMinDistance );
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::SendMsgCreation
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::SendMsgCreation() const
{
    assert( pKnowledgeGroup_ );
    assert( pPopulationKnown_ );

    client::PopulationKnowledgeCreation asnMsg;

    asnMsg().set_oid_connaissance     ( nID_ );
    asnMsg().set_oid_groupe_possesseur( pKnowledgeGroup_ ->GetID() );
    asnMsg().set_oid_population_reelle( pPopulationKnown_->GetID() );
    asnMsg().set_camp                 ( GetArmy()         .GetID() );
    asnMsg.Send( NET_Publisher_ABC::Publisher() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::SendMsgDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::SendMsgDestruction() const
{
    if( pKnowledgeGroup_ )
    {
        client::PopulationKnowledgeDestruction asnMsg;

        asnMsg().set_oid_connaissance     ( nID_ );
        asnMsg().set_oid_groupe_possesseur( pKnowledgeGroup_ ->GetID() );
        asnMsg.Send( NET_Publisher_ABC::Publisher() );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateOnNetwork() const
{
    if( bReconAttributesValid_ && bDecStateUpdated_ )
    {
        client::PopulationKnowledgeUpdate asnMsg;
        asnMsg().set_oid_connaissance     ( nID_ );
        asnMsg().set_oid_groupe_possesseur( pKnowledgeGroup_ ->GetID() );
        asnMsg().set_etat_domination       ( (unsigned int)( rDominationState_ * 100. ) );
        asnMsg.Send( NET_Publisher_ABC::Publisher() );
    }

    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->UpdateOnNetwork();

    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
        it->second->UpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::SendStateToNewClient() const
{
    SendMsgCreation();

    if( bReconAttributesValid_ )
    {
        client::PopulationKnowledgeUpdate asnMsg;
        asnMsg().set_oid_connaissance     ( nID_ );
        asnMsg().set_oid_groupe_possesseur( pKnowledgeGroup_ ->GetID() );
        asnMsg().set_etat_domination      ( (unsigned int)( rDominationState_ * 100. ) );
        asnMsg.Send( NET_Publisher_ABC::Publisher() );
    }

    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->SendStateToNewClient();

    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
        it->second->SendStateToNewClient();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::GetDominationState
// Created: NLD 2006-02-22
// -----------------------------------------------------------------------------
MT_Float DEC_Knowledge_Population::GetDominationState() const
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
// Name: DEC_Knowledge_Population::GetKnowledgeGroup
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
const MIL_KnowledgeGroup& DEC_Knowledge_Population::GetKnowledgeGroup() const
{
    assert( pKnowledgeGroup_ );
    return *pKnowledgeGroup_;
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
