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

#include "MIL_pch.h"

#include "DEC_Population_PathSection.h"

#include "DEC_Population_PathfinderRule.h"
#include "DEC_Population_Path.h"

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathSection constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_Population_PathSection::DEC_Population_PathSection( DEC_Population_Path& path, const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint )
    : DEC_PathSection_ABC( path, vStartPoint, vEndPoint )
    , rule_              ( *new DEC_Population_PathfinderRule() )
{
    const DEC_Population_Path::T_PopulationPathChannelerVector& channelers = path.GetChannelers();
    for( DEC_Population_Path::CIT_PopulationPathChannelerVector it = channelers.begin(); it != channelers.end(); ++it )
        rule_.AddChanneler( *it );
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathSection destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Population_PathSection::~DEC_Population_PathSection()
{
    delete &rule_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathSection::GetRule
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
TerrainRule_ABC& DEC_Population_PathSection::GetRule() const
{
    return rule_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Population_PathSection::NeedRefine
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
bool DEC_Population_PathSection::NeedRefine() const
{
    return false;
}
