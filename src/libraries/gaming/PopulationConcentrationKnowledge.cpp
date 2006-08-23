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
#include "PopulationKnowledge.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/CoordinateConverter_ABC.h"
#include "Tools.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::PopulationConcentrationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::PopulationConcentrationKnowledge( Controller& controller, const CoordinateConverter_ABC& converter, const Population_ABC& resolver, const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg )
    : controller_( controller )
    , resolver_  ( resolver )
    , nID_           ( asnMsg.oid_connaissance_concentration )
    , pConcentration_( resolver_.FindConcentration( asnMsg.oid_concentration_reelle ) )
    , position_      ( converter.ConvertToXY( asnMsg.position ) )
{
    // NOTHING
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
// Name: PopulationConcentrationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg )
{
    if( asnMsg.m.attitudePresent )
        eAttitude_ = ( E_PopulationAttitude )asnMsg.attitude;
    if( asnMsg.m.est_percuPresent )
        bIsPerceived_ = ( bool )asnMsg.est_percu;
    if( asnMsg.m.nb_humains_vivantsPresent )
        nNbrAliveHumans_ = ( uint )asnMsg.nb_humains_vivants;
    if( asnMsg.m.nb_humains_mortsPresent )
        nNbrDeadHumans_ = ( uint )asnMsg.nb_humains_morts;
    if( asnMsg.m.oid_concentration_reellePresent )
        pConcentration_ = resolver_.FindConcentration( asnMsg.oid_concentration_reelle );
    if( asnMsg.m.pertinencePresent )
        rRelevance_ = asnMsg.pertinence;
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::Display
// Created: AGE 2006-02-24
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Population", "Concentration" ) )
                .Display( tools::translate( "Population", "Id:" ), nID_ )
                .Display( tools::translate( "Population", "Concentration associée:" ), pConcentration_ )
                .Display( tools::translate( "Population", "Position:" ), position_ )
                .Display( tools::translate( "Population", "Humains vivants:" ), nNbrAliveHumans_ )
                .Display( tools::translate( "Population", "Humains morts:" ), nNbrDeadHumans_ )
                .Display( tools::translate( "Population", "Attitude:" ), eAttitude_ )
                .Display( tools::translate( "Population", "Percue:" ), bIsPerceived_ )
                .Display( tools::translate( "Population", "Pertinence:" ), rRelevance_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::DisplayInList
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    displayer.Display( tools::translate( "Population", "Populations connues" ) )
                .Start( tools::translate( "Population", "Concentration - " ) ).Add( nID_ ).End();
}
