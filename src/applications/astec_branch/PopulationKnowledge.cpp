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
#include "KnowledgeGroup.h"
#include "Population.h"
#include "AgentManager.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationFlowKnowledge.h"
#include "TypePopulation.h"
#include "Model.h"
#include "TeamsModel.h"
#include "AgentsModel.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::PopulationKnowledge( Controller& controller, const ASN1T_MsgPopulationKnowledgeCreation& message )
    : controller_( controller )
    , nID_        ( message.oid_connaissance )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::~PopulationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationKnowledge::~PopulationKnowledge()
{
    Resolver< PopulationConcentrationKnowledge >::DeleteAll();
    Resolver< PopulationFlowKnowledge >::DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetId
// Created: AGE 2006-02-15
// -----------------------------------------------------------------------------
unsigned long PopulationKnowledge::GetId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationKnowledgeUpdate& /*message*/ )
{
    // NOTHING
}

// =============================================================================
// NETWORK
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& message )
{
    if( ! Resolver< PopulationConcentrationKnowledge >::Find( message.oid_connaissance_concentration ) )
    {
        PopulationConcentrationKnowledge* pKnowledge = new PopulationConcentrationKnowledge( controller_, message );
        Resolver< PopulationConcentrationKnowledge >::Register( message.oid_connaissance_concentration, *pKnowledge );
    };
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& message )
{
    Resolver< PopulationConcentrationKnowledge >::
        Get( message.oid_connaissance_concentration ).Update( message );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeDestruction& message )
{
    delete Resolver< PopulationConcentrationKnowledge >::Find( message.oid_connaissance_concentration );
    Resolver< PopulationConcentrationKnowledge >::Remove( message.oid_connaissance_concentration );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeCreation& message )
{
    if( ! Resolver< PopulationFlowKnowledge >::Find( message.oid_connaissance_flux ) )
    {
        PopulationFlowKnowledge* pKnowledge = new PopulationFlowKnowledge( controller_, message );
        Resolver< PopulationFlowKnowledge >::Register( message.oid_connaissance_flux, *pKnowledge );
    };
    controller_.Update( *this );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& message )
{
    Resolver< PopulationFlowKnowledge >::
        Get( message.oid_connaissance_flux ).Update( message );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::Update
// Created: SBO 2005-10-21
// -----------------------------------------------------------------------------
void PopulationKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeDestruction& message )
{
    delete Resolver< PopulationFlowKnowledge >::Find( message.oid_connaissance_flux );
    Resolver< PopulationFlowKnowledge >::Remove( message.oid_connaissance_flux );
    controller_.Update( *this );
}