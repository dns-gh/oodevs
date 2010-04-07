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
// Name: DEC_KS_PopulationKnowledgeSynthetizer::GetKnowledgeToUpdate
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& DEC_KS_PopulationKnowledgeSynthetizer::GetKnowledgeToUpdate( MIL_Population& populationKnown ) const
{
    assert( pBlackBoard_ );
    DEC_Knowledge_Population* pKnowledge = pBlackBoard_->GetKnowledgePopulationContainer().GetKnowledgePopulation( populationKnown );
    if( pKnowledge )
        return *pKnowledge;
    
    return pBlackBoard_->GetKnowledgePopulationContainer().CreateKnowledgePopulation( pBlackBoard_->GetKnowledgeGroup(), populationKnown );
}
 
// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::GetKnowledgeToJamedUpdate
// Created: FDS 2010-04-06
// -----------------------------------------------------------------------------
DEC_Knowledge_Population& DEC_KS_PopulationKnowledgeSynthetizer::GetKnowledgeToJamedUpdate( MIL_Population& populationKnown, const MIL_KnowledgeGroup& knowledgeGroup ) const
{
    DEC_Knowledge_Population* pKnowledge = knowledgeGroup.GetKnowledge().GetKnowledgePopulationContainer().GetKnowledgePopulation( populationKnown );
    if( pKnowledge )
        return *pKnowledge;
    
    return knowledgeGroup.GetKnowledge().GetKnowledgePopulationContainer().CreateKnowledgePopulation( knowledgeGroup, populationKnown );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromPerception( const DEC_Knowledge_PopulationPerception& perception )
{
    GetKnowledgeToUpdate( perception.GetPopulationPerceived() ).Update( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromJamedPerception
// Created: FDS 2010-04-06
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromJamedPerception( const DEC_Knowledge_PopulationPerception& perception, const MIL_KnowledgeGroup& knowledgeGroup )
{
    GetKnowledgeToJamedUpdate( perception.GetPopulationPerceived(), knowledgeGroup ).Update( perception );
}


// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromCollision
// Created: NLD 2005-10-28
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromCollision( const DEC_Knowledge_PopulationCollision& collision )
{
    GetKnowledgeToUpdate( collision.GetPopulation() ).Update( collision );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromJamedCollision
// Created: FDS 2010-04-06
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromJamedCollision( const DEC_Knowledge_PopulationCollision& collision, const MIL_KnowledgeGroup& knowledgeGroup )
{
    GetKnowledgeToJamedUpdate( collision.GetPopulation(), knowledgeGroup ).Update( collision );
}


// -----------------------------------------------------------------------------
// Name: DEC_KS_PopulationKnowledgeSynthetizer::Talk
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KS_PopulationKnowledgeSynthetizer::Talk( int /*currentTimeStep*/ )
{
    assert( pBlackBoard_ );

    // $$$$ _RC_ FDS 2010-04-07: Ne fonctionne pas, crash de l'optimiseur. 
    // boost::function< void ( DEC_Knowledge_PopulationPerception& ) > methodPerception = boost::bind( & DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromPerception, this );
    // boost::function< void ( DEC_Knowledge_PopulationCollision& ) > methodCollision   = boost::bind( & DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromCollision, this );

    class_mem_fun_void_const_t< DEC_KS_PopulationKnowledgeSynthetizer, DEC_Knowledge_PopulationPerception > methodPerception( & DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromPerception, *this );
    class_mem_fun_void_const_t< DEC_KS_PopulationKnowledgeSynthetizer, DEC_Knowledge_PopulationCollision  > methodCollision ( & DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromCollision , *this );

    // Synthèse de la perception des subordonnés
    // Ajout automatique de la connaissance de chaque subordonné    
    const MIL_KnowledgeGroup::T_AutomateVector& automates = pBlackBoard_->GetKnowledgeGroup().GetAutomates();
    for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
    {
        const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
        for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
        {
            MIL_AgentPion& pion = **itPion;
            if( pion.GetRole< PHY_RolePion_Communications >().CanCommunicate() )
            {
                if(  ! pBlackBoard_->GetKnowledgeGroup().IsJammedKnowledgeGroup() )
                { 
                    pion.GetKnowledge().GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( methodPerception );
                    pion.GetKnowledge().GetKnowledgePopulationCollisionContainer ().ApplyOnKnowledgesPopulationCollision ( methodCollision  );
                }
            }
            else
            {
                // $$$$ _RC_ FDS 2010-04-07: encore un soucis, les connaissances du pion brouillé, passe au pion non brouillé, étrange !!
                pion.GetKnowledge().GetKnowledgePopulationPerceptionContainer().ApplyOnKnowledgesPopulationPerception( boost::bind( & DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromJamedPerception, this, _1, boost::ref( pion.GetKnowledgeGroup() ) )  );
                pion.GetKnowledge().GetKnowledgePopulationCollisionContainer ().ApplyOnKnowledgesPopulationCollision ( boost::bind( & DEC_KS_PopulationKnowledgeSynthetizer::UpdateKnowledgesFromJamedCollision, this, _1, boost::ref( pion.GetKnowledgeGroup() ) )  );
            }
        }
    }
    
    // Relevance
    if( ! pBlackBoard_->GetKnowledgeGroup().IsJammedKnowledgeGroup() )
        pBlackBoard_->GetKnowledgePopulationContainer().ApplyOnKnowledgesPopulation( boost::bind( &DEC_Knowledge_Population::UpdateRelevance, _1 ) );
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
