// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathClass
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
const DEC_Agent_PathClass& DEC_Agent_Path::GetPathClass() const
{
    return pathClass_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetUnitSpeeds
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const PHY_Speeds& DEC_Agent_Path::GetUnitSpeeds() const
{
    return unitSpeeds_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetUnitMaxSlope
// Created: AGE 2005-04-13
// -----------------------------------------------------------------------------
inline
double DEC_Agent_Path::GetUnitMaxSlope() const
{
    return rMaxSlope_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetUnitSlopeDeceleration
// Created: JSR 2013-11-08
// -----------------------------------------------------------------------------
inline
double DEC_Agent_Path::GetUnitSlopeDeceleration() const
{
    return rSlopeDeceleration_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetAutomataFuseau
// Created: SBO 2005-08-17
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& DEC_Agent_Path::GetAutomataFuseau() const
{
    return automateFuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetFuseau
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const MIL_Fuseau& DEC_Agent_Path::GetFuseau() const
{
    return fuseau_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetDirDanger
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const MT_Vector2D& DEC_Agent_Path::GetDirDanger() const
{
    return vDirDanger_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathKnowledgeObjects
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const DEC_Agent_Path::T_PathKnowledgeObjectByTypesVector& DEC_Agent_Path::GetPathKnowledgeObjects() const
{
    return pathKnowledgeObjects_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetCostOutsideOfAllObjects
// Created: NLD 2007-02-09
// -----------------------------------------------------------------------------
inline
double DEC_Agent_Path::GetCostOutsideOfAllObjects() const
{
    return rCostOutsideOfAllObjects_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathKnowledgeAgents
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
inline
const DEC_Agent_Path::T_PathKnowledgeAgentVector& DEC_Agent_Path::GetPathKnowledgeAgents() const
{
    return pathKnowledgeAgents_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetPathKnowledgePopulations
// Created: SBO 2006-02-23
// -----------------------------------------------------------------------------
inline
const DEC_Agent_Path::T_PathKnowledgePopulationVector& DEC_Agent_Path::GetPathKnowledgePopulations() const
{
    return pathKnowledgePopulations_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::NeedRefine
// Created: NLD 2006-01-30
// -----------------------------------------------------------------------------
inline
bool DEC_Agent_Path::NeedRefine() const
{
    return bRefine_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::UseStrictClosest
// Created: AGE 2007-05-09
// -----------------------------------------------------------------------------
inline
bool DEC_Agent_Path::UseStrictClosest() const
{
    return ! automateFuseau_.IsNull();
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_Path::GetNextWaypoints
// Created: CMA 2012-02-20
// -----------------------------------------------------------------------------
inline
const T_PointVector& DEC_Agent_Path::GetNextWaypoints() const
{
    return nextWaypoints_;
}
