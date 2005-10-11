//*****************************************************************************
// 
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathSection_ABC.cpp $
// $Author: Age $
// $Modtime: 12/05/05 16:18 $
// $Revision: 7 $
// $Workfile: DEC_PathSection_ABC.cpp $
// 
//*****************************************************************************

#include "MIL_pch.h"

#include "DEC_PathSection_ABC.h"

#include "DEC_Path_ABC.h"
#include "pathfind/TerrainPathfinder.h"

#include <ctime>

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_PathSection_ABC::DEC_PathSection_ABC( DEC_Path_ABC& path, const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint )
    : vStartPoint_        ( vStartPoint )
    , vEndPoint_          ( vEndPoint   )
    , path_               ( path  )
    , bCanceled_          ( false )
    , nAddedPoints_       ( 0 )
    , nComputationEndTime_( 0 )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_PathSection_ABC::~DEC_PathSection_ABC()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
MT_Float DEC_PathSection_ABC::GetLength() const
{
    return vStartPoint_.Distance( vEndPoint_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC::Execute
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
bool DEC_PathSection_ABC::Execute( TerrainPathfinder& pathfind, uint nComputationEndTime )
{
    geometry::Point2f from( float( vStartPoint_.rX_ ), float( vStartPoint_.rY_ ) );
    geometry::Point2f to( float( vEndPoint_.rX_ ), float( vEndPoint_.rY_ ) );

    nComputationEndTime_ = nComputationEndTime;
    if( NeedRefine() )
        pathfind.SetPathfindConfiguration( 1, 3 ); // $$$$ AGE 2005-03-30: whatever
    pathfind.SetCallback( this );
    const bool bResult = pathfind.ComputePath( from, to, GetRule(), *this );
    pathfind.SetPathfindConfiguration( 0, 0 );
    pathfind.SetCallback( 0 );
    return bResult;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC::Handle
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
void DEC_PathSection_ABC::Handle( const TerrainPathPoint& point )
{
    const geometry::Point2f p( point );
    ++ nAddedPoints_;
    path_.AddResultPoint( MT_Vector2D( p.X(), p.Y() ), point.DataAtPoint(), point.DataToNextPoint() );
}


// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC::ShouldEndComputation
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
bool DEC_PathSection_ABC::ShouldEndComputation( float /*rCostToCurrentNode*/, float /*rCostToGoalNode*/ )
{
    if( bCanceled_ )
        return true;

    if( (uint)time( 0 ) >= nComputationEndTime_ )
        return true;

    return false;
}

