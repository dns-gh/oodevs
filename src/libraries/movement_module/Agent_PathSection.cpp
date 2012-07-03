//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/Agent_PathSection.cpp $
// $Author: Age $
// $Modtime: 12/05/05 16:18 $
// $Revision: 7 $
// $Workfile: Agent_PathSection.cpp $
//
//*****************************************************************************

#include <boost/shared_ptr.hpp>
#include <set>
#include "Agent_PathSection.h"
#include "Agent_Path.h"
#include "Agent_PathfinderRule.h"

using namespace sword::movement;

// -----------------------------------------------------------------------------
// Name: Agent_PathSection constructor
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
Agent_PathSection::Agent_PathSection( const sword::wrapper::View& entity, Agent_Path& path, const MT_Vector2D& startPoint, const MT_Vector2D& endPoint )
    : PathSection_ABC( path, startPoint, endPoint )
    , rule_          ( *new Agent_PathfinderRule( entity, path, startPoint, endPoint ) )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Agent_PathSection destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
Agent_PathSection::~Agent_PathSection()
{
    delete &rule_;
}

// -----------------------------------------------------------------------------
// Name: Agent_PathSection::GetRule
// Created: NLD 2005-09-30
// -----------------------------------------------------------------------------
TerrainRule_ABC& Agent_PathSection::GetRule() const
{
    return rule_;
}
