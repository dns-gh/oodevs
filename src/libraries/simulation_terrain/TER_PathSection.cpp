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
#include "MT_Tools/MT_Logger.h"
#include <ctime>

// TerrainRule_ABC proxy handling computation timeouts.
class TER_PathSection::Canceler : public TerrainRule_ABC
{
public:
    Canceler( TerrainRule_ABC& rule )
        : rule_( rule )
        , stopTime_( 0 )
        , canceled_( false )
    {
    }

    void SetStopTime( unsigned int stopTime )
    {
        stopTime_ = stopTime;
    }

    // Note the difference between Cancel() and SetStopTime(0) is the former
    // will not write an "aborted" message in the log file. Cancelling queries
    // is part of pathfinding workflow.
    void Cancel()
    {
        canceled_ = true;
    }

    virtual float EvaluateCost( const geometry::Point2f& from,
                                const geometry::Point2f& to )
    {
        return rule_.EvaluateCost( from, to );
    }

    virtual float GetCost( const geometry::Point2f& from,
                           const geometry::Point2f& to,
                           const TerrainData& terrainTo,
                           const TerrainData& terrainBetween,
                           std::ostream* reason )
    {
        return rule_.GetCost( from, to, terrainTo, terrainBetween, reason );
    }

    virtual bool ShouldEndComputation()
    {
        if( canceled_ )
            return true;
        const auto now = static_cast< unsigned int >( time( 0 ) );
        if( now >= stopTime_ )
        {
            MT_LOG_ERROR_MSG( "Pathfind computation aborted - timeout" );
            canceled_ = true;
            return true;
        }
        return false;
    }

private: 
    TerrainRule_ABC& rule_;
    unsigned int stopTime_;
    bool canceled_;
};

// -----------------------------------------------------------------------------
// Name: TER_PathSection constructor
// Created: NLD 2005-02-22
// -----------------------------------------------------------------------------
TER_PathSection::TER_PathSection( std::unique_ptr< TerrainRule_ABC > rule,
    const MT_Vector2D& startPoint, const MT_Vector2D& endPoint, bool needRefine,
    bool useStrictClosest )
    : rule_               ( std::move( rule ) )
    , canceler_           ( new Canceler( *rule_ ) )
    , startPoint_         ( startPoint )
    , endPoint_           ( endPoint )
    , needRefine_         ( needRefine )
    , useStrictClosest_   ( useStrictClosest )
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
PathResultPtr TER_PathSection::Execute( TER_Pathfinder_ABC& pathfind,
        unsigned int nComputationEndTime )
{
    geometry::Point2f from( float( startPoint_.rX_ ), float( startPoint_.rY_ ) );
    geometry::Point2f to( float( endPoint_.rX_ ), float( endPoint_.rY_ ) );
    canceler_->SetStopTime( nComputationEndTime );
    if( needRefine_ )
        pathfind.SetConfiguration( 1, 3 ); // $$$$ AGE 2005-03-30: whatever
    pathfind.SetChoiceRatio( useStrictClosest_ ? 0.f : 0.1f );
    const auto res = pathfind.ComputePath( from, to, *canceler_ );
    pathfind.SetConfiguration( 0, 0 );
    return res;
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
// Name: TER_PathSection::Cancel
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
void TER_PathSection::Cancel()
{
    canceler_->Cancel();
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
