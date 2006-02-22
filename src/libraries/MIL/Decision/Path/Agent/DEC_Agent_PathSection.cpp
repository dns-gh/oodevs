//*****************************************************************************
// 
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_Agent_PathSection.cpp $
// $Author: Age $
// $Modtime: 12/05/05 16:18 $
// $Revision: 7 $
// $Workfile: DEC_Agent_PathSection.cpp $
// 
//*****************************************************************************

#include "MIL_pch.h"

#include "DEC_Agent_PathSection.h"

#include "DEC_Agent_Path.h"
#include "DEC_Agent_PathfinderRule.h"

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathSection constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
DEC_Agent_PathSection::DEC_Agent_PathSection( DEC_Agent_Path& path, const MT_Vector2D& startPoint, const MT_Vector2D& endPoint )
    : DEC_PathSection_ABC( path, startPoint, endPoint )
    , rule_              ( *new DEC_Agent_PathfinderRule( path, startPoint, endPoint ) )
{  
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathSection destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_Agent_PathSection::~DEC_Agent_PathSection()
{
    delete &rule_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Agent_PathSection::GetRule
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
TerrainRule_ABC& DEC_Agent_PathSection::GetRule() const
{
    return rule_;
}

