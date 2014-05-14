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
#include "DEC_Agent_Path.h"
#include "DEC_Agent_PathfinderRule.h"

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathSection constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_Agent_PathSection::DEC_Agent_PathSection( DEC_Agent_Path& path, const MT_Vector2D& startPoint, const MT_Vector2D& endPoint )
    : DEC_PathSection_ABC( path, startPoint, endPoint )
    , rule_              ( new DEC_Agent_PathfinderRule( path, startPoint, endPoint ) )
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
