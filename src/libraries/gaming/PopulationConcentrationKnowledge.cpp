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
#include "PopulationKnowledge_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::PopulationConcentrationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::PopulationConcentrationKnowledge( Controller& controller, const CoordinateConverter_ABC& converter, const Population_ABC& resolver, const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg )
    : controller_    ( controller )
    , resolver_      ( resolver )
    , nID_           ( asnMsg.oid_connaissance_concentration )
    , pConcentration_( resolver_.FindConcentration( asnMsg.oid_concentration_reelle ) )
    , position_      ( converter.ConvertToXY( asnMsg.position ) )
    , radius_        ( 100.f )
    , deadRadius_    ( 0 )
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
void PopulationConcentrationKnowledge::DoUpdate( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg )
{
    static const float oneOnpi = 1.f / std::acos( -1.f );
    if( asnMsg.m.attitudePresent )
        eAttitude_ = ( E_PopulationAttitude )asnMsg.attitude;
    if( asnMsg.m.est_percuPresent )
        bIsPerceived_ = asnMsg.est_percu != 0;
    if( asnMsg.m.nb_humains_vivantsPresent )
        nNbrAliveHumans_ = ( uint )asnMsg.nb_humains_vivants;
    if( asnMsg.m.nb_humains_mortsPresent )
        nNbrDeadHumans_ = ( uint )asnMsg.nb_humains_morts;
    if( asnMsg.m.oid_concentration_reellePresent )
        pConcentration_ = resolver_.FindConcentration( asnMsg.oid_concentration_reelle );
    if( asnMsg.m.pertinencePresent )
        rRelevance_ = asnMsg.pertinence;

    const float density = pConcentration_ ? pConcentration_->GetDensity() : 0.f;
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
    displayer.Group( tools::translate( "Population", "Concentration" ) )
                .Display( tools::translate( "Population", "Identifier:" ), nID_ )
                .Display( tools::translate( "Population", "Associated concentration:" ), pConcentration_ )
                .Display( tools::translate( "Population", "Location:" ), position_ )
                .Display( tools::translate( "Population", "Alive people:" ), nNbrAliveHumans_ )
                .Display( tools::translate( "Population", "Dead people:" ), nNbrDeadHumans_ )
                .Display( tools::translate( "Population", "Mood:" ), eAttitude_ )
                .Display( tools::translate( "Population", "Perceived:" ), bIsPerceived_ )
                .Display( tools::translate( "Population", "Relevance:" ), rRelevance_ );
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
    if( pConcentration_ ) // $$$$ SBO 2007-02-27: isPerceived?
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
