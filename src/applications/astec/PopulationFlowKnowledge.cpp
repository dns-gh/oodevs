//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationFlowKnowledge.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:45 $
// $Revision: 8 $
// $Workfile: PopulationFlowKnowledge.cpp $
//
//*****************************************************************************

#include "astec_pch.h"
#include "PopulationFlowKnowledge.h"

#include "App.h"
#include "World.h"
#include "Population.h"
#include "PopulationFlow.h"
#include "PopulationKnowledge.h"
#include "AgentManager.h"

// =============================================================================
// FlowPart
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::FlowPart::FlowPart
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::FlowPart::FlowPart( ASN1T_PortionFlux& asn )
     : rRelevance_ ( asn.pertinence )
{
    for( uint i = 0; i < asn.forme.vecteur_point.n; ++i )
	{
		MT_Vector2D point;
        App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asn.forme.vecteur_point.elem[ i ].data, point );
        flowPart_.push_back( point );
	}
}

// =============================================================================
// PopulationFlowKnowledge
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::PopulationFlowKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::PopulationFlowKnowledge( const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg )
    : nID_                 ( asnMsg.oid_connaissance_flux )
    , pGtia_               ( App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur ) )
    , pPopulationKnowledge_( 0 )
    , pFlow_               ( 0 )
{
    assert( pGtia_ );
    pPopulationKnowledge_ = pGtia_->FindPopulationKnowledge( asnMsg.oid_connaissance_population );
    assert( pPopulationKnowledge_ );
    const Population& population = pPopulationKnowledge_->GetPopulation();
    
    if( asnMsg.oid_flux_reel == 0 )
        pFlow_ = 0;
    else
    {
        pFlow_ = population.FindFlow( asnMsg.oid_flux_reel );
        assert( pFlow_ );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::~PopulationFlowKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::~PopulationFlowKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& asnMsg )
{
    if( asnMsg.m.attitudePresent )
        eAttitude_ = ( E_PopulationAttitude )asnMsg.attitude;
    if( asnMsg.m.directionPresent )
        rDirection_ = ( MT_Float )asnMsg.direction;
    if( asnMsg.m.vitessePresent )
        rSpeed_ = ( MT_Float )asnMsg.vitesse;
    if( asnMsg.m.est_percuPresent )
        bIsPerceived_ = ( bool )asnMsg.est_percu;
    if( asnMsg.m.nb_humains_vivantsPresent )
        nNbrAliveHumans_ = ( uint )asnMsg.nb_humains_vivants;
    if( asnMsg.m.nb_humains_mortsPresent )
        nNbrDeadHumans_ = ( uint )asnMsg.nb_humains_morts;
    if( asnMsg.m.oid_flux_reelPresent )
    {
        assert( pPopulationKnowledge_ );
        const Population& population = pPopulationKnowledge_->GetPopulation();
        if( asnMsg.oid_flux_reel == 0 )
            pFlow_ = 0;
        else
        {
            pFlow_ = population.FindFlow( asnMsg.oid_flux_reel );
            assert( pFlow_ );
        }
    }
    if( asnMsg.m.portions_fluxPresent )
    {
        flowParts_.Set();
        for( uint i = 0; i < asnMsg.portions_flux.n; ++i )
            flowParts_.Data().push_back( FlowPart( asnMsg.portions_flux.elem[ i ] ) );
    }
}
