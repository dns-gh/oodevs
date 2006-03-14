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

#include "astec_pch.h"
#include "PopulationConcentrationKnowledge.h"

#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationKnowledge.h"
#include "AgentManager.h"
#include "Model.h"
#include "TeamsModel.h"
#include "KnowledgeGroupsModel.h"
#include "Controller.h"
#include "Displayer_ABC.h"
#include "CoordinateConverter.h"

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::PopulationConcentrationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::PopulationConcentrationKnowledge( Controller& controller, const CoordinateConverter& converter, const Population& resolver, const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg )
    : controller_( controller )
    , resolver_  ( resolver )
    , nID_           ( asnMsg.oid_connaissance_concentration )
    , pConcentration_( resolver_.FindConcentration( asnMsg.oid_concentration_reelle ) )
    , position_      ( converter.ConvertToXY( asnMsg.position ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::~PopulationConcentrationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::~PopulationConcentrationKnowledge()
{
    // NOTHING
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
    displayer.Group( "Concentration" )
                .Display( "Id:", nID_ )
                .Display( "Concentration associée:", pConcentration_ )
                .Display( "Position:", position_ )
                .Display( "Humains vivants:", nNbrAliveHumans_ )
                .Display( "Humains morts:", nNbrDeadHumans_ )
                .Display( "Attitude:", eAttitude_ )
                .Display( "Percue:", bIsPerceived_ )
                .Display( "Pertinence:", rRelevance_ );
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::DisplayInList
// Created: AGE 2006-02-27
// -----------------------------------------------------------------------------
void PopulationConcentrationKnowledge::DisplayInList( Displayer_ABC& displayer ) const
{
    displayer.Display( "Populations connues" ).Start( "Concentration - " ).Add( nID_ ).End();
}
