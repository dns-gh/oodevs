//*****************************************************************************
// 
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationKnowledge.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:45 $
// $Revision: 8 $
// $Workfile: PopulationKnowledge.cpp $
// 
//*****************************************************************************

#include "astec_pch.h"
#include "PopulationKnowledge.h"

#include "App.h"
#include "Gtia.h"
#include "Population.h"
#include "AgentManager.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"
#include "TypePopulation.h"
#include "Model.h"

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( const ASN1T_MsgPopulationKnowledgeCreation& asnMsg )
    : nID_        ( asnMsg.oid_connaissance )
    , pGtia_      ( & App::GetApp().GetModel().GetGtia      ( asnMsg.oid_groupe_possesseur ) )
    , pTeam_      ( & App::GetApp().GetModel().GetTeam      ( asnMsg.camp                  ) )
    , pPopulation_( & App::GetApp().GetModel().GetPopulation( asnMsg.oid_population_reelle ) )
    , pType_      ( &pPopulation_->GetType() )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::~PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::~PopulationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationKnowledgeUpdate& /*asnMsg*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetPopulation
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
const Population& PopulationKnowledge::GetPopulation() const
{
    assert( pPopulation_ );
    return *pPopulation_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetTeam
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
const Team* PopulationKnowledge::GetTeam() const
{
    return pTeam_;
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg )
{
    if( concentrations_.find( asnMsg.oid_connaissance_concentration ) != concentrations_.end() )
        return;
    PopulationConcentrationKnowledge* pKnowledge = new PopulationConcentrationKnowledge( asnMsg );
    assert( pKnowledge );
    concentrations_.insert( std::make_pair( pKnowledge->GetID(), pKnowledge ) );

//    App::GetApp().NotifyPopulationConcentrationKnowledgeCreated( *pGtia_, *pKnowledge );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg )
{
    IT_ConcentrationKnowledgeMap it = concentrations_.find( asnMsg.oid_connaissance_concentration );
    assert( it != concentrations_.end() );
    it->second->Update( asnMsg );
//    App::GetApp().NotifyPopulationConcentrationKnowledgeUpdated( *pGtia_, *( it->second ) );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& asnMsg )
{
    IT_ConcentrationKnowledgeMap it = concentrations_.find( asnMsg.oid_connaissance_concentration );
    assert( it != concentrations_.end() );
//    App::GetApp().NotifyPopulationConcentrationKnowledgeDeleted( *pGtia_, *( it->second ) );
    delete it->second;
    concentrations_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg )
{
    if( flows_.find( asnMsg.oid_connaissance_flux ) != flows_.end() )
        return;
    PopulationFlowKnowledge* pKnowledge = new PopulationFlowKnowledge( asnMsg );
    assert( pKnowledge );
    flows_.insert( std::make_pair( pKnowledge->GetID(), pKnowledge ) );

//    App::GetApp().NotifyPopulationFlowKnowledgeCreated( *pGtia_, *pKnowledge );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& asnMsg )
{
    IT_FlowKnowledgeMap it = flows_.find( asnMsg.oid_connaissance_flux );
    assert( it != flows_.end() );
    it->second->Update( asnMsg );
//    App::GetApp().NotifyPopulationFlowKnowledgeUpdated( *pGtia_, *( it->second ) );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeDestruction& asnMsg )
{
    IT_FlowKnowledgeMap it = flows_.find( asnMsg.oid_connaissance_flux );
    assert( it != flows_.end() );
//    App::GetApp().NotifyPopulationFlowKnowledgeDeleted( *pGtia_, *( it->second ) );
    delete it->second;
    flows_.erase( it );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetConcentrations
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
const PopulationKnowledge::T_ConcentrationKnowledgeMap& PopulationKnowledge::GetConcentrations() const
{
    return concentrations_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetFlows
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
const PopulationKnowledge::T_FlowKnowledgeMap& PopulationKnowledge::GetFlows() const
{
    return flows_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetGtia
// Created: SBO 2005-10-19
// -----------------------------------------------------------------------------
Gtia* PopulationKnowledge::GetGtia() const
{
    return pGtia_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetType
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
const TypePopulation& PopulationKnowledge::GetType() const
{
    assert( pType_ );
    return *pType_;
}
