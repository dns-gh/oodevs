// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2002 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Agent_PathSection.h"
#include "DEC_Agent_PathfinderRule.h"

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathSection constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_Agent_PathSection::DEC_Agent_PathSection( DEC_PathResult_ABC& result, const DEC_Agent_PathfinderPath& path,
    const MT_Vector2D& startPoint, const MT_Vector2D& endPoint, bool needRefine, bool useStrictClosest )
    : DEC_PathSection_ABC( result, startPoint, endPoint, needRefine, useStrictClosest )
    , rule_( new DEC_Agent_PathfinderRule( path, startPoint, endPoint ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathSection destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Agent_PathSection::~DEC_Agent_PathSection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathSection::GetRule
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
TerrainRule_ABC& DEC_Agent_PathSection::GetRule() const
{
    return *rule_;
}
