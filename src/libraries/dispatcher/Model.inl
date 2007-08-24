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
// Name: ModelsContainer< Side >& Model::GetSides
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
inline
const ModelsContainer< Side >& Model::GetSides() const
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
// Name: ModelsContainer< KnowledgeGroup > Model::GetKnowledgeGroups
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
const ModelsContainer< KnowledgeGroup >& Model::GetKnowledgeGroups() const
{
    return knowledgeGroups_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< AgentKnowledge > Model::GetAgentKnowledges
// Created: NLD 2006-09-25
// -----------------------------------------------------------------------------
inline
ModelsContainer< AgentKnowledge >& Model::GetAgentKnowledges()
{
    return agentKnowledges_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< AgentKnowledge >& Model::GetAgentKnowledges
// Created: JCR 2007-05-15
// -----------------------------------------------------------------------------
inline
const ModelsContainer< AgentKnowledge >& Model::GetAgentKnowledges() const
{
    return agentKnowledges_;
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
// Name: ModelsContainer< Formation >& Model::GetFormations
// Created: SBO 2006-11-15
// -----------------------------------------------------------------------------
inline
const ModelsContainer< Formation >& Model::GetFormations() const
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
// Name: ModelsContainer< Object >& Model::GetObjects
// Created: NLD 2006-09-29
// -----------------------------------------------------------------------------
inline
const ModelsContainer< Object >& Model::GetObjects() const
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

// -----------------------------------------------------------------------------
// Name: ModelsContainer< Limit>& Model::GetLimits
// Created: SBO 2007-06-01
// -----------------------------------------------------------------------------
inline
const ModelsContainer< Limit >& Model::GetLimits() const
{
    return limits_;
}

// -----------------------------------------------------------------------------
// Name: ModelsContainer< Lima >& Model::GetLimas
// Created: SBO 2007-06-06
// -----------------------------------------------------------------------------
inline
const ModelsContainer< Lima >& Model::GetLimas() const
{
    return limas_;
}

// -----------------------------------------------------------------------------
// Name: Model::GetAgentTypes
// Created: SBO 2007-08-22
// -----------------------------------------------------------------------------
inline
const kernel::AgentTypes& Model::GetAgentTypes() const
{
    return *agentTypes_;
}

}