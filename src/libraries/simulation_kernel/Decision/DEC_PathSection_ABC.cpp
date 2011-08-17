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

#include "simulation_kernel_pch.h"
#include "DEC_PathSection_ABC.h"
#include "DEC_Path_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <ctime>

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_PathSection_ABC::DEC_PathSection_ABC( DEC_Path_ABC& path, const MT_Vector2D& startPoint, const MT_Vector2D& endPoint )
    : startPoint_         ( startPoint )
    , endPoint_           ( endPoint   )
    , path_               ( path  )
    , bCanceled_          ( false )
    , nAddedPoints_       ( 0 )
    , nComputationEndTime_( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_PathSection_ABC::~DEC_PathSection_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection_ABC::Execute
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
bool DEC_PathSection_ABC::Execute( TerrainPathfinder& pathfind, unsigned int nComputationEndTime )
{
    geometry::Point2f from( float( startPoint_.rX_ ), float( startPoint_.rY_ ) );
    geometry::Point2f to( float( endPoint_.rX_ ), float( endPoint_.rY_ ) );
    nComputationEndTime_ = nComputationEndTime;
    if( path_.NeedRefine() )
        pathfind.SetConfiguration( 1, 3 ); // $$$$ AGE 2005-03-30: whatever
    pathfind.SetChoiceRatio( path_.UseStrictClosest() ? 0.f : 0.1f );
    pathfind.SetCallback( this );
    const bool bResult = pathfind.ComputePath( from, to, GetRule(), *this );
    pathfind.SetConfiguration( 0, 0 );
    pathfind.SetCallback( 0 );
    return bResult;
}

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
    if( (unsigned int)time( 0 ) >= nComputationEndTime_ )
    {
        MT_LOG_ERROR_MSG( "Pathfind computation aborted - timeout" );
        return true;
    }
    return false;
}
