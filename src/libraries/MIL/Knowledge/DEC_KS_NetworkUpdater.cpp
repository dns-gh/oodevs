// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_NetworkUpdater.cpp $
// $Author: Jvt $
// $Modtime: 23/03/05 18:58 $
// $Revision: 3 $
// $Workfile: DEC_KS_NetworkUpdater.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"
#include "DEC_KS_NetworkUpdater.h"

#include "DEC_KnowledgeBlackBoard.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_Agent.h"
#include "DEC_Knowledge_ObjectPerception.h"
#include "DEC_Knowledge_Object.h"
#include "DEC_Knowledge_PopulationPerception.h"
#include "DEC_Knowledge_Population.h"
#include "DEC_Knowledge_PopulationCollision.h"

// -----------------------------------------------------------------------------
// Name: DEC_KS_NetworkUpdater constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_NetworkUpdater::DEC_KS_NetworkUpdater( DEC_KnowledgeBlackBoard& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard )
{
    pBlackBoard_->AddToScheduler( *this );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_NetworkUpdater destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_NetworkUpdater::~DEC_KS_NetworkUpdater()
{
    assert( pBlackBoard_ );
    pBlackBoard_->RemoveFromScheduler( *this );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_NetworkUpdater::Prepare
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_NetworkUpdater::Prepare()
{
    // Nothing
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_NetworkUpdater::Talk
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_NetworkUpdater::Talk()
{
    assert( pBlackBoard_ );
    
    pBlackBoard_->ApplyOnKnowledgesAgentPerception     ( std::mem_fun_ref( & DEC_Knowledge_AgentPerception     ::UpdateOnNetwork ) );
    pBlackBoard_->ApplyOnKnowledgesAgent               ( std::mem_fun_ref( & DEC_Knowledge_Agent               ::UpdateOnNetwork ) );
    pBlackBoard_->ApplyOnKnowledgesObjectPerception    ( std::mem_fun_ref( & DEC_Knowledge_ObjectPerception    ::UpdateOnNetwork ) );
    pBlackBoard_->ApplyOnKnowledgesObject              ( std::mem_fun_ref( & DEC_Knowledge_Object              ::UpdateOnNetwork ) );
    pBlackBoard_->ApplyOnKnowledgesPopulationPerception( std::mem_fun_ref( & DEC_Knowledge_PopulationPerception::UpdateOnNetwork ) );
    pBlackBoard_->ApplyOnKnowledgesPopulation          ( std::mem_fun_ref( & DEC_Knowledge_Population          ::UpdateOnNetwork ) );
    pBlackBoard_->ApplyOnKnowledgesPopulationCollision ( std::mem_fun_ref( & DEC_Knowledge_PopulationCollision ::UpdateOnNetwork ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_NetworkUpdater::SendFullState
// Created: NLD 2004-03-18
// -----------------------------------------------------------------------------
void DEC_KS_NetworkUpdater::SendFullState()
{
    assert( pBlackBoard_ );

    pBlackBoard_->ApplyOnKnowledgesAgentPerception     ( std::mem_fun_ref( & DEC_Knowledge_AgentPerception     ::SendStateToNewClient ) );
    pBlackBoard_->ApplyOnKnowledgesAgent               ( std::mem_fun_ref( & DEC_Knowledge_Agent               ::SendStateToNewClient ) );
    pBlackBoard_->ApplyOnKnowledgesObjectPerception    ( std::mem_fun_ref( & DEC_Knowledge_ObjectPerception    ::SendStateToNewClient ) );
    pBlackBoard_->ApplyOnKnowledgesObject              ( std::mem_fun_ref( & DEC_Knowledge_Object              ::SendStateToNewClient ) );
    pBlackBoard_->ApplyOnKnowledgesPopulationPerception( std::mem_fun_ref( & DEC_Knowledge_PopulationPerception::SendStateToNewClient ) );
    pBlackBoard_->ApplyOnKnowledgesPopulation          ( std::mem_fun_ref( & DEC_Knowledge_Population          ::SendStateToNewClient ) );
    pBlackBoard_->ApplyOnKnowledgesPopulationCollision ( std::mem_fun_ref( & DEC_Knowledge_PopulationCollision ::SendStateToNewClient ) );
}
