// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PathSection.h"

// -----------------------------------------------------------------------------
// Name: TER_PathSection constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
TER_PathSection::TER_PathSection( std::unique_ptr< TerrainRule_ABC > rule,
    const MT_Vector2D& startPoint, const MT_Vector2D& endPoint, bool needRefine,
    bool useStrictClosest )
    : rule_               ( std::move( rule ) )
    , startPoint_         ( startPoint )
    , endPoint_           ( endPoint )
    , needRefine_         ( needRefine )
    , useStrictClosest_   ( useStrictClosest )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
TER_PathSection::~TER_PathSection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::GetPosStart
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
const MT_Vector2D& TER_PathSection::GetPosStart() const
{
    return startPoint_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::GetPosEnd
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
const MT_Vector2D& TER_PathSection::GetPosEnd() const
{
    return endPoint_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
double TER_PathSection::GetLength() const
{
    return startPoint_.Distance( endPoint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::SetPosStart
// Created: CMA 2011-04-18
// -----------------------------------------------------------------------------
void TER_PathSection::SetPosStart( const MT_Vector2D& point )
{
    startPoint_ = point;
}

TerrainRule_ABC& TER_PathSection::GetRule()
{
    return *rule_;
}

bool TER_PathSection::NeedRefine() const
{
    return needRefine_;
}

bool TER_PathSection::UseStrictClosest() const
{
    return useStrictClosest_;
}
