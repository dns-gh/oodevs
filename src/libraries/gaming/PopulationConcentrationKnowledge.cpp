//*****************************************************************************
//
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/PopulationConcentrationKnowledge.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:45 $
// $Revision: 8 $
// $Workfile: PopulationConcentrationKnowledge.cpp $
//
//*****************************************************************************

#include "gaming_pch.h"
#include "PopulationConcentrationKnowledge.h"
#include "PopulationConcentration.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationConcentration_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/Tools.h"
#include "protocol/Protocol.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::PopulationConcentrationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::PopulationConcentrationKnowledge( Controller& controller, const CoordinateConverter_ABC& converter, const Population_ABC& resolver, const sword::CrowdConcentrationKnowledgeCreation& message )
    : controller_     ( controller )
    , resolver_       ( resolver )
    , nID_            ( message.knowledge().id() )
    , concentrationId_( message.concentration().id() )
    , position_       ( converter.ConvertToXY( message.position() ) )
    , deadRadius_     ( 0 )
    , concentration_  ( 0 )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::~PopulationConcentrationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::~PopulationConcentrationKnowledge()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::DoUpdate
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::DoUpdate( const sword::CrowdConcentrationKnowledgeUpdate& message )
{
    static const float oneOnpi = 1.f / std::acos( -1.f );
    if( message.has_attitude() )
        eAttitude_ = static_cast< E_PopulationAttitude >( message.attitude() );
    if( message.has_perceived( ) )
        bIsPerceived_ = message.perceived() != 0;
    if( message.has_alive() )
        nNbrAliveHumans_ = static_cast< unsigned int >( message.alive() );
    if( message.has_dead() )
        nNbrDeadHumans_ = static_cast< unsigned int >( message.dead() );
    if( message.has_concentration() )
    {
        concentrationId_ = message.concentration().id();
        if( !concentrationId_ )
        {
            concentration_ = 0;
            radius_ = boost::none;
        }
    }
    if( message.has_pertinence() )
        rRelevance_ = static_cast< float >( message.pertinence() );

    const PopulationConcentration* concentration = GetConcentration();
    const float density = concentration ? concentration->GetDensity() : 0.f;
    if( density > 0.f && nNbrAliveHumans_.IsSet() )
    {
        radius_     = std::sqrt( ( ( nNbrAliveHumans_ + nNbrDeadHumans_ ) / density ) * oneOnpi );
        deadRadius_ = std::sqrt( ( nNbrDeadHumans_ / density ) * oneOnpi );
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Display( Displayer_ABC& displayer ) const
{
    Displayer_ABC& group = displayer.Group( tools::translate( "Crowd", "Concentration" ) );
    group.Display( tools::translate( "Crowd", "Alive people:" ), nNbrAliveHumans_ )
         .Display( tools::translate( "Crowd", "Dead people:" ), nNbrDeadHumans_ )
         .Display( tools::translate( "Crowd", "Perceived:" ), bIsPerceived_ )
         .Display( tools::translate( "Crowd", "Relevance:" ), rRelevance_ );
    if( eAttitude_.IsSet() )
        group.Display( tools::translate( "Crowd", "Mood:" ), tools::ToString( eAttitude_ ) );
    else
        group.Display( tools::translate( "Crowd", "Mood:" ), ValueNotSet() );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::DisplayInList
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Crowd", "Known crowds" ) )
                .Start( tools::translate( "Crowd", "Concentration - " ) ).Add( nID_ ).End();
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
// Name: PopulationConcentrationKnowledge::Draw
// Created: SBO 2007-02-27
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Draw( const geometry::Point2f&, const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    if( !viewport.IsVisible( position_ ) )
        return;
    const float radius = GetRadius();
    if( GetConcentration() && radius > 0 ) // $$$$ SBO 2007-02-27: isPerceived?
    {
        float currentColor[ 4 ];
        glPushAttrib( GL_CURRENT_BIT );
        glGetFloatv( GL_CURRENT_COLOR, currentColor );
        currentColor[ 3 ] = 0.5f * ( 1.f + rRelevance_ * 0.01f );
        glColor4fv( currentColor );
        tools.DrawDisc( position_, radius );
        glColor4f( COLOR_BLACK );
        tools.DrawDisc( position_, deadRadius_ );
        SelectColor( eAttitude_, currentColor[ 3 ] );
        tools.DrawCircle( position_, radius );
        glPopAttrib();
    }
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Pick( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& /*viewport*/, gui::GlTools_ABC& tools ) const
{
    tools.DrawDisc( position_, GetRadius() );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetNId
// Created: NPT 2012-10-11
// -----------------------------------------------------------------------------
uint PopulationConcentrationKnowledge::GetNId() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetRadius
// Created: SLI 2013-11-28
// -----------------------------------------------------------------------------
float PopulationConcentrationKnowledge::GetRadius() const
{
    if( radius_ )
        return *radius_;
    if( const PopulationConcentration* concentration = GetConcentration() )
        return concentration->GetRadius();
    return 0;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetConcentration
// Created: SLI 2013-11-28
// -----------------------------------------------------------------------------
const PopulationConcentration* PopulationConcentrationKnowledge::GetConcentration() const
{
    if( !concentration_ )
        concentration_ = static_cast< const PopulationConcentration* >( resolver_.FindConcentration( concentrationId_ ) );
    return concentration_;
}
