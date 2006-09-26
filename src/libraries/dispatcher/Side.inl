// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

namespace dispatcher
{

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer< KnowledgeGroup >& Side::GetKnowledgeGroups
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
ModelRefsContainer< KnowledgeGroup >& Side::GetKnowledgeGroups()
{
    return knowledgeGroups_;
}

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer< Automat        >& Side::GetAutomats
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
inline
ModelRefsContainer< Automat >& Side::GetAutomats()
{
    return automats_;
}

}