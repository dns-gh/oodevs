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


// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::GetKnowledgeRapForGlobal
// Created: NLD 2004-04-07
// -----------------------------------------------------------------------------
inline
DEC_Knowledge_RapForGlobal& DEC_KnowledgeBlackBoard_Automate::GetKnowledgeRapForGlobal()
{
    return knowledgeRapForGlobal_;
}

// -----------------------------------------------------------------------------
// Name: DEC_KnowledgeBlackBoard_Automate::GetRapForGlobalValue
// Created: NLD 2004-04-08
// -----------------------------------------------------------------------------
inline
MT_Float DEC_KnowledgeBlackBoard_Automate::GetRapForGlobalValue() const
{
    return knowledgeRapForGlobal_.GetValue();
}