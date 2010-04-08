// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_PopulationKnowledgeSynthetizer.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 11:01 $
// $Revision: 7 $
// $Workfile: DEC_KS_PopulationKnowledgeSynthetizer.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KS_PopulationKnowledgeSynthetizer.h"

#include "DEC_BlackBoard_CanContainKnowledgePopulation.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationCollision.h"
#include "DEC_BlackBoard_CanContainKnowledgePopulationPerception.h"
#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationCollision.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"

#include <boost/bind.hpp>
#include <boost/function.hpp>


BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_PopulationKnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_PopulationKnowledgeSynthetizer::DEC_KS_PopulationKnowledgeSynthetizer( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_           ( &blackBoard )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_PopulationKnowledgeSynthetizer::DEC_KS_PopulationKnowledgeSynthetizer()
    : DEC_KnowledgeSource_ABC( )
    , pBlackBoard_           ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_PopulationKnowledgeSynthetizer::~DEC_KS_PopulationKnowledgeSynthetizer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::Prepare
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::Prepare()
{
    pBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( boost::bind( & DEC_Knowledge_Population::Prepare, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::Talk
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::Talk( int /*currentTimeStep*/ )
{
    assert( pBlackBoard_ );

    pBlackBoard_->ApplyOnKnowledgesPopulationPerception(); 

    // si le group de connaissance est un group brouillé, aucune connaissance ne lui est transmise
    if( ! pBlackBoard_->GetKnowledgeGroup().IsJammedKnowledgeGroup() )
    {    
        // Relevance
        pBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( boost::bind( &DEC_Knowledge_Population::UpdateRelevance, _1 ) );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::CleanKnowledgePopulation
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::CleanKnowledgePopulation( DEC_Knowledge_Population& knowledge )
{
    if( knowledge.Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->GetKnowledgePopulationContainer().DestroyKnowledgePopulation( knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::Clean
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::Clean()
{
    assert( pBlackBoard_ );

    // $$$$ _RC_ FDS 2010-04-07: Ne fonctionne pas, crash de l'optimiseur. 
    // boost::function< void ( DEC_Knowledge_Population& )  > methodPopulation = boost::bind( &DEC_KS_PopulationKnowledgeSynthetizer::CleanKnowledgePopulation, this );

    class_mem_fun_void_t< DEC_KS_PopulationKnowledgeSynthetizer, DEC_Knowledge_Population > methodPopulation( & DEC_KS_PopulationKnowledgeSynthetizer::CleanKnowledgePopulation, *this );
    pBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( methodPopulation );
}
