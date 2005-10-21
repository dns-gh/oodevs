//*****************************************************************************
// 
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_PopulationKnowledge.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:45 $
// $Revision: 8 $
// $Workfile: MOS_PopulationKnowledge.cpp $
// 
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_PopulationKnowledge.h"

#include "MOS_App.h"
#include "MOS_Gtia.h"
#include "MOS_Population.h"
#include "MOS_AgentManager.h"
#include "MOS_PopulationConcentrationKnowledge.h"

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledge::MOS_PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
MOS_PopulationKnowledge::MOS_PopulationKnowledge( const ASN1T_MsgPopulationKnowledgeCreation& asnMsg )
    : nID_        ( asnMsg.oid_connaissance )
    , pGtia_      ( MOS_App::GetApp().GetAgentManager().FindGtia      ( asnMsg.oid_groupe_possesseur ) )
    , pTeam_      ( MOS_App::GetApp().GetAgentManager().FindTeam      ( asnMsg.camp                  ) )
    , pPopulation_( MOS_App::GetApp().GetAgentManager().FindPopulation( asnMsg.oid_population_reelle ) )
{
    assert( pGtia_ );
    assert( pTeam_ );
    assert( pPopulation_ );
}



// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledge::~MOS_PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
MOS_PopulationKnowledge::~MOS_PopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledge::Update( const ASN1T_MsgPopulationKnowledgeUpdate& /*asnMsg*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledge::GetPopulation
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
const MOS_Population& MOS_PopulationKnowledge::GetPopulation() const
{
    assert( pPopulation_ );
    return *pPopulation_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledge::GetTeam
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
const MOS_Team* MOS_PopulationKnowledge::GetTeam() const
{
    return pTeam_;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledge::OnReceiveMsgPopulationConcentrationKnowledgeCreation
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledge::OnReceiveMsgPopulationConcentrationKnowledgeCreation( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg )
{
    if( concentrations_.find( asnMsg.oid_connaissance_concentration ) != concentrations_.end() )
        return;
    MOS_PopulationConcentrationKnowledge* pKnowledge = new MOS_PopulationConcentrationKnowledge( asnMsg );
    assert( pKnowledge );
    concentrations_.insert( std::make_pair( pKnowledge->GetID(), pKnowledge ) ).second;

    MOS_App::GetApp().NotifyPopulationConcentrationKnowledgeCreated( *pGtia_, *pKnowledge );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledge::OnReceiveMsgPopulationConcentrationKnowledgeUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledge::OnReceiveMsgPopulationConcentrationKnowledgeUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg )
{
    IT_ConcentrationKnowledgeMap it = concentrations_.find( asnMsg.oid_connaissance_concentration );
    assert( it != concentrations_.end() );
    it->second->Update( asnMsg );
    MOS_App::GetApp().NotifyPopulationConcentrationKnowledgeUpdated( *pGtia_, *( it->second ) );
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledge::OnReceiveMsgPopulationConcentrationKnowledgeDestruction
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationKnowledge::OnReceiveMsgPopulationConcentrationKnowledgeDestruction( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& asnMsg )
{
    IT_ConcentrationKnowledgeMap it = concentrations_.find( asnMsg.oid_connaissance_concentration );
    assert( it != concentrations_.end() );
    MOS_App::GetApp().NotifyPopulationConcentrationKnowledgeDeleted( *pGtia_, *( it->second ) );
    delete it->second;
    concentrations_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledge::GetConcentrations
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
const MOS_PopulationKnowledge::T_ConcentrationKnowledgeMap& MOS_PopulationKnowledge::GetConcentrations() const
{
    return concentrations_;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationKnowledge::GetGtia
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
MOS_Gtia* MOS_PopulationKnowledge::GetGtia() const
{
    return pGtia_;
}
