// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Path_KnowledgeObjectBurnSurface.h"
#include "Entities/Objects/BurnSurfaceAttribute.h"

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectBurnSurface constructor
// Created: JSR 2010-12-20
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObjectBurnSurface::DEC_Path_KnowledgeObjectBurnSurface( const DEC_Knowledge_Object& knowledge )
: pBurnSurfaceAttribute_( knowledge.RetrieveAttribute< BurnSurfaceAttribute >() )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectBurnSurface destructor
// Created: JSR 2010-12-20
// -----------------------------------------------------------------------------
DEC_Path_KnowledgeObjectBurnSurface::~DEC_Path_KnowledgeObjectBurnSurface()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectBurnSurface::ComputeCost
// Created: JSR 2010-12-20
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObjectBurnSurface::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& /*nToTerrainType*/, const TerrainData& /*nLinkTerrainType*/ ) const
{
    if( pBurnSurfaceAttribute_ )
        return pBurnSurfaceAttribute_->ComputePathCost( from, to );
    else
        return std::numeric_limits< double >::min();
}

// -----------------------------------------------------------------------------
// Name: DEC_Path_KnowledgeObjectBurnSurface::GetCostOut
// Created: JSR 2010-12-21
// -----------------------------------------------------------------------------
double DEC_Path_KnowledgeObjectBurnSurface::GetCostOut() const
{
    return 0;
}
