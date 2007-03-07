// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Population_Path_Channeler.h"
#include "DEC_Population_PathClass.h"

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path_Channeler constructor
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
DEC_Population_Path_Channeler::DEC_Population_Path_Channeler( const DEC_Population_PathClass& pathClass, const TER_Localisation& location )
    : location_( location )
{
    location_.Scale( pathClass.GetChannelingRange() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path_Channeler destructor
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
DEC_Population_Path_Channeler::~DEC_Population_Path_Channeler()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path_Channeler::ComputeCost
// Created: SBO 2006-01-16
// -----------------------------------------------------------------------------
double DEC_Population_Path_Channeler::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& /*nToTerrainType*/, const TerrainData& /*nLinkTerrainType*/ ) const
{
    const MT_Line line( from, to );
    if( location_.Intersect2D( line ) || location_.IsInside( to ) )
        return 0.;
    return std::numeric_limits< double >::min();
}
