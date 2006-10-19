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
// Name: ModelRefsContainer< Formation >& Side::GetFormations
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
inline
ModelRefsContainer< Formation >& Side::GetFormations()
{
    return formations_;
}

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
// Name: ModelRefsContainer< Object >& Side::GetObjects
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
ModelRefsContainer< Object >& Side::GetObjects()
{
    return objects_;
}

// -----------------------------------------------------------------------------
// Name: ModelRefsContainer< Population >& Side::GetPopulations
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
ModelRefsContainer< Population >& Side::GetPopulations()
{
    return populations_;
}

// -----------------------------------------------------------------------------
// Name: Side::GetID
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
inline
unsigned long Side::GetID() const
{
    return nID_;
}

}
