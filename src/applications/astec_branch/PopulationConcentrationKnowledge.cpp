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

#include "App.h"
#include "World.h"
#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationKnowledge.h"
#include "AgentManager.h"
#include "Model.h"

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::PopulationConcentrationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
PopulationConcentrationKnowledge::PopulationConcentrationKnowledge( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg )
    : nID_                 ( asnMsg.oid_connaissance_concentration )
    , pGtia_               ( & App::GetApp().GetModel().GetGtia( asnMsg.oid_groupe_possesseur ) )
    , pPopulationKnowledge_( 0 )
    , pConcentration_      ( 0 )
{
//    pPopulationKnowledge_ = pGtia_->FindPopulationKnowledge( asnMsg.oid_connaissance_population );
//    assert( pPopulationKnowledge_ );
//    const Population& population = pPopulationKnowledge_->GetPopulation();
//
//    if( asnMsg.oid_concentration_reelle == 0 )
//        pConcentration_ = 0;
//    else
//    {
//        pConcentration_ = population.FindConcentration( asnMsg.oid_concentration_reelle );
//        assert( pConcentration_ );
//    }
//    
//    App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.position.data, position_ );
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
    {
        const Population& population = pPopulationKnowledge_->GetPopulation();
        if( asnMsg.oid_concentration_reelle == 0 )
            pConcentration_ = 0;
        else
        {
            pConcentration_ = population.FindConcentration( asnMsg.oid_concentration_reelle );
            assert( pConcentration_ );
        }
    }
    if( asnMsg.m.pertinencePresent )
        rRelevance_ = asnMsg.pertinence;
}

// -----------------------------------------------------------------------------
// Name: PopulationConcentrationKnowledge::GetArea
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
MT_Float PopulationConcentrationKnowledge::GetArea() const
{
    assert( pPopulationKnowledge_ );
    return GetNbrAliveHumans() / pPopulationKnowledge_->GetType().GetConcentrationDensity();
}
