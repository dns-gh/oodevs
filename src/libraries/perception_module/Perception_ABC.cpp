// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "Perception_ABC.h"
#include "PerceptionLevel.h"

using namespace sword;
using namespace sword::perception;

// -----------------------------------------------------------------------------
// Name: Perception_ABC::Compute
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const sword::perception::PerceptionLevel& Perception_ABC::ComputePoint( const wrapper::View& /*perceiver*/, const SurfacesAgent_ABC& /*surfaces*/, const MT_Vector2D& /*vPoint*/ ) const
{
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: Perception_ABC::ExecuteAgents
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void Perception_ABC::ExecuteAgents( const wrapper::View& /*model*/, const wrapper::View& /*perceiver*/, const SurfacesAgent_ABC& /*surfaces*/, const T_AgentPtrVector& /*perceivableAgents*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Perception_ABC::ComputeAgent
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const sword::perception::PerceptionLevel& Perception_ABC::ComputeAgent( const wrapper::View& /*model*/, const wrapper::View& /*perceiver*/, const SurfacesAgent_ABC& /*surfaces*/, const wrapper::View& /*target*/ ) const
{
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: Perception_ABC::ExecuteObjects
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void Perception_ABC::ExecuteObjects( const wrapper::View& /*model*/, const wrapper::View& /*perceiver*/, const SurfacesObject_ABC& /*surfaces*/, const T_ObjectVector& /*perceivableObjects*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Perception_ABC::ComputeObject
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const sword::perception::PerceptionLevel& Perception_ABC::ComputeObject( const wrapper::View& /*perceiver*/, const SurfacesObject_ABC& /*surfaces*/, const wrapper::View& /*knowledgeObject*/ ) const
{
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: Perception_ABC::ExecuteFlows
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void Perception_ABC::ExecuteFlows( const wrapper::View& /*perceiver*/, const SurfacesAgent_ABC& /*surfaces*/, const T_ConstPopulationFlowVector& /*perceivableFlows*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Perception_ABC::ComputeConcentration
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
const sword::perception::PerceptionLevel& Perception_ABC::ComputeConcentration( const wrapper::View& /*perceiver*/, const SurfacesAgent_ABC& /*surfaces*/, const wrapper::View& /*concentration*/ ) const
{
    return PerceptionLevel::notSeen_;
}

// -----------------------------------------------------------------------------
// Name: Perception_ABC::ExecuteConcentrations
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
void Perception_ABC::ExecuteConcentrations( const wrapper::View& /*perceiver*/, const SurfacesAgent_ABC& /*surfaces*/, const T_ConstPopulationConcentrationVector& /*perceivableConcentrations*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Perception_ABC::FinalizePerception
// Created: SLI 2012-07-10
// -----------------------------------------------------------------------------
void Perception_ABC::FinalizePerception( const wrapper::View& /*perceiver*/, const SurfacesAgent_ABC& /*surfaces*/ )
{
    // NOTHING
}
