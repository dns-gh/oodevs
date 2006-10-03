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
// Name: PopulationKnowledge::GetID
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
unsigned long PopulationKnowledge::GetID() const
{
    return nID_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetKnowledgeGroup
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
inline
const KnowledgeGroup& PopulationKnowledge::GetKnowledgeGroup() const
{
    return knowledgeGroup_;
}

// -----------------------------------------------------------------------------
// Name: PopulationKnowledge::GetPopulation
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
inline
const Population& PopulationKnowledge::GetPopulation() const
{
    return population_;
}

}
