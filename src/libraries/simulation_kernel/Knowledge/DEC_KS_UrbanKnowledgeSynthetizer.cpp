// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_KS_UrbanKnowledgeSynthetizer.h"
#include "DEC_KnowledgeBlackBoard_Army.h"
#include "DEC_KnowledgeBlackBoard_AgentPion.h"
#include "DEC_BlackBoard_CanContainKnowledgeUrban.h"
#include "DEC_BlackBoard_CanContainKnowledgeUrbanPerception.h"
#include "DEC_Knowledge_Urban.h"
#include "DEC_Knowledge_UrbanPerception.h"
#include "MIL_KnowledgeGroup.h"
#include "Entities/MIL_Army.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Entities/Objects/UrbanObjectWrapper.h"

BOOST_CLASS_EXPORT_IMPLEMENT( DEC_KS_UrbanKnowledgeSynthetizer )

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer constructor
// Created: MGD 2009-12-05
// -----------------------------------------------------------------------------
DEC_KS_UrbanKnowledgeSynthetizer::DEC_KS_UrbanKnowledgeSynthetizer( DEC_KnowledgeBlackBoard_Army& blackBoard )
    : DEC_KnowledgeSource_ABC( blackBoard, 1 )
    , pBlackBoard_           ( &blackBoard )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer constructor
// Created: MGD 2009-12-05
// -----------------------------------------------------------------------------
DEC_KS_UrbanKnowledgeSynthetizer::DEC_KS_UrbanKnowledgeSynthetizer()
    : DEC_KnowledgeSource_ABC(  )
    , pBlackBoard_           ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer destructor
// Created: MGD 2009-12-05
// -----------------------------------------------------------------------------
DEC_KS_UrbanKnowledgeSynthetizer::~DEC_KS_UrbanKnowledgeSynthetizer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer::Prepare
// Created: MGD 2009-12-05
// -----------------------------------------------------------------------------
void DEC_KS_UrbanKnowledgeSynthetizer::Prepare()
{
    std::mem_fun_ref_t< void, DEC_Knowledge_Urban > functorUrban = std::mem_fun_ref( &DEC_Knowledge_Urban::Prepare );
    pBlackBoard_->GetKnowledgeUrbanContainer().ApplyOnKnowledgesUrbanRef( functorUrban );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer::GetKnowledgeToUpdate
// Created: MGD 2009-12-05
// -----------------------------------------------------------------------------
inline
boost::shared_ptr< DEC_Knowledge_Urban > DEC_KS_UrbanKnowledgeSynthetizer::GetKnowledgeToUpdate( const UrbanObjectWrapper& objectKnown ) const
{
    assert( pBlackBoard_ );
    boost::shared_ptr< DEC_Knowledge_Urban > pKnowledge = pBlackBoard_->GetKnowledgeUrbanContainer().GetKnowledgeUrban( objectKnown );

    if( pKnowledge && pKnowledge->IsValid() )
        return pKnowledge;

    return pBlackBoard_->GetKnowledgeUrbanContainer().CreateKnowledgeUrban( pBlackBoard_->GetArmy(), objectKnown );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer::UpdateKnowledgesFromUrbanPerception
// Created: MGD 2009-12-05
// -----------------------------------------------------------------------------
inline
void DEC_KS_UrbanKnowledgeSynthetizer::UpdateKnowledgesFromUrbanPerception( const DEC_Knowledge_UrbanPerception& perception )
{
    const UrbanObjectWrapper* urbanObject = dynamic_cast< UrbanObjectWrapper* >( MIL_AgentServer::GetWorkspace().GetEntityManager().FindObject( perception.GetUrbanPerceivedId() ) );
    if( urbanObject )
        GetKnowledgeToUpdate( *urbanObject )->Update( perception );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer::SynthetizeSubordinatesPerception
// Created: MGD 2009-12-05
// -----------------------------------------------------------------------------
void DEC_KS_UrbanKnowledgeSynthetizer::SynthetizeSubordinatesPerception()
{
    class_mem_fun_void_const_t< DEC_KS_UrbanKnowledgeSynthetizer, DEC_Knowledge_UrbanPerception> methodUpdateKnowledgesFromUrbanPerception( & DEC_KS_UrbanKnowledgeSynthetizer::UpdateKnowledgesFromUrbanPerception, *this );

    const MIL_Army::T_KnowledgeGroupMap& knowledgeGroups = pBlackBoard_->GetArmy().GetKnowledgeGroups();
    for( MIL_Army::CIT_KnowledgeGroupMap itKnowledgeGroup = knowledgeGroups.begin(); itKnowledgeGroup != knowledgeGroups.end(); ++itKnowledgeGroup )
    {
        const MIL_KnowledgeGroup::T_AutomateVector& automates = itKnowledgeGroup->second->GetAutomates();
        for( MIL_KnowledgeGroup::CIT_AutomateVector itAutomate = automates.begin(); itAutomate != automates.end(); ++itAutomate )
        {
            const MIL_Automate::T_PionVector& pions = (**itAutomate).GetPions();
            for( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
            {
                MIL_AgentPion& pion = **itPion;
                pion.GetKnowledge().GetKnowledgeUrbanPerceptionContainer().ApplyOnKnowledgesUrbanPerception( methodUpdateKnowledgesFromUrbanPerception );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer::UpdateKnowledgeRelevance
// Created: MGD 2009-12-05
// -----------------------------------------------------------------------------
void DEC_KS_UrbanKnowledgeSynthetizer::UpdateKnowledgeRelevance( boost::shared_ptr< DEC_Knowledge_Urban >& knowledge )
{
    assert( pBlackBoard_ );
    knowledge->UpdateRelevance();
    // @TODO MGD destroy when not relevant pBlackBoard_->GetKnowledgeUrbanContainer().NotifyKnowledgeUrbanDissociatedFromRealUrban( knowledge->GetUrbanKnown(), knowledge );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer::Talk
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_KS_UrbanKnowledgeSynthetizer::Talk( int /*currentTimeStep*/ )
{
    // Synthesis of the perceptions of the subordinates
    SynthetizeSubordinatesPerception();

    // Relevance
    class_mem_fun_void_t< DEC_KS_UrbanKnowledgeSynthetizer,  boost::shared_ptr< DEC_Knowledge_Urban > > methodRelevance( & DEC_KS_UrbanKnowledgeSynthetizer::UpdateKnowledgeRelevance, *this );
    pBlackBoard_->GetKnowledgeUrbanContainer().ApplyOnKnowledgesUrban( methodRelevance );
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer::CleanKnowledgeUrban
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_KS_UrbanKnowledgeSynthetizer::CleanKnowledgeUrban( boost::shared_ptr< DEC_Knowledge_Urban >& knowledge )
{
    if( knowledge->Clean() )
    {
        assert( pBlackBoard_ );
        pBlackBoard_->GetKnowledgeUrbanContainer().DestroyKnowledgeUrban( *knowledge ); // The knowledge will be deleted
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_KS_UrbanKnowledgeSynthetizer::Clean
// Created: MGD 2009-12-07
// -----------------------------------------------------------------------------
void DEC_KS_UrbanKnowledgeSynthetizer::Clean()
{
    assert( pBlackBoard_ );

    class_mem_fun_void_t< DEC_KS_UrbanKnowledgeSynthetizer, boost::shared_ptr< DEC_Knowledge_Urban > > methodUrban( & DEC_KS_UrbanKnowledgeSynthetizer::CleanKnowledgeUrban, *this );
    pBlackBoard_->GetKnowledgeUrbanContainer().ApplyOnKnowledgesUrban( methodUrban );
}
