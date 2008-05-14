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
#include "clients_kernel/PopulationFlow_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Viewport_ABC.h"
#include "Tools.h"
#include "PopulationKnowledge_ABC.h"

using namespace kernel;

// =============================================================================
// FlowPart
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::FlowPart::FlowPart
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::FlowPart::FlowPart( ASN1T_FlowPart& asn, const CoordinateConverter_ABC& converter, geometry::Rectangle2f& boundingBox )
     : rRelevance_ ( asn.pertinence )
{
    for( uint i = 0; i < asn.forme.coordinates.n; ++i )
    {
        const geometry::Point2f point = converter.ConvertToXY( asn.forme.coordinates.elem[ i ] );
        flowPart_.push_back( point );
        boundingBox.Incorporate( point );
    }
}

// =============================================================================
// PopulationFlowKnowledge
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::PopulationFlowKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::PopulationFlowKnowledge( Controller& controller, const CoordinateConverter_ABC& converter, const Population_ABC& popu, const ASN1T_MsgPopulationFlowKnowledgeCreation& asnMsg )
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
// Name: PopulationFlowKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::DoUpdate( const ASN1T_MsgPopulationFlowKnowledgeUpdate& asnMsg )
{
    if( asnMsg.m.attitudePresent )
        eAttitude_ = ( E_PopulationAttitude )asnMsg.attitude;
    if( asnMsg.m.directionPresent )
        rDirection_ = ( float )asnMsg.direction;
    if( asnMsg.m.vitessePresent )
        rSpeed_ = ( float )asnMsg.vitesse;
    if( asnMsg.m.est_percuPresent )
        bIsPerceived_ = asnMsg.est_percu != 0;
    if( asnMsg.m.nb_humains_vivantsPresent )
        nNbrAliveHumans_ = ( uint )asnMsg.nb_humains_vivants;
    if( asnMsg.m.nb_humains_mortsPresent )
        nNbrDeadHumans_ = ( uint )asnMsg.nb_humains_morts;
    if( asnMsg.m.oid_flux_reelPresent )
        pFlow_ = popu_.FindFlow( asnMsg.oid_flux_reel );
    if( asnMsg.m.portions_fluxPresent )
    {
        boundingBox_ = geometry::Rectangle2f();
        flowParts_.clear(); flowParts_.reserve( asnMsg.portions_flux.n );
        for( uint i = 0; i < asnMsg.portions_flux.n; ++i )
            flowParts_.push_back( FlowPart( asnMsg.portions_flux.elem[ i ], converter_, boundingBox_ ) );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::Display
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Population", "Flow" ) )
                .Display( tools::translate( "Population", "Identifier:" ), nID_ )
                .Display( tools::translate( "Population", "Associated flow:" ), pFlow_ )
                .Display( tools::translate( "Population", "Heading:" ), rDirection_ * Units::degrees )
                .Display( tools::translate( "Population", "Speed:" ), rSpeed_ * Units::kilometersPerHour )
                .Display( tools::translate( "Population", "Alive people:" ), nNbrAliveHumans_ )
                .Display( tools::translate( "Population", "Dead people:" ), nNbrDeadHumans_ )
                .Display( tools::translate( "Population", "Mood:" ), eAttitude_ )
                .Display( tools::translate( "Population", "Perceived:" ), bIsPerceived_ )
                .Display( tools::translate( "Population", "Known chunks:" ), flowParts_.size() );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::DisplayInList
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Population", "Known populations" ) ).Start( tools::translate( "Population", "Flow - " ) ).Add( nID_ ).End();
}

namespace
{
    void SelectColor( E_PopulationAttitude attitude )
    {
        if( attitude == ePopulationAttitude_Agressive )
            glColor4f( COLOR_POPULATION_ATTITUDE_AGRESSIVE );
        else if( attitude == ePopulationAttitude_Excitee )
            glColor4f( COLOR_POPULATION_ATTITUDE_EXCITED );
        else if( attitude == ePopulationAttitude_Agitee )
            glColor4f( COLOR_POPULATION_ATTITUDE_AGITATED );
        else // ePopulationAttitude_Calme
            glColor4f( COLOR_POPULATION_ATTITUDE_CALM );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::Draw
// Created: SBO 2007-02-27
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !viewport.IsVisible( boundingBox_ ) )
        return;
    if( pFlow_ )
    {
        glPushAttrib( GL_CURRENT_BIT | GL_LINE_BIT );
        float color[4];
        glGetFloatv( GL_CURRENT_COLOR, color );
        for( unsigned int i = 0; i < flowParts_.size(); ++i )
        {
            const FlowPart& part = flowParts_[i];
            SelectColor( eAttitude_ );
            glLineWidth( 10.f );
            tools.DrawLines( part.flowPart_ );
            glColor4f( color[0], color[1], color[2], 0.5f * ( 1.f + part.rRelevance_ * 0.01f ) );
            glLineWidth( 8.f );
            tools.DrawLines( part.flowPart_ );
        }
        glPopAttrib();
    }
}
