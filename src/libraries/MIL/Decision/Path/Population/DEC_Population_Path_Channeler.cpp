// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2005-03-08 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Population_Path_Channeler.cpp $
// $Author: Nld $
// $Modtime: 20/07/05 18:46 $
// $Revision: 10 $
// $Workfile: DEC_Population_Path_Channeler.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "DEC_Population_Path_Channeler.h"

// -----------------------------------------------------------------------------
// Name: DEC_Population_Path_Channeler constructor
// Created: AGE 2005-03-23
// -----------------------------------------------------------------------------
DEC_Population_Path_Channeler::DEC_Population_Path_Channeler( const TER_Localisation& location )
: location_ ( location )
{
    location_.Scale( 1000. );
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
MT_Float DEC_Population_Path_Channeler::ComputeCost( const MT_Vector2D& from, const MT_Vector2D& to, const TerrainData& /*nToTerrainType*/, const TerrainData& /*nLinkTerrainType*/ ) const
{
    const MT_Line line( from, to );
    if( location_.Intersect2D( line ) || location_.IsInside( to ) )
        return 0.;
    return 10.;
}
