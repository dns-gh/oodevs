// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2006-04-12 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

// =============================================================================
// ACCESSORS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetPion
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
MIL_AgentPion& DEC_KnowledgeBlackBoard_AgentPion::GetPion() const
{
    assert( pPion_ );
    return *pPion_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgeAgentPerceptionContainer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_BlackBoard_CanContainKnowledgeAgentPerception& DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgeAgentPerceptionContainer() const
{
    assert( pKnowledgeAgentPerceptionContainer_ );
    return *pKnowledgeAgentPerceptionContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgeObjectCollisionContainer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_BlackBoard_CanContainKnowledgeObjectCollision& DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgeObjectCollisionContainer() const
{
    assert( pKnowledgeObjectCollisionContainer_ );
    return *pKnowledgeObjectCollisionContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgeObjectPerceptionContainer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_BlackBoard_CanContainKnowledgeObjectPerception& DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgeObjectPerceptionContainer() const
{
    assert( pKnowledgeObjectPerceptionContainer_ );
    return *pKnowledgeObjectPerceptionContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgePopulationPerceptionContainer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_BlackBoard_CanContainKnowledgePopulationPerception& DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgePopulationPerceptionContainer() const
{
    assert( pKnowledgePopulationPerceptionContainer_ );
    return *pKnowledgePopulationPerceptionContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgePopulationCollisionContainer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_BlackBoard_CanContainKnowledgePopulationCollision& DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgePopulationCollisionContainer() const
{
    assert( pKnowledgePopulationCollisionContainer_ );
    return *pKnowledgePopulationCollisionContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetKsFire
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_KS_Fire& DEC_KnowledgeBlackBoard_AgentPion::GetKsFire() const
{
    assert( pKsFire_ );
    return *pKsFire_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetKsPerception
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_KS_Perception& DEC_KnowledgeBlackBoard_AgentPion::GetKsPerception() const
{
    assert( pKsPerception_ );
    return *pKsPerception_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetKsObjectInteraction
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_KS_ObjectInteraction& DEC_KnowledgeBlackBoard_AgentPion::GetKsObjectInteraction() const
{
    assert( pKsObjectInteraction_ );
    return *pKsObjectInteraction_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetKsPopulationInteraction
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_KS_PopulationInteraction& DEC_KnowledgeBlackBoard_AgentPion::GetKsPopulationInteraction() const
{
    assert( pKsPopulationInteraction_ );
    return *pKsPopulationInteraction_;
}

//------------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgeRapForLocal
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_RapForLocal& DEC_KnowledgeBlackBoard_AgentPion::GetKnowledgeRapForLocal()
{
    return knowledgeRapForLocal_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_AgentPion::GetRapForLocalValue
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
inline
MT_Float DEC_KnowledgeBlackBoard_AgentPion::GetRapForLocalValue() const
{
    return knowledgeRapForLocal_.GetValue();
}


