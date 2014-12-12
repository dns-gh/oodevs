// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PathComputer.h"
#include "TER_PathPoint.h"
#include "TER_PathSection.h"
#include "TER_Pathfinder.h"
#include "TER_Pathfinder_ABC.h"
#include "MT_Tools/MT_Logger.h"
#include <pathfind/TerrainRule_ABC.h>
#include <boost/make_shared.hpp>

namespace
{

std::string GetStateAsString( TER_Path_ABC::E_State state )
{
    switch( state )
    {
        case TER_Path_ABC::eInvalid    : return "Invalid";
        case TER_Path_ABC::eComputing  : return "Computing";
        case TER_Path_ABC::eValid      : return "Valid";
        case TER_Path_ABC::eImpossible : return "Impossible";
        case TER_Path_ABC::ePartial    : return "Partial";
        case TER_Path_ABC::eCanceled   : return "Canceled";
        default                        : return "UNKNOWN";
    }
}

std::string GetPathAsString(
    const std::vector< boost::shared_ptr< TER_PathSection > >& sections )
{
    std::stringstream strTmp;
    strTmp << "   Path points : ";
    if( !sections.empty() )
    {
       strTmp << sections.front()->GetPosStart();
        for( auto itSection = sections.begin(); itSection != sections.end(); ++itSection )
            strTmp << " -> " << ( *itSection )->GetPosEnd();
    }
    return strTmp.str();
}

// TerrainRule_ABC proxy handling computation timeouts.
class Canceler : public TerrainRule_ABC
{
public:
    Canceler( TerrainRule_ABC& rule, TER_PathFuture& future, unsigned int stopTime )
        : rule_( rule )
        , future_( future )
        , stopTime_( stopTime )
    {
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
        if( future_.IsCanceled() )
            return true;
        const auto now = static_cast< unsigned int >( time( 0 ) );
        if( now >= stopTime_ )
        {
            MT_LOG_ERROR_MSG( "Pathfind computation aborted - timeout" );
            future_.Cancel();
            return true;
        }
        return false;
    }

private: 
    TerrainRule_ABC& rule_;
    TER_PathFuture& future_;
    const unsigned int stopTime_;
};

}  // namespace

TER_PathComputer::TER_PathComputer()
    : nState_( TER_Path_ABC::eComputing )
{
    // NOTHING
}

TER_PathComputer::~TER_PathComputer()
{
}

boost::shared_ptr< TER_PathResult > TER_PathComputer::GetPathResult() const
{
    const auto res = boost::make_shared< TER_PathResult >();
    res->state = nState_;
    res->lastWaypoint = lastWaypoint_;
    res->computedWaypoints = computedWaypoints_;
    res->points = resultList_;
    return res;
}

boost::shared_ptr< TER_PathResult > TER_PathComputer::Execute(
        std::size_t requestId, std::size_t callerId,
        const std::vector< boost::shared_ptr< TER_PathSection > >& sections,
        TER_Pathfinder_ABC& pathfind, TER_PathFuture& future,
        unsigned int deadlineSeconds, bool debugPath )
{
    if( debugPath )
    {
        MT_LOG_MESSAGE_MSG( "TER_PathComputer[" << requestId << "] begin entity: "
                << callerId );
        MT_LOG_MESSAGE_MSG( "TER_PathComputer[" << requestId << "] sections: "
                << GetPathAsString( sections ) );
        profiler_.Start();
    }
    try
    {
        DoExecute( sections, pathfind, future, deadlineSeconds );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "TER_PathComputer[" << requestId << "] failed: " << e.what() );
        nState_ = TER_Path_ABC::eCanceled;
        return GetPathResult();
    }
    if( debugPath )
    {
        const double rComputationTime = profiler_.Stop();
        std::stringstream stream;
        if( ! resultList_.empty() )
            stream << "[" << resultList_.front()->GetPos() << "] -> [" << resultList_.back()->GetPos() << "]";
        MT_LOG_MESSAGE_MSG( "TER_PathComputer[" << requestId << "] duration: "
                            << rComputationTime
                            << ", state: " << GetStateAsString( nState_ )
                            << ", result: " << stream.str() );
    }
    return GetPathResult();
}

void TER_PathComputer::DoExecute(
        const std::vector< boost::shared_ptr< TER_PathSection > >& sections,
        TER_Pathfinder_ABC& pathfind, TER_PathFuture& future,
        unsigned int deadlineSeconds )
{
    if( sections.empty() )
        throw MASA_EXCEPTION( "List of path sections is empty" );
    lastWaypoint_ = sections.back()->GetPosEnd();
    computedWaypoints_.clear();
    nState_ = TER_Path_ABC::eComputing;
    for( auto it = sections.begin(); it != sections.end(); ++it )
    {
        TER_PathSection& pathSection = **it;
        const auto res = ComputeSection( pathfind, pathSection, future, deadlineSeconds );
        for( auto ip = res->points.begin(); ip != res->points.end(); ++ip )
        {
            const auto first = ip == res->points.begin();
            if( first && !resultList_.empty() )
                // skip the first next point of the new section
                continue;
            const geometry::Point2f p( *ip );
            const auto point = boost::make_shared< TER_PathPoint >(
                MT_Vector2D( ip->Point().X(), ip->Point().Y() ), ip->DataAtPoint(),
                ip->DataToNextPoint(), ip == res->points.begin() );
            resultList_.push_back( point );
        }

        if( !res->found )
        {
            if( auto last = GetLastPosition() )
                computedWaypoints_.push_back( *last );

            if( future.IsCanceled() )
            {
                nState_ = TER_Path_ABC::eCanceled;
                return;
            }
            else if( it == sections.begin() && res->points.size() < 2 )
            {
                nState_ = TER_Path_ABC::eImpossible;
                return;
            }
            else
            {
                NotifyPartialSection();
                auto itNextPathSection = it + 1;
                if( itNextPathSection == sections.end() )
                {
                    nState_ = TER_Path_ABC::ePartial;
                    return;
                }
                else
                    (*itNextPathSection)->SetPosStart( *GetLastPosition() );
            }
        }
        else if( auto last = GetLastPosition() )
        {
            computedWaypoints_.push_back( *last );
            NotifyCompletedSection();
        }
    }
    nState_ = TER_Path_ABC::eValid;
}

void TER_PathComputer::NotifyPartialSection()
{
    if( !resultList_.empty() )
        resultList_.back()->NotifyPartial();
}

void TER_PathComputer::NotifyCompletedSection()
{
    if( !resultList_.empty() )
        resultList_.back()->NotifyWaypoint();
}

boost::optional< MT_Vector2D > TER_PathComputer::GetLastPosition() const
{
    if( resultList_.empty() )
        return boost::none;
    return resultList_.back()->GetPos();
}

boost::shared_ptr< PathResult > TER_PathComputer::ComputeSection(
    TER_Pathfinder_ABC& pathfind, TER_PathSection& section, TER_PathFuture& future,
    unsigned int deadlineSeconds )
{
    const auto start = section.GetPosStart();
    const auto end = section.GetPosEnd();
    geometry::Point2f from( float( start.rX_ ), float( start.rY_ ) );
    geometry::Point2f to( float( end.rX_ ), float( end.rY_ ) );
    if( section.NeedRefine() )
        pathfind.SetConfiguration( 1, 3 ); // $$$$ AGE 2005-03-30: whatever
    Canceler canceler( section.GetRule(), future, deadlineSeconds );
    pathfind.SetChoiceRatio( section.UseStrictClosest() ? 0.f : 0.1f );
    const auto res = pathfind.ComputePath( from, to, canceler );
    pathfind.SetConfiguration( 0, 0 );
    return res;
}
