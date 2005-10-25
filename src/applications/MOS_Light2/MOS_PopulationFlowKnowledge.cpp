//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_PopulationFlowKnowledge.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:45 $
// $Revision: 8 $
// $Workfile: MOS_PopulationFlowKnowledge.cpp $
//
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_PopulationFlowKnowledge.h"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Population.h"
#include "MOS_PopulationFlow.h"
#include "MOS_PopulationKnowledge.h"
#include "MOS_AgentManager.h"

// =============================================================================
// FlowPart
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::FlowPart::FlowPart
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
MOS_PopulationFlowKnowledge::FlowPart::FlowPart( ASN1T_PortionFlux& asn )
: flowPart_   ()
, rRelevance_ ( asn.pertinence )
{
    for( uint i = 0; i < asn.forme.vecteur_point.n; ++i )
	{
		MT_Vector2D point;
        MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asn.forme.vecteur_point.elem[ i ].data, point );
        flowPart_.push_back( point );
	}
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::FlowPart::~FlowPart
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
MOS_PopulationFlowKnowledge::FlowPart::~FlowPart()
{
    flowPart_.clear();
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::FlowPart::GetPart
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
const T_PointVector& MOS_PopulationFlowKnowledge::FlowPart::GetPart() const
{
    return flowPart_;
}
    
// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::FlowPart::GetRelevance
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
const MT_Float MOS_PopulationFlowKnowledge::FlowPart::GetRelevance() const
{
    return rRelevance_;
}


// =============================================================================
// MOS_PopulationFlowKnowledge
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::MOS_PopulationFlowKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
MOS_PopulationFlowKnowledge::MOS_PopulationFlowKnowledge( const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg )
    : nID_                 ( asnMsg.oid_connaissance_flux )
    , pGtia_               ( MOS_App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur ) )
    , pPopulationKnowledge_( 0 )
    , pFlow_               ( 0 )
    , rDirection_          ( 0. )
    , rSpeed_              ( 0. )
    , nNbrAliveHumans_     ( 0 )
    , nNbrDeadHumans_      ( 0 )
    , eAttitude_           ( ePopulationAttitude_Calme )
    , bIsPerceived_        ( false )
    , rRelevance_          ( 0. )
{
    assert( pGtia_ );
    pPopulationKnowledge_ = pGtia_->FindPopulationKnowledge( asnMsg.oid_connaissance_population );
    assert( pPopulationKnowledge_ );
    const MOS_Population& population = pPopulationKnowledge_->GetPopulation();
    pFlow_ = population.FindFlow( asnMsg.oid_flux_reel );
}



// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::~MOS_PopulationFlowKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
MOS_PopulationFlowKnowledge::~MOS_PopulationFlowKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationFlowKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationFlowKnowledge::Update( const ASN1T_MsgPopulationFluxKnowledgeUpdate& asnMsg )
{
    if( asnMsg.m.attitudePresent )
        eAttitude_ = ( E_PopulationAttitude )asnMsg.attitude;
    if( asnMsg.m.directionPresent )
        rDirection_ = asnMsg.direction;
    if( asnMsg.m.vitessePresent )
        rSpeed_ = asnMsg.vitesse;
    if( asnMsg.m.est_percuPresent )
        bIsPerceived_ = asnMsg.est_percu;
    if( asnMsg.m.nb_humains_vivantsPresent )
        nNbrAliveHumans_ = asnMsg.nb_humains_vivants;
    if( asnMsg.m.nb_humains_mortsPresent )
        nNbrDeadHumans_ = asnMsg.nb_humains_morts;
    if( asnMsg.m.oid_flux_reelPresent )
    {
        assert( pPopulationKnowledge_ );
        const MOS_Population& population = pPopulationKnowledge_->GetPopulation();
        pFlow_ = population.FindFlow( asnMsg.oid_flux_reel );
    }
    if( asnMsg.m.portions_fluxPresent )
    {
        for( uint i = 0; i < asnMsg.portions_flux.n; ++i )
            flowParts_.push_back( new FlowPart( asnMsg.portions_flux.elem[ i ] ) );
    }
//    if( asnMsg.m.pertinencePresent )
//        rRelevance_ = asnMsg.pertinence;
}
