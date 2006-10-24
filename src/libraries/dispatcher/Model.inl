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
// Name: ModelsContainer< Side > Model::GetSides
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
ModelsContainer< Side >& Model::GetSides()
{
    return sides_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< KnowledgeGroup > Model::GetKnowledgeGroups
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
ModelsContainer< KnowledgeGroup >& Model::GetKnowledgeGroups()
{
    return knowledgeGroups_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< Formation >& Model::GetFormations
// Created: NLD 2006-10-13
// -----------------------------------------------------------------------------
inline
ModelsContainer< Formation >& Model::GetFormations()
{
    return formations_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< Automat > Model::GetAutomats
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
ModelsContainer< Automat >& Model::GetAutomats()
{
    return automats_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< Automat >& Model::GetAutomats
// Created: NLD 2006-10-24
// -----------------------------------------------------------------------------
inline
const ModelsContainer< Automat >& Model::GetAutomats() const
{
    return automats_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< Agent >& Model::GetAgents
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
inline
ModelsContainer< Agent >& Model::GetAgents()
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< Agent >& Model::GetAgents
// Created: NLD 2006-09-26
// -----------------------------------------------------------------------------
inline
const ModelsContainer< Agent >& Model::GetAgents() const
{
    return agents_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< Object >& Model::GetObjects
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
inline
ModelsContainer< Object >& Model::GetObjects()
{
    return objects_;
}

// -----------------------------------------------------------------------------
// Name: }ModelsContainer< Population >& Model::GetPopulations
// Created: NLD 2006-10-03
// -----------------------------------------------------------------------------
inline
ModelsContainer< Population >& Model::GetPopulations()
{
    return populations_;
}

}