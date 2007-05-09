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
// Name: KnowledgeGroup::GetID
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
unsigned long KnowledgeGroup::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer< Automat > KnowledgeGroup::GetAutomats
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
inline
ModelRefsContainer< Automat >& KnowledgeGroup::GetAutomats()
{
    return automats_;
}

}
