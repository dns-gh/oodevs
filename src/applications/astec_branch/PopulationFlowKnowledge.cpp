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
#include "Model.h"
#include "TeamsModel.h"
#include "KnowledgeGroupsModel.h"
#include "Controller.h"
#include "Displayer_ABC.h"
#include "Units.h"

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
PopulationFlowKnowledge::PopulationFlowKnowledge( Controller& controller, const Population& popu, const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg )
    : controller_( controller )
    , popu_( popu )
    , nID_                 ( asnMsg.oid_connaissance_flux )
    , pFlow_               ( 0 )
{
    pFlow_ = popu_.FindFlow( asnMsg.oid_flux_reel );
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
        pFlow_ = popu_.FindFlow( asnMsg.oid_flux_reel );
    if( asnMsg.m.portions_fluxPresent )
    {
        flowParts_.clear(); flowParts_.reserve( asnMsg.portions_flux.n );
        for( uint i = 0; i < asnMsg.portions_flux.n; ++i )
            flowParts_.push_back( FlowPart( asnMsg.portions_flux.elem[ i ] ) );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::Display
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( "Flux" )
                .Display( "Id:", nID_ )
                .Display( "Flux associée:", pFlow_ )
                .Display( "Direction:", rDirection_ * Units::degrees )
                .Display( "Vitesse:", rSpeed_ * Units::kilometersPerHour )
                .Display( "Humains vivants:", nNbrAliveHumans_ )
                .Display( "Humains morts:", nNbrDeadHumans_ )
                .Display( "Attitude:", eAttitude_ )
                .Display( "Percue:", bIsPerceived_ )
                .Display( "Portions connues:", flowParts_.size() );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::DisplayInList
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    displayer.Display( "Populations connues" ).Start( "Flow - " ).Add( nID_ ).End();
}
