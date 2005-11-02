//*****************************************************************************
// 
// $Created: AGN 03-04-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_PopulationConcentrationKnowledge.cpp $
// $Author: Age $
// $Modtime: 31/03/05 17:45 $
// $Revision: 8 $
// $Workfile: MOS_PopulationConcentrationKnowledge.cpp $
// 
//*****************************************************************************

#include "MOS_Light2_pch.h"
#include "MOS_PopulationConcentrationKnowledge.h"

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_Population.h"
#include "MOS_PopulationConcentration.h"
#include "MOS_PopulationKnowledge.h"
#include "MOS_AgentManager.h"

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::MOS_PopulationConcentrationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
MOS_PopulationConcentrationKnowledge::MOS_PopulationConcentrationKnowledge( const ASN1T_MsgPopulationConcentrationKnowledgeCreation& asnMsg )
    : nID_                 ( asnMsg.oid_connaissance_concentration )
    , pGtia_               ( MOS_App::GetApp().GetAgentManager().FindGtia( asnMsg.oid_groupe_possesseur ) )
    , pPopulationKnowledge_( 0 )
    , pConcentration_      ( 0 )
{
    assert( pGtia_ );
    pPopulationKnowledge_ = pGtia_->FindPopulationKnowledge( asnMsg.oid_connaissance_population );
    assert( pPopulationKnowledge_ );
    const MOS_Population& population = pPopulationKnowledge_->GetPopulation();
    pConcentration_ = population.FindConcentration( asnMsg.oid_concentration_reelle );

    MOS_App::GetApp().GetWorld().MosToSimMgrsCoord( (const char*)asnMsg.position.data, position_ );
}



// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::~MOS_PopulationConcentrationKnowledge
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
MOS_PopulationConcentrationKnowledge::~MOS_PopulationConcentrationKnowledge()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::Update
// Created: SBO 2005-10-17
// -----------------------------------------------------------------------------
void MOS_PopulationConcentrationKnowledge::Update( const ASN1T_MsgPopulationConcentrationKnowledgeUpdate& asnMsg )
{
    if( asnMsg.m.attitudePresent )
        eAttitude_ = ( E_PopulationAttitude )asnMsg.attitude;
    if( asnMsg.m.est_percuPresent )
        bIsPerceived_ = asnMsg.est_percu;
    if( asnMsg.m.nb_humains_vivantsPresent )
        nNbrAliveHumans_ = asnMsg.nb_humains_vivants;
    if( asnMsg.m.nb_humains_mortsPresent )
        nNbrDeadHumans_ = asnMsg.nb_humains_morts;
    if( asnMsg.m.oid_concentration_reellePresent )
    {
        const MOS_Population& population = pPopulationKnowledge_->GetPopulation();
        pConcentration_ = population.FindConcentration( asnMsg.oid_concentration_reelle );
    }
    if( asnMsg.m.pertinencePresent )
        rRelevance_ = asnMsg.pertinence;
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationConcentrationKnowledge::GetArea
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
MT_Float MOS_PopulationConcentrationKnowledge::GetArea() const
{
    assert( pPopulationKnowledge_ );
    return GetNbrAliveHumans() / pPopulationKnowledge_->GetType().GetConcentrationDensity();
}
