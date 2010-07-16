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
#include "clients_kernel/PopulationKnowledge_ABC.h"

using namespace kernel;

// =============================================================================
// FlowPart
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::FlowPart::FlowPart
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::FlowPart::FlowPart( const MsgsSimToClient::MsgFlowPart& message, const CoordinateConverter_ABC& converter, geometry::Rectangle2f& boundingBox )
     : relevance_ ( unsigned short( message.pertinence() ) )
{
    for( int i = 0; i < message.forme().location().coordinates().elem_size(); ++i )
    {
        const geometry::Point2f point = converter.ConvertToXY( message.forme().location().coordinates().elem( i ) );
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
PopulationFlowKnowledge::PopulationFlowKnowledge( Controller& controller, const CoordinateConverter_ABC& converter, const Population_ABC& popu, const MsgsSimToClient::MsgPopulationFlowKnowledgeCreation& message )
    : controller_( controller )
    , converter_ ( converter )
    , popu_      ( popu )
    , nID_       ( message.oid_connaissance_flux() )
    , pFlow_     ( 0 )
{
    pFlow_ = popu_.FindFlow( message.oid_flux_reel() );
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
void PopulationFlowKnowledge::DoUpdate( const MsgsSimToClient::MsgPopulationFlowKnowledgeUpdate& message )
{
    if( message.has_attitude()  )
        eAttitude_ = ( E_PopulationAttitude )message.attitude();
    if( message.has_direction()  )
        rDirection_ = ( float )message.direction().heading();
    if( message.has_vitesse()  )
        rSpeed_ = ( float )message.vitesse();
    if( message.has_est_percu()  )
        bIsPerceived_ = message.est_percu() != 0;
    if( message.has_nb_humains_vivants()  )
        nNbrAliveHumans_ = ( uint )message.nb_humains_vivants();
    if( message.has_nb_humains_morts()  )
        nNbrDeadHumans_ = ( uint )message.nb_humains_morts();
    if( message.has_oid_flux_reel()  )
        pFlow_ = popu_.FindFlow( message.oid_flux_reel() );
    if( message.has_portions_flux()  )
    {
        boundingBox_ = geometry::Rectangle2f();
        flowParts_.clear(); flowParts_.reserve( message.portions_flux().elem_size() );
        for( int i = 0; i < message.portions_flux().elem_size(); ++i )
            flowParts_.push_back( FlowPart( message.portions_flux().elem( i ), converter_, boundingBox_ ) );
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
                .Display( tools::translate( "Population", "Heading:" ), rDirection_ * Units::degrees )
                .Display( tools::translate( "Population", "Speed:" ), rSpeed_ * Units::kilometersPerHour )
                .Display( tools::translate( "Population", "Alive people:" ), nNbrAliveHumans_ )
                .Display( tools::translate( "Population", "Dead people:" ), nNbrDeadHumans_ )
                .Display( tools::translate( "Population", "Mood:" ), tools::ToString( eAttitude_ ) )
                .Display( tools::translate( "Population", "Perceived:" ), bIsPerceived_ );
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
            glColor4f( color[0], color[1], color[2], 0.5f * ( 1.f + part.relevance_ * 0.01f ) );
            glLineWidth( 8.f );
            tools.DrawLines( part.flowPart_ );
        }
        glPopAttrib();
    }
}
