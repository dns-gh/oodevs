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

#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/PopulationConcentration_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/PopulationKnowledge_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::PopulationConcentrationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::PopulationConcentrationKnowledge( Controller& controller, const CoordinateConverter_ABC& converter, const Population_ABC& resolver, const MsgsSimToClient::MsgPopulationConcentrationKnowledgeCreation& message )
    : controller_     ( controller )
    , resolver_       ( resolver )
    , nID_            ( message.oid_connaissance_concentration() )
    , concentrationId_( message.oid_concentration_reelle() )
    , position_       ( converter.ConvertToXY( message.position() ) )
    , radius_         ( 100.f )
    , deadRadius_     ( 0 )
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
void PopulationConcentrationKnowledge::DoUpdate( const MsgsSimToClient::MsgPopulationConcentrationKnowledgeUpdate& message )
{
    static const float oneOnpi = 1.f / std::acos( -1.f );
    if( message.has_attitude()  )
        eAttitude_ = ( E_PopulationAttitude )message.attitude();
    if( message.has_est_percu()  )
        bIsPerceived_ = message.est_percu() != 0;
    if( message.has_nb_humains_vivants()  )
        nNbrAliveHumans_ = ( uint )message.nb_humains_vivants();
    if( message.has_nb_humains_morts()  )
        nNbrDeadHumans_ = ( uint )message.nb_humains_morts();
    if( message.has_oid_concentration_reelle()  )
        concentrationId_ = message.oid_concentration_reelle();
    if( message.has_pertinence()  )
        rRelevance_ = float( message.pertinence() );

    const kernel::PopulationConcentration_ABC* concentration = resolver_.FindConcentration( concentrationId_ );
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
    Displayer_ABC& group = displayer.Group( tools::translate( "Population", "Concentration" ) );
    group.Display( tools::translate( "Population", "Alive people:" ), nNbrAliveHumans_ )
         .Display( tools::translate( "Population", "Dead people:" ), nNbrDeadHumans_ )
         .Display( tools::translate( "Population", "Perceived:" ), bIsPerceived_ )
         .Display( tools::translate( "Population", "Relevance:" ), rRelevance_ );
    if( eAttitude_.IsSet() )
        group.Display( tools::translate( "Population", "Mood:" ), tools::ToString( eAttitude_ ) );
    else
        group.Display( tools::translate( "Population", "Mood:" ), ValueNotSet() );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::DisplayInList
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Population", "Known populations" ) )
                .Start( tools::translate( "Population", "Concentration - " ) ).Add( nID_ ).End();
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
// Name: PopulationConcentrationKnowledge::Draw
// Created: SBO 2007-02-27
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Draw( const geometry::Point2f&, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !viewport.IsVisible( position_ ) )
        return;
    if( resolver_.FindConcentration( concentrationId_ ) ) // $$$$ SBO 2007-02-27: isPerceived?
    {
        float currentColor[4];
        glPushAttrib( GL_CURRENT_BIT );
        glGetFloatv( GL_CURRENT_COLOR, currentColor );
        currentColor[3] = 0.5f * ( 1.f + rRelevance_ * 0.01f );
        glColor4fv( currentColor );
        tools.DrawDisc( position_, radius_ );
        glColor4f( COLOR_BLACK );
        tools.DrawDisc( position_, deadRadius_ );
        SelectColor( eAttitude_ );
        tools.DrawCircle( position_, radius_ );
        glPopAttrib();
    }
}
