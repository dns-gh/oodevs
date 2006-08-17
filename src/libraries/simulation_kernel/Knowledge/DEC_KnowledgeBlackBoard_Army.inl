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
// Name: DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectContainer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_BlackBoard_CanContainKnowledgeObject& DEC_KnowledgeBlackBoard_Army::GetKnowledgeObjectContainer() const
{
    assert( pKnowledgeObjectContainer_ );
    return *pKnowledgeObjectContainer_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetArmy
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
MIL_Army& DEC_KnowledgeBlackBoard_Army::GetArmy() const
{
    assert( pArmy_ );
    return *pArmy_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Army::GetKSObjectKnowledgeSynthetizer
// Created: NLD 2006-04-12
// -----------------------------------------------------------------------------
inline
DEC_KS_ObjectKnowledgeSynthetizer& DEC_KnowledgeBlackBoard_Army::GetKsObjectKnowledgeSynthetizer() const
{
    assert( pKsObjectKnowledgeSynthetizer_ );
    return *pKsObjectKnowledgeSynthetizer_;
}
