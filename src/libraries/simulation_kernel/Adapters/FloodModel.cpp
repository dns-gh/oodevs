// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "FloodModel.h"

using namespace sword;

// -----------------------------------------------------------------------------
// Name: FloodModel constructor
// Created: LGY 2012-06-13
// -----------------------------------------------------------------------------
FloodModel::FloodModel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FloodModel destructor
// Created: LGY 2012-06-13
// -----------------------------------------------------------------------------
FloodModel::~FloodModel()
{
    // NOTHING
}
// -----------------------------------------------------------------------------
// Name: FloodModel::GenerateFlood
// Created: LGY 2012-06-18
// -----------------------------------------------------------------------------
bool FloodModel::GenerateFlood( const geometry::Point2d& center, int depth, int refDist, bool force /*= false*/ )
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: FloodModel::GetDeepAreas
// Created: LGY 2012-06-18
// -----------------------------------------------------------------------------
const std::vector< geometry::Polygon2f* >& FloodModel::GetDeepAreas() const
{
    return std::vector< geometry::Polygon2f* >();
}

// -----------------------------------------------------------------------------
// Name: FloodModel::GetLowAreas
// Created: LGY 2012-06-18
// -----------------------------------------------------------------------------
const std::vector< geometry::Polygon2f* >& FloodModel::GetLowAreas() const
{
    return std::vector< geometry::Polygon2f* >();
}
