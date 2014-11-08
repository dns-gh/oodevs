// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PathComputer.h"
#include "MIL_AgentServer.h"
#include "SlopeSpeedModifier.h"
#include "Tools/MIL_Config.h"
#include "meteo/PHY_MeteoDataManager.h"
#include "meteo/RawVisionData/Elevation.h"
#include "meteo/RawVisionData/PHY_RawVisionData.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Line.h"
#include "simulation_terrain/TER_PathPoint.h"
#include "simulation_terrain/TER_PathSection.h"
#include "simulation_terrain/TER_Pathfinder.h"
#include "simulation_terrain/TER_Pathfinder_ABC.h"
#include <pathfind/TerrainRule_ABC.h>
#include <boost/make_shared.hpp>

namespace
{

std::size_t computersId = 1;

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

DEC_PathComputer::DEC_PathComputer( std::size_t id )
    : id_( id )
    , computerId_( ::computersId++ )
    , nState_( TER_Path_ABC::eComputing )
{
    // NOTHING
}

DEC_PathComputer::~DEC_PathComputer()
{
}

boost::shared_ptr< TER_PathResult > DEC_PathComputer::GetPathResult() const
{
    const auto res = boost::make_shared< TER_PathResult >();
    res->state = nState_;
    res->lastWaypoint = lastWaypoint_;
    res->computedWaypoints = computedWaypoints_;
    res->points = resultList_;
    return res;
}

boost::shared_ptr< TER_PathResult > DEC_PathComputer::Execute(
        const std::vector< boost::shared_ptr< TER_PathSection > >& sections,
        TER_Pathfinder_ABC& pathfind, TER_PathFuture& future,
        unsigned int deadlineSeconds, bool debugPath )
{
    if( !resultList_.empty() )
        throw MASA_EXCEPTION( "List of path points is not empty before running pathfind" );
    if( debugPath )
    {
        MT_LOG_MESSAGE_MSG( "DEC_PathComputer::Execute: " << this << " computation begin" <<
                            ", Request: " << computerId_ <<
                            ", Entity: " << id_ );
        MT_LOG_MESSAGE_MSG( GetPathAsString( sections ) );
        profiler_.Start();
    }
    pathfind.SetId( id_ );
    try
    {
        DoExecute( sections, pathfind, future, deadlineSeconds );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "DEC_PathComputer::Execute failed: " << e.what() );
        nState_ = TER_Path_ABC::eCanceled;
        return GetPathResult();
    }
    if( debugPath )
    {
        const double rComputationTime = profiler_.Stop();
        std::stringstream stream;
        if( ! resultList_.empty() )
            stream << "[" << resultList_.front()->GetPos() << "] -> [" << resultList_.back()->GetPos() << "]";
        MT_LOG_MESSAGE_MSG( "DEC_PathComputer::Execute: " << this <<
                            ", Request: " << computerId_ <<
                            ", Time: " << rComputationTime <<
                            ", State: " << GetStateAsString( nState_ ) <<
                            ", Result: " << stream.str() );
    }
    return GetPathResult();
}

void DEC_PathComputer::DoExecute(
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
            const geometry::Point2f p( *ip );
            AddResultPoint( MT_Vector2D( p.X(), p.Y() ), ip->DataAtPoint(),
                    ip->DataToNextPoint(), ip == res->points.begin() );
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
        { computedWaypoints_.push_back( *last );
            NotifyCompletedSection();
        }
    }
    nState_ = TER_Path_ABC::eValid;
}

void DEC_PathComputer::AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint, bool beginPoint )
{
    if( beginPoint && !resultList_.empty() )
        // skip the first next point of the new section
        return;
    if( !resultList_.empty() && MIL_AgentServer::IsInitialized() )
    {
        SlopeSpeedModifier slopeSpeedModifier;
        const PHY_RawVisionData& elevation = MIL_AgentServer::GetWorkspace().GetMeteoDataManager().GetRawVisionData();
        auto decelerationFunc = boost::bind( &SlopeSpeedModifier::ComputeLocalSlope, &slopeSpeedModifier, boost::cref( elevation ), _1, _2 );
        const MT_Vector2D& startPoint = resultList_.back()->GetPos();
        SplitOnMajorGridLines( static_cast< int32_t >( elevation.GetCellSize() ),
                startPoint, vPos, decelerationFunc );
        const SlopeSpeedModifier::T_Slopes& slopes = slopeSpeedModifier.GetSlopes();
        for( auto itSlope = slopes.begin(); itSlope != slopes.end(); ++itSlope )
        {
            auto itPoint = resultList_.rbegin();
            while( itPoint != resultList_.rend() && !( *itPoint )->IsSlopeValid() )
                ( *itPoint++ )->SetSlope( itSlope->second );
            if( itSlope->second > 0 && itSlope + 1 != slopes.end() )
            {
                const MT_Line segment( startPoint, vPos );
                const MT_Vector2D projected = segment.ProjectPointOnLine( ( itSlope + 1 )->first );
                auto point = boost::make_shared< TER_PathPoint >( projected, nObjectTypes, nObjectTypesToNextPoint, beginPoint );
                resultList_.push_back( point );
            }
        }
    }
    auto point = boost::make_shared< TER_PathPoint >( vPos, nObjectTypes, nObjectTypesToNextPoint, beginPoint );
    resultList_.push_back( point );
}

void DEC_PathComputer::NotifyPartialSection()
{
    if( !resultList_.empty() )
        resultList_.back()->NotifyPartial();
}

void DEC_PathComputer::NotifyCompletedSection()
{
    if( !resultList_.empty() )
        resultList_.back()->NotifyWaypoint();
}

boost::optional< MT_Vector2D > DEC_PathComputer::GetLastPosition() const
{
    if( resultList_.empty() )
        return boost::none;
    return resultList_.back()->GetPos();
}

boost::shared_ptr< PathResult > DEC_PathComputer::ComputeSection(
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
