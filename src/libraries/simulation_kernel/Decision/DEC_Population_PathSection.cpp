//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Population_PathSection.cpp $
// $Author: Age $
// $Modtime: 12/05/05 16:18 $
// $Revision: 7 $
// $Workfile: DEC_Population_PathSection.cpp $
//
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Population_PathSection.h"
#include "DEC_Population_PathfinderRule.h"
#include "DEC_Population_Path.h"

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathSection constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_Population_PathSection::DEC_Population_PathSection( DEC_Population_Path& path, const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint )
    : DEC_PathSection_ABC( path, vStartPoint, vEndPoint, false, false )
    , rule_( new DEC_Population_PathfinderRule( path ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathSection destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Population_PathSection::~DEC_Population_PathSection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathSection::GetRule
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
TerrainRule_ABC& DEC_Population_PathSection::GetRule() const
{
    return *rule_;
}
