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
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/Units.h"
#include "clients_kernel/Tools.h"
#include "protocol/SimulationSenders.h"

using namespace kernel;

// =============================================================================
// FlowPart
// =============================================================================

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::FlowPart::FlowPart
// Created: SBO 2005-10-25
// -----------------------------------------------------------------------------
PopulationFlowKnowledge::FlowPart::FlowPart( const sword::FlowPart& message, const CoordinateConverter_ABC& converter, geometry::Rectangle2f& boundingBox )
     : relevance_ ( unsigned short( message.pertinence() ) )
{
    for( int i = 0; i < message.shape().location().coordinates().elem_size(); ++i )
    {
        const geometry::Point2f point = converter.ConvertToXY( message.shape().location().coordinates().elem( i ) );
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
PopulationFlowKnowledge::PopulationFlowKnowledge( Controller& controller, const CoordinateConverter_ABC& converter, const Population_ABC& popu, const sword::CrowdFlowKnowledgeCreation& message )
    : controller_( controller )
    , converter_ ( converter )
    , popu_      ( popu )
    , nID_       ( message.knowledge().id() )
    , pFlow_     ( 0 )
{
    pFlow_ = popu_.FindFlow( message.flow().id() );
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
void PopulationFlowKnowledge::DoUpdate( const sword::CrowdFlowKnowledgeUpdate& message )
{
    if( message.has_attitude()  )
        eAttitude_ = ( E_PopulationAttitude )message.attitude();
    if( message.has_direction()  )
        rDirection_ = ( float )message.direction().heading();
    if( message.has_speed()  )
        rSpeed_ = ( float )message.speed();
    if( message.has_perceived()  )
        bIsPerceived_ = message.perceived() != 0;
    if( message.has_alive()  )
        nNbrAliveHumans_ = ( uint )message.alive();
    if( message.has_dead()  )
        nNbrDeadHumans_ = ( uint )message.dead();
    if( message.has_flow()  )
        pFlow_ = popu_.FindFlow( message.flow().id() );
    if( message.has_parts()  )
    {
        boundingBox_ = geometry::Rectangle2f();
        flowParts_.clear(); flowParts_.reserve( message.parts().elem_size() );
        for( int i = 0; i < message.parts().elem_size(); ++i )
            flowParts_.push_back( FlowPart( message.parts().elem( i ), converter_, boundingBox_ ) );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::Display
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::Display( Displayer_ABC& displayer ) const
{
    Displayer_ABC& group = displayer.Group( tools::translate( "Crowd", "Flow" ) );
    group.Display( tools::translate( "Crowd", "Heading:" ), rDirection_ * Units::degrees )
         .Display( tools::translate( "Crowd", "Speed:" ), rSpeed_ * Units::kilometersPerHour )
         .Display( tools::translate( "Crowd", "Alive people:" ), nNbrAliveHumans_ )
         .Display( tools::translate( "Crowd", "Dead people:" ), nNbrDeadHumans_ )
         .Display( tools::translate( "Crowd", "Perceived:" ), bIsPerceived_ );
    if( eAttitude_.IsSet() )
        group.Display( tools::translate( "Crowd", "Mood:" ), tools::ToString( eAttitude_ ) );
    else
        group.Display( tools::translate( "Crowd", "Mood:" ), ValueNotSet() );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::DisplayInList
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Crowd", "Known crowds" ) ).Start( tools::translate( "Crowd", "Flow - " ) ).Add( nID_ ).End();
}

namespace
{
    void SelectColor( E_PopulationAttitude attitude, GLfloat alpha )
    {
        if( attitude == ePopulationAttitude_Agressive )
            glColor4f( COLOR_POPULATION_ATTITUDE_AGRESSIVE, alpha );
        else if( attitude == ePopulationAttitude_Excitee )
            glColor4f( COLOR_POPULATION_ATTITUDE_EXCITED, alpha );
        else if( attitude == ePopulationAttitude_Agitee )
            glColor4f( COLOR_POPULATION_ATTITUDE_AGITATED, alpha );
        else // ePopulationAttitude_Calme
            glColor4f( COLOR_POPULATION_ATTITUDE_CALM, alpha );
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::Draw
// Created: SBO 2007-02-27
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::Draw( const geometry::Point2f&, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
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
            if( !tools.IsPickingMode() )
                SelectColor( eAttitude_, color[ 3 ] );
            glLineWidth( 10.f );
            tools.DrawLines( part.flowPart_ );
            if( !tools.IsPickingMode() )
            {
                glColor4f( color[0], color[1], color[2], 0.5f * ( 1.f + part.relevance_ * 0.01f ) );
                glLineWidth( 8.f );
                tools.DrawLines( part.flowPart_ );
            }
        }
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void PopulationFlowKnowledge::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: PopulationFlowKnowledge::GetNId
// Created: NPT 2012-10-11
// -----------------------------------------------------------------------------
uint PopulationFlowKnowledge::GetNId() const
{
    return nID_;
}
