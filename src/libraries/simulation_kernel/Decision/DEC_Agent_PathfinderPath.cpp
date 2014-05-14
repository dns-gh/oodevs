// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Agent_PathfinderPath.h"
#include "DEC_Agent_Path.h"

DEC_Agent_PathfinderPath::DEC_Agent_PathfinderPath( const DEC_Agent_Path& path )
    : path_( path )
{
    // NOTHING
}

const MIL_Fuseau& DEC_Agent_PathfinderPath::GetFuseau() const
{
    return path_.GetFuseau();
}

const MIL_Fuseau& DEC_Agent_PathfinderPath::GetAutomataFuseau() const
{
    return path_.GetAutomataFuseau();
}

const PHY_Speeds& DEC_Agent_PathfinderPath::GetUnitSpeeds() const
{
    return path_.GetUnitSpeeds();
}

const MT_Vector2D& DEC_Agent_PathfinderPath::GetDirDanger() const
{
    return path_.GetDirDanger();
}

const DEC_Agent_PathClass& DEC_Agent_PathfinderPath::GetPathClass() const
{
    return path_.GetPathClass();
}

double DEC_Agent_PathfinderPath::GetUnitMaxSlope() const
{
    return path_.GetUnitMaxSlope();
}

double DEC_Agent_PathfinderPath::GetUnitSlopeDeceleration() const
{
    return path_.GetUnitSlopeDeceleration();
}

double DEC_Agent_PathfinderPath::GetUnitMajorWeight() const
{
    return path_.GetUnitMajorWeight();
}

double DEC_Agent_PathfinderPath::GetCostOutsideOfAllObjects() const
{
    return path_.GetCostOutsideOfAllObjects();
}

const DEC_Agent_PathfinderPath::T_PathKnowledgeObjectByTypesVector& DEC_Agent_PathfinderPath::GetPathKnowledgeObjects() const
{
    return path_.GetPathKnowledgeObjects();
}

const DEC_Agent_PathfinderPath::T_PathKnowledgeAgentVector& DEC_Agent_PathfinderPath::GetPathKnowledgeAgents() const
{
    return path_.GetPathKnowledgeAgents();
}

const DEC_Agent_PathfinderPath::T_PathKnowledgePopulationVector& DEC_Agent_PathfinderPath::GetPathKnowledgePopulations() const
{
    return path_.GetPathKnowledgePopulations();
}
