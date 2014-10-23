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

#include "simulation_kernel_pch.h"
#include "DEC_PathSection.h"
#include "DEC_PathResult_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include "simulation_terrain/TER_Pathfinder_ABC.h"
#include <ctime>

// -----------------------------------------------------------------------------
// Name: DEC_PathSection constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_PathSection::DEC_PathSection( DEC_PathResult_ABC& result, std::unique_ptr< TerrainRule_ABC > rule,
    const MT_Vector2D& startPoint, const MT_Vector2D& endPoint, bool needRefine, bool useStrictClosest )
    : result_             ( result  )
    , rule_               ( std::move( rule ) )
    , startPoint_         ( startPoint )
    , endPoint_           ( endPoint )
    , needRefine_         ( needRefine )
    , useStrictClosest_   ( useStrictClosest )
    , bCanceled_          ( false )
    , nComputationEndTime_( 0 )
    , nAddedPoints_       ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
DEC_PathSection::~DEC_PathSection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::Execute
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
bool DEC_PathSection::Execute( TER_Pathfinder_ABC& pathfind, unsigned int nComputationEndTime )
{
    geometry::Point2f from( float( startPoint_.rX_ ), float( startPoint_.rY_ ) );
    geometry::Point2f to( float( endPoint_.rX_ ), float( endPoint_.rY_ ) );
    nComputationEndTime_ = nComputationEndTime;
    if( needRefine_ )
        pathfind.SetConfiguration( 1, 3 ); // $$$$ AGE 2005-03-30: whatever
    pathfind.SetChoiceRatio( useStrictClosest_ ? 0.f : 0.1f );
    const bool bResult = pathfind.ComputePath( from, to, *rule_, this, *this );
    pathfind.SetConfiguration( 0, 0 );
    return bResult;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::Handle
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
void DEC_PathSection::Handle( const TerrainPathPoint& point )
{
    const geometry::Point2f p( point );
    result_.AddResultPoint( MT_Vector2D( p.X(), p.Y() ), point.DataAtPoint(), point.DataToNextPoint(), nAddedPoints_ == 0u );
    ++ nAddedPoints_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::ShouldEndComputation
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
bool DEC_PathSection::ShouldEndComputation( float /*rCostToCurrentNode*/, float /*rCostToGoalNode*/ )
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

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::GetPosStart
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_PathSection::GetPosStart() const
{
    return startPoint_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::GetPosEnd
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
const MT_Vector2D& DEC_PathSection::GetPosEnd() const
{
    return endPoint_;
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

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
double DEC_PathSection::GetLength() const
{
    return startPoint_.Distance( endPoint_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathSection::SetPosStart
// Created: CMA 2011-04-18
// -----------------------------------------------------------------------------
void DEC_PathSection::SetPosStart( const MT_Vector2D& point )
{
    startPoint_ = point;
}
