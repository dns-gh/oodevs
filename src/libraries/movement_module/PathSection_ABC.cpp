//*****************************************************************************
//
// $Created: AGN 02-11-25 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/PathSection_ABC.cpp $
// $Author: Age $
// $Modtime: 12/05/05 16:18 $
// $Revision: 7 $
// $Workfile: PathSection_ABC.cpp $
//
//*****************************************************************************

#include "PathSection_ABC.h"
#include "PathResult.h"
#include "pathfind/TerrainRule_ABC.h"
#include "geometry/Types.h"
#include "wrapper/Hook.h"
#include <module_api/Log.h>
#include <ctime>

using namespace sword::movement;

typedef bool (*PathfindShouldEndComputation)( float rCostToCurrentNode, float rCostToGoalNode, void* userData );
typedef float (*PathfindEvaluateCost)( const geometry::Point2f& from, const geometry::Point2f& to, void* userData );
typedef float (*PathfindGetCost)( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween, void* userData );
typedef void (*PathfindHandlePathPoint)( const geometry::Point2f& point, const TerrainData& dataAtPoint, const TerrainData& dataToNextPoint, void* userData ); // $$$$ MCO 2012-05-23: be careful this is called in pathfind threads

DECLARE_HOOK( ComputePathfind, bool, ( TER_Pathfinder_ABC& pathfind, bool needRefine, bool strictClosest,
                                       const geometry::Point2f& from, const geometry::Point2f& to,
                                       PathfindEvaluateCost evaluate, void* evaluateData,
                                       PathfindGetCost get, void* getData,
                                       PathfindHandlePathPoint handler, void* handlerData,
                                       PathfindShouldEndComputation termination, void* terminationData ) ) // $$$$ MCO : or just have hooks on the other side as callbacks

// -----------------------------------------------------------------------------
// Name: PathSection_ABC constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
PathSection_ABC::PathSection_ABC( PathResult& path, const MT_Vector2D& startPoint, const MT_Vector2D& endPoint )
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
// Name: PathSection_ABC destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
PathSection_ABC::~PathSection_ABC()
{
    // NOTHING
}

namespace
{
    float RuleEvaluateCost( const geometry::Point2f& from, const geometry::Point2f& to, void* userData )
    {
        return static_cast< TerrainRule_ABC* >( userData )->EvaluateCost( from, to );
    }
    float RuleGetCost( const geometry::Point2f& from, const geometry::Point2f& to, const TerrainData& terrainTo, const TerrainData& terrainBetween, void* userData )
    {
        return static_cast< TerrainRule_ABC* >( userData )->GetCost( from, to, terrainTo, terrainBetween );
    }
    void PathSectionHandlePathPoint( const geometry::Point2f& point, const TerrainData& dataAtPoint, const TerrainData& dataToNextPoint, void* userData )
    {
        static_cast< PathSection_ABC* >( userData )->HandlePathPoint( MT_Vector2D( point.X(), point.Y() ), dataAtPoint, dataToNextPoint );
    }
    bool PathSectionShouldEndComputation( float rCostToCurrentNode, float rCostToGoalNode, void* userData )
    {
        return static_cast< PathSection_ABC* >( userData )->ShouldEndComputation( rCostToCurrentNode, rCostToGoalNode );
    }
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::Execute
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
bool PathSection_ABC::Execute( TER_Pathfinder_ABC& pathfind, unsigned int nComputationEndTime )
{
    nComputationEndTime_ = nComputationEndTime;
    const geometry::Point2f from( float( startPoint_.rX_ ), float( startPoint_.rY_ ) );
    const geometry::Point2f to( float( endPoint_.rX_ ), float( endPoint_.rY_ ) );
    return GET_HOOK( ComputePathfind )( pathfind, path_.NeedRefine(), path_.UseStrictClosest(), from, to,
                                              &RuleEvaluateCost, &GetRule(),
                                              &RuleGetCost, &GetRule(),
                                              &PathSectionHandlePathPoint, this,
                                              &PathSectionShouldEndComputation, this );
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::HandlePathPoint
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
void PathSection_ABC::HandlePathPoint( const MT_Vector2D& point, const TerrainData& dataAtPoint, const TerrainData& dataToNextPoint )
{
    ++ nAddedPoints_;
    path_.AddResultPoint( point, dataAtPoint, dataToNextPoint );
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::ShouldEndComputation
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
bool PathSection_ABC::ShouldEndComputation( float /*rCostToCurrentNode*/, float /*rCostToGoalNode*/ )
{
    if( bCanceled_ )
        return true;
    if( (unsigned int)time( 0 ) >= nComputationEndTime_ )
    {
        ::SWORD_Log( SWORD_LOG_LEVEL_ERROR, "Pathfind computation aborted - timeout" );
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::GetPosStart
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
const MT_Vector2D& PathSection_ABC::GetPosStart() const
{
    return startPoint_;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::GetPosEnd
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
const MT_Vector2D& PathSection_ABC::GetPosEnd() const
{
    return endPoint_;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::IsImpossible
// Created: AGE 2005-03-04
// -----------------------------------------------------------------------------
bool PathSection_ABC::IsImpossible() const
{
    return nAddedPoints_ < 2;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::Cancel
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
void PathSection_ABC::Cancel()
{
    bCanceled_ = true;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
double PathSection_ABC::GetLength() const
{
    return startPoint_.Distance( endPoint_ );
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::GetPath
// Created: CMA 2011-04-18
// -----------------------------------------------------------------------------
const PathResult& PathSection_ABC::GetPath() const
{
    return path_;
}

// -----------------------------------------------------------------------------
// Name: PathSection_ABC::SetPosStart
// Created: CMA 2011-04-18
// -----------------------------------------------------------------------------
void PathSection_ABC::SetPosStart( const MT_Vector2D& point )
{
    startPoint_ = point;
}
