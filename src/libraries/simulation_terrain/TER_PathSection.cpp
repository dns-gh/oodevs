// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PathSection.h"
#include "TER_Pathfinder_ABC.h"
#include "TER_PathResult_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <ctime>

// -----------------------------------------------------------------------------
// Name: TER_PathSection constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
TER_PathSection::TER_PathSection( TER_PathResult_ABC& result, std::unique_ptr< TerrainRule_ABC > rule,
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
// Name: TER_PathSection destructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
TER_PathSection::~TER_PathSection()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::Execute
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
bool TER_PathSection::Execute( TER_Pathfinder_ABC& pathfind, unsigned int nComputationEndTime )
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
// Name: TER_PathSection::Handle
// Created: AGE 2005-02-24
// -----------------------------------------------------------------------------
void TER_PathSection::Handle( const TerrainPathPoint& point )
{
    const geometry::Point2f p( point );
    result_.AddResultPoint( MT_Vector2D( p.X(), p.Y() ), point.DataAtPoint(), point.DataToNextPoint(), nAddedPoints_ == 0u );
    ++ nAddedPoints_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::ShouldEndComputation
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
bool TER_PathSection::ShouldEndComputation( float /*rCostToCurrentNode*/, float /*rCostToGoalNode*/ )
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
// Name: TER_PathSection::GetPosStart
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
const MT_Vector2D& TER_PathSection::GetPosStart() const
{
    return startPoint_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::GetPosEnd
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
const MT_Vector2D& TER_PathSection::GetPosEnd() const
{
    return endPoint_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::IsImpossible
// Created: AGE 2005-03-04
// -----------------------------------------------------------------------------
bool TER_PathSection::IsImpossible() const
{
    return nAddedPoints_ < 2;
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::Cancel
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
void TER_PathSection::Cancel()
{
    bCanceled_ = true;
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::GetLength
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
double TER_PathSection::GetLength() const
{
    return startPoint_.Distance( endPoint_ );
}

// -----------------------------------------------------------------------------
// Name: TER_PathSection::SetPosStart
// Created: CMA 2011-04-18
// -----------------------------------------------------------------------------
void TER_PathSection::SetPosStart( const MT_Vector2D& point )
{
    startPoint_ = point;
}
