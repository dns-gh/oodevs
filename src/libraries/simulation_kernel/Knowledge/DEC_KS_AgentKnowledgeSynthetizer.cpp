// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_KS_AgentKnowledgeSynthetizer.cpp $
// $Author: Nld $
// $Modtime: 21/04/05 11:01 $
// $Revision: 7 $
// $Workfile: DEC_KS_AgentKnowledgeSynthetizer.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KS_AgentKnowledgeSynthetizer.h"

#include "DEC_KnowledgeBlackBoard_KnowledgeGroup.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgent.h"
#include "DEC_BlackBoard_CanContainKnowledgeAgentPerception.h"
#include "DEC_Knowledge_AgentPerception.h"
#include "DEC_Knowledge_Agent.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "simulation_kernel/MIL_AgentServer.h" // LTO
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h" // LTO
#include <boost/bind.hpp>
#include <boost/function.hpp>
 
BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_AgentKnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer constructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_AgentKnowledgeSynthetizer::DEC_KS_AgentKnowledgeSynthetizer( DEC_KnowledgeBlackBoard_KnowledgeGroup& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_           ( &blackBoard )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer constructor
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
DEC_KS_AgentKnowledgeSynthetizer::DEC_KS_AgentKnowledgeSynthetizer()
    : DEC_KnowledgeSource_ABC()
    , pBlackBoard_           ( 0 )
{

}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer destructor
// Created: NLD 2004-03-11
// -----------------------------------------------------------------------------
DEC_KS_AgentKnowledgeSynthetizer::~DEC_KS_AgentKnowledgeSynthetizer()
{
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::Prepare
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::Prepare()
{
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( & DEC_Knowledge_Agent::Prepare, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::GetKnowledgeToUpdate
// Created: NLD 2004-03-19
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent& DEC_KS_AgentKnowledgeSynthetizer::GetKnowledgeToUpdate( MIL_Agent_ABC& agentKnown ) const
{
    assert( pBlackBoard_ );
    boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = pBlackBoard_->GetKnowledgeAgentContainer().GetKnowledgeAgent( agentKnown );
    if( pKnowledge.get() )
        return *pKnowledge;
    
    return pBlackBoard_->GetKnowledgeAgentContainer().CreateKnowledgeAgent( pBlackBoard_->GetKnowledgeGroup(), agentKnown );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::GetKnowledgeToUpdate
// Created: FDS 2010-03-25
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_Agent& DEC_KS_AgentKnowledgeSynthetizer::GetKnowledgeToUpdate( MIL_Agent_ABC& agentKnown, const MIL_KnowledgeGroup& knowledgeGroup ) const
{
    boost::shared_ptr< DEC_Knowledge_Agent > pKnowledge = knowledgeGroup.GetKnowledge().GetKnowledgeAgentContainer().GetKnowledgeAgent( agentKnown );    
    if( pKnowledge.get() )
        return *pKnowledge;
    
    return knowledgeGroup.GetKnowledge().GetKnowledgeAgentContainer().CreateKnowledgeAgent( knowledgeGroup, agentKnown );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromAgentPerception
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
inline
void DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromAgentPerception( const DEC_Knowledge_AgentPerception& perception, int currentTimeStep )
{
    if( perception.IsAvailable() )
        GetKnowledgeToUpdate( perception.GetAgentPerceived() ).Update( perception, currentTimeStep );
}


// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgeFromAgentJamedPerception
// Created: FDS 2010-03-25
// -----------------------------------------------------------------------------
inline
void DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgeFromAgentJamedPerception( const DEC_Knowledge_AgentPerception& perception, const MIL_KnowledgeGroup& knowledgeGroup, int currentTimeStep )
{
    if( perception.IsAvailable() )
        GetKnowledgeToUpdate( perception.GetAgentPerceived(), knowledgeGroup ).Update( perception, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromAgentPerception
// Created: SLG 2009-11-26
// LTO
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromKnowledgeAgent( const DEC_Knowledge_Agent& agentKnowledge, int currentTimeStep )
{
    if( agentKnowledge.IsValid() )
        GetKnowledgeToUpdate( agentKnowledge.GetAgentKnown() ).Update( agentKnowledge, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromParentKnowledgeGroup
// Created: SLG 2009-11-26
// LTO
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromParentKnowledgeGroup( const DEC_Knowledge_Agent& agentKnowledge, int currentTimeStep )
{
    if( agentKnowledge.IsValid() )
        GetKnowledgeToUpdate( agentKnowledge.GetAgentKnown() ).Update( agentKnowledge, currentTimeStep );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::Talk
// Created: NLD 2004-03-12
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::Talk( int currentTimeStep )
{
    assert( pBlackBoard_ );

    {
        boost::function< void ( const DEC_Knowledge_AgentPerception& ) > functor = boost::bind( & DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromAgentPerception, this, _1, boost::ref(currentTimeStep) );

        // Synthèse de la perception des subordonnés
        // Ajout automatique de la connaissance de chaque subordonné    
        const MIL_KnowledgeGroup::T_AutomateVector& automates = pBlackBoard_->GetKnowledgeGroup().GetAutomates();
        for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
        {
            const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
            for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            {
                MIL_AgentPion& pion = **itPion;
                 // $$$$ FDS 2010-03-25: Les perceptions des subordonnées sont envoyées uniquement dans le cas ou celui ci peut communiquer.
                if ( pion.GetRole< PHY_RolePion_Communications >().CanCommunicate() )
                {
                    if( ! pBlackBoard_->GetKnowledgeGroup().IsJammedKnowledgeGroup() )
                        pion.GetKnowledge().GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( functor );
                }
                else
                    pion.GetKnowledge().GetKnowledgeAgentPerceptionContainer().ApplyOnKnowledgesAgentPerception( boost::bind( &DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgeFromAgentJamedPerception, this, _1, boost::ref( pion.GetKnowledgeGroup() ), boost::ref(currentTimeStep) ) );                    
            }
        }
    }

    // mis à jour des groupes de connaissances parents
    const MIL_KnowledgeGroup& knowledgeGroup = pBlackBoard_->GetKnowledgeGroup();
    boost::function< void ( const DEC_Knowledge_Agent& ) > functor = boost::bind( &DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
    for( MIL_KnowledgeGroup::CIT_KnowledgeGroupVector itKG( knowledgeGroup.GetKnowledgeGroups().begin() ); itKG != knowledgeGroup.GetKnowledgeGroups().end(); ++itKG )
    {
        const MIL_KnowledgeGroup& innerKg = **itKG;
        if ( innerKg.IsEnabled() && knowledgeGroup.IsEnabled() )
            innerKg.GetKnowledge().GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( functor );
    }

        // LTO begin
        //mis à jour des groupes de connaissances fils avec un délai
    //boost::function< void ( const DEC_Knowledge_Agent& ) > functorChildren = boost::bind( &DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) );
        MIL_KnowledgeGroup* pParent = knowledgeGroup.GetParent();

        if ( pBlackBoard_->GetKnowledgeGroup().GetTimeToDiffuseToKnowledgeGroup() < currentTimeStep )
        {
            if ( pParent && pParent->IsEnabled() && knowledgeGroup.IsEnabled() )
            pParent->GetKnowledge().GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( &DEC_KS_AgentKnowledgeSynthetizer::UpdateKnowledgesFromParentKnowledgeGroup, this, _1, boost::ref(currentTimeStep) ) );
            pBlackBoard_->GetKnowledgeGroup().RefreshTimeToDiffuseToKnowledgeGroup();
        }
        // LTO end

    if( ! pBlackBoard_->GetKnowledgeGroup().IsJammedKnowledgeGroup() )
    {
        // Extrapolation
        pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( & DEC_Knowledge_Agent::Extrapolate, _1 ) );
        // Relevance
        pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( boost::bind( &DEC_Knowledge_Agent::UpdateRelevance, _1, currentTimeStep ) );
    }

}

// =============================================================================
// CLEAN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::CleanKnowledgeAgent
// Created: NLD 2004-03-17
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::CleanKnowledgeAgent( DEC_Knowledge_Agent& knowledge )
{
    assert( pBlackBoard_ );

    if( knowledge.Clean() )
        pBlackBoard_->GetKnowledgeAgentContainer().DestroyKnowledgeAgent( knowledge ); // The knowledge will be deleted
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_AgentKnowledgeSynthetizer::Clean
// Created: NLD 2004-03-16
// -----------------------------------------------------------------------------
void DEC_KS_AgentKnowledgeSynthetizer::Clean()
{
    assert( pBlackBoard_ );

    class_mem_fun_void_t< DEC_KS_AgentKnowledgeSynthetizer, DEC_Knowledge_Agent > methodAgent( & DEC_KS_AgentKnowledgeSynthetizer::CleanKnowledgeAgent, *this );
    pBlackBoard_->GetKnowledgeAgentContainer().ApplyOnKnowledgesAgent( methodAgent );
}
