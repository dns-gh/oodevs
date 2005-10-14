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

#include "MIL_pch.h"
#include "DEC_Knowledge_Population.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_PopulationConcentration.h"
#include "DEC_Knowledge_PopulationFlow.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_Knowledge_PopulationConcentrationPerception.h"
#include "DEC_Knowledge_PopulationFlowPerception.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Populations/MIL_Population.h"
#include "Entities/Populations/MIL_PopulationConcentration.h"
#include "Entities/Populations/MIL_PopulationFlow.h"
#include "Entities/MIL_Army.h"
#include "Network/NET_ASN_Messages.h"

BOOST_CLASS_EXPORT_GUID( DEC_Knowledge_Population, "DEC_Knowledge_Population" )

MIL_MOSIDManager DEC_Knowledge_Population::idManager_;

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::DEC_Knowledge_Population( const MIL_KnowledgeGroup& knowledgeGroup, MIL_Population& populationKnown )
    : DEC_Knowledge_ABC()
    , nID_             ( idManager_.GetFreeSimID() )
    , pKnowledgeGroup_ ( &knowledgeGroup )
    , pPopulationKnown_( &populationKnown )
    , concentrations_  ()
    , flows_           ()
    , pArmy_           ( &populationKnown.GetArmy() )
{
    SendMsgCreation();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population constructor
// Created: JVT 2005-03-17
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::DEC_Knowledge_Population()
    : DEC_Knowledge_ABC()
    , nID_             ( 0 )
    , pKnowledgeGroup_ ( 0 )
    , pPopulationKnown_( 0 )
    , concentrations_  ()
    , flows_           ()
    , pArmy_           ( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_Knowledge_Population::~DEC_Knowledge_Population()
{
    SendMsgDestruction();
    idManager_.ReleaseSimID( nID_ );
}

// =============================================================================
// CHECKPOINTS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::load
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::load( MIL_CheckPointInArchive& file, const uint )
{
    assert( false );
//    file >> boost::serialization::base_object< DEC_Knowledge_ABC >( *this );
//
//    file >> const_cast< MIL_AgentPion*& >( pAgentPerceiving_ ) 
//         >> pPopulationPerceived_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::save
// Created: JVT 2005-03-23
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::save( MIL_CheckPointOutArchive& file, const uint ) const
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
// Name: DEC_Knowledge_Population::Prepare
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Prepare()
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->Prepare();

//    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
//        it->second->Prepare();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Update
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::Update( const DEC_Knowledge_PopulationPerception& perception )
{
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
//    GetKnowledge( perception.GetFlowPerceived() ).Update( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::UpdateRelevance
// Created: NLD 2005-10-13
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateRelevance()
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->UpdateRelevance();

//    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
//        it->second->UpdateRelevance(); 
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::Clean
// Created: NLD 2005-10-12
// -----------------------------------------------------------------------------
bool DEC_Knowledge_Population::Clean()
{
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

//    for( IT_FlowMap it = flows_.begin(); it != flows_.end(); )
//    {
//        DEC_Knowledge_PopulationFlow* pKnowledge = it->second;
//        if( pKnowledge->Clean() )
//        {
//            delete pKnowledge;
//            it = flows_.erase( it );
//        }
//        else 
//            ++ it;
//    }

    return concentrations_.empty() && flows_.empty();
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
//    if( !pKnowledge )
//        pKnowledge = new DEC_Knowledge_PopulationFlow( *this, flow );
    return *pKnowledge;
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
    assert( pArmy_ );

    NET_ASN_MsgPopulationKnowledgeCreation asnMsg;

    asnMsg.GetAsnMsg().oid_connaissance      = nID_;
    asnMsg.GetAsnMsg().oid_groupe_possesseur = pKnowledgeGroup_ ->GetID();
    asnMsg.GetAsnMsg().oid_population_reelle = pPopulationKnown_->GetID();    
    asnMsg.GetAsnMsg().camp                  = pArmy_           ->GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::SendMsgDestruction
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::SendMsgDestruction() const
{
    assert( pKnowledgeGroup_ );

    NET_ASN_MsgPopulationKnowledgeDestruction asnMsg;

    asnMsg.GetAsnMsg().oid_connaissance      = nID_;
    asnMsg.GetAsnMsg().oid_groupe_possesseur = pKnowledgeGroup_ ->GetID();
    asnMsg.Send();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::UpdateOnNetwork
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::UpdateOnNetwork() const
{
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->UpdateOnNetwork();

//    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
//        it->second->UpdateOnNetwork();
}

// -----------------------------------------------------------------------------
// Name: DEC_Knowledge_Population::SendStateToNewClient
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_Knowledge_Population::SendStateToNewClient() const
{
    SendMsgCreation();
    for( CIT_ConcentrationMap it = concentrations_.begin(); it != concentrations_.end(); ++it )
        it->second->SendStateToNewClient();

//    for( CIT_FlowMap it = flows_.begin(); it != flows_.end(); ++it )
//        it->second->SendStateToNewClient();
}
