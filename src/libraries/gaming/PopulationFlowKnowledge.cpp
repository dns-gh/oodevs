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

#include "gaming_pch.h"
#include "PopulationFlowKnowledge.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "PopulationFlow.h"
#include "PopulationKnowledge.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"

// =============================================================================
// FlowPart
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::FlowPart::FlowPart
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::FlowPart::FlowPart( ASN1T_PortionFlux& asn, const CoordinateConverter_ABC& converter )
     : rRelevance_ ( asn.pertinence )
{
    for( uint i = 0; i < asn.forme.vecteur_point.n; ++i )
        flowPart_.push_back( converter.ConvertToXY( asn.forme.vecteur_point.elem[ i ] ) );
}

// =============================================================================
// PopulationFlowKnowledge
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::PopulationFlowKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::PopulationFlowKnowledge( Controller& controller, const CoordinateConverter_ABC& converter, const Population_ABC& popu, const ASN1T_MsgPopulationFluxKnowledgeCreation& asnMsg )
    : controller_( controller )
    , converter_ ( converter )
    , popu_      ( popu )
    , nID_       ( asnMsg.oid_connaissance_flux )
    , pFlow_     ( 0 )
{
    pFlow_ = popu_.FindFlow( asnMsg.oid_flux_reel );
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::~PopulationFlowKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::~PopulationFlowKnowledge()
{
    controller_.Delete( *this );
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
        rDirection_ = ( float )asnMsg.direction;
    if( asnMsg.m.vitessePresent )
        rSpeed_ = ( float )asnMsg.vitesse;
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
            flowParts_.push_back( FlowPart( asnMsg.portions_flux.elem[ i ], converter_ ) );
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
