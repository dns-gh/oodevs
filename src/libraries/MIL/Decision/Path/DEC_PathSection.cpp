//*****************************************************************************
// 
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathSection.cpp $
// $Author: Age $
// $Modtime: 12/05/05 16:18 $
// $Revision: 7 $
// $Workfile: DEC_PathSection.cpp $
// 
//*****************************************************************************

#include "MIL_Pch.h"

#include "DEC_PathSection.h"
#include "DEC_Path.h"
#include "MIL_AgentServer.h"
#include "DEC_Pathfind_Manager.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "pathfind/TerrainPathfinder.h"
#include "pathfind/TerrainRule_ABC.h"

// -----------------------------------------------------------------------------
// Name: DEC_PathSection constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_PathSection::DEC_PathSection( DEC_Path& path, DEC_PathType pathType, const MT_Vector2D& vStartPoint, const MT_Vector2D& vEndPoint )
    : vStartPoint_( vStartPoint           )
    , vEndPoint_  ( vEndPoint             )
    , path_       ( path                  )
    , pRule_      ( 0 )
    , bCanceled_  ( false )
    , nAddedPoints_( 0 )
    , nComputationStart_( 0 )
    , nPendingRequests_( 0 )
{
    pRule_   = &pathType.CreateRule( path, vStartPoint, vEndPoint );
    bRefine_ = path.GetQueryMaker().CanFly() && ! path.GetQueryMaker().IsAutonomous();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_PathSection::~DEC_PathSection()
{
    delete pRule_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
MT_Float DEC_PathSection::GetLength() const
{
    return vStartPoint_.Distance( vEndPoint_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::Execute
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
bool DEC_PathSection::Execute( TerrainPathfinder& pathfind )
{
    geometry::Point2f from( float( vStartPoint_.rX_ ), float( vStartPoint_.rY_ ) );
    geometry::Point2f to( float( vEndPoint_.rX_ ), float( vEndPoint_.rY_ ) );
    nComputationStart_ = clock(); // $$$$ AGE 2005-02-28: something better than clock() ?
    nPendingRequests_  = MIL_AgentServer::GetWorkspace().GetPathFindManager().GetNbrRequests();
    if( bRefine_ )
        pathfind.SetPathfindConfiguration( 1, 3 ); // $$$$ AGE 2005-03-30: whatever
    pathfind.SetCallback( this );
    const bool bResult = pathfind.ComputePath( from, to, *pRule_, *this );
    pathfind.SetPathfindConfiguration( 0, 0 );
    pathfind.SetCallback( 0 );
    return bResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::IsImpossible
// Created: AGE 2005-03-04
// -----------------------------------------------------------------------------
bool DEC_PathSection::IsImpossible() const
{
    return nAddedPoints_ < 2;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::Cancel
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
void DEC_PathSection::Cancel()
{
    bCanceled_ = true;
}

// =============================================================================
// TOOLS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::ShouldEndComputation
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
bool DEC_PathSection::ShouldEndComputation( float /*rCostToCurrentNode*/, float /*rCostToGoalNode*/ )
{
    bCanceled_ = bCanceled_ || ComputationTimedOut();
    return bCanceled_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::Handle
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
void DEC_PathSection::Handle( const TerrainPathPoint& point )
{
    const geometry::Point2f p( point );
    ++ nAddedPoints_;
    path_.AddResultPoint( MT_Vector2D( p.X(), p.Y() ), point.DataAtPoint(), point.DataToNextPoint() );
}

// =============================================================================
// OPERATIONS
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::ComputationTimedOut
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
bool DEC_PathSection::ComputationTimedOut() const
{
    const MT_Float rLength = GetLength();
    MT_Float rMaxComputationTime = rLength / 40000.; // Base computation time : 1s for 40km
    // $$$$ AGE 2005-05-10: 
//    rMaxComputationTime /= ( nPendingRequests_ + 1 );
    const bool bTimeOut = clock() - nComputationStart_ > CLOCKS_PER_SEC * rMaxComputationTime;
    return false; // $$$$ AGE 2005-05-12: 
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::GetPosStart
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_PathSection::GetPosStart() const
{
    return vStartPoint_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::GetPosEnd
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_PathSection::GetPosEnd() const
{
    return vEndPoint_;
}
