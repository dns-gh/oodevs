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
#include "simulation_terrain/TER_Pathfinder_ABC.h"
#include <pathfind/TerrainRule_ABC.h>
#include <boost/make_shared.hpp>

namespace
{

std::size_t computersId = 1;

}  // namespace

// TerrainRule_ABC proxy handling computation timeouts.
class DEC_PathComputer::Canceler : public TerrainRule_ABC
{
public:
    Canceler()
        : rule_( 0 )
        , stopTime_( 0 )
        , canceled_( false )
    {
    }

    void SetRule( TerrainRule_ABC* rule )
    {
        rule_ = rule;
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
        return rule_->EvaluateCost( from, to );
    }

    virtual float GetCost( const geometry::Point2f& from,
                           const geometry::Point2f& to,
                           const TerrainData& terrainTo,
                           const TerrainData& terrainBetween,
                           std::ostream* reason )
    {
        return rule_->GetCost( from, to, terrainTo, terrainBetween, reason );
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
    TerrainRule_ABC* rule_;
    unsigned int stopTime_;
    bool canceled_;
};

DEC_PathComputer::DEC_PathComputer( std::size_t id )
    : id_( id )
    , computerId_( ::computersId++ )
    , nState_( TER_Path_ABC::eComputing )
    , bJobCanceled_( false )
    , canceler_( new Canceler() )
{
    // NOTHING
}

DEC_PathComputer::~DEC_PathComputer()
{
    for( auto it = pathSections_.begin(); it != pathSections_.end(); ++it )
        delete *it;
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

double DEC_PathComputer::GetLength() const
{
    double length = 0;
    for( auto it = pathSections_.begin(); it != pathSections_.end(); ++it )
        length += (*it)->GetLength();
    return length;
}

boost::shared_ptr< TER_PathResult > DEC_PathComputer::Execute( TER_Pathfinder_ABC& pathfind,
        unsigned int deadlineSeconds, bool debugPath )
{
    if( !resultList_.empty() )
        throw MASA_EXCEPTION( "List of path points is not empty before running pathfind" );
    if( debugPath )
    {
        MT_LOG_MESSAGE_MSG( "DEC_PathComputer::Execute: " << this << " computation begin" <<
                            ", Request: " << computerId_ <<
                            ", Entity: " << id_ );
        MT_LOG_MESSAGE_MSG( GetPathAsString() );
        profiler_.Start();
    }
    pathfind.SetId( id_ );
    try
    {
        DoExecute( pathfind, deadlineSeconds );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "DEC_PathComputer::Execute failed: " << e.what() );
        bJobCanceled_ = true;
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
                            ", State: " << GetStateAsString() <<
                            ", Result: " << stream.str() );
    }
    return GetPathResult();
}

void DEC_PathComputer::DoExecute( TER_Pathfinder_ABC& pathfind, unsigned int deadlineSeconds )
{
    if( pathSections_.empty() )
        throw MASA_EXCEPTION( "List of path sections is empty" );
    canceler_->SetStopTime( deadlineSeconds );
    lastWaypoint_ = pathSections_.back()->GetPosEnd();
    computedWaypoints_.clear();
    nState_ = TER_Path_ABC::eComputing;
    for( auto it = pathSections_.begin(); it != pathSections_.end(); ++it )
    {
        if( bJobCanceled_ )
        {
            nState_ = TER_Path_ABC::eCanceled;
            return;
        }
        TER_PathSection& pathSection = **it;
        canceler_->SetRule( &pathSection.GetRule() );
        const auto res = pathSection.Execute( pathfind, *canceler_ );
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

            if( bJobCanceled_ )
            {
                nState_ = TER_Path_ABC::eCanceled;
                return;
            }
            else if( it == pathSections_.begin() && res->points.size() < 2 )
            {
                nState_ = TER_Path_ABC::eImpossible;
                return;
            }
            else
            {
                NotifyPartialSection();
                T_PathSectionVector::iterator itNextPathSection = it + 1;
                if( itNextPathSection == pathSections_.end() )
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

boost::shared_ptr< TER_PathResult > DEC_PathComputer::Cancel()
{
    bJobCanceled_ = true;
    canceler_->Cancel();
    nState_ = TER_Path_ABC::eCanceled;
    const auto res = boost::make_shared< TER_PathResult >();
    res->state = TER_Path_ABC::eCanceled;
    return res;
}

std::string DEC_PathComputer::GetStateAsString() const
{
    switch( nState_ )
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

std::string DEC_PathComputer::GetPathAsString() const
{
    std::stringstream strTmp;
    strTmp << "   Path points : " << pathSections_.front()->GetPosStart();
    for( auto itSection = pathSections_.begin(); itSection != pathSections_.end(); ++itSection )
        strTmp << " -> " << ( *itSection )->GetPosEnd();
    return strTmp.str();
}

void DEC_PathComputer::RegisterPathSection( TER_PathSection& section )
{
    pathSections_.push_back( &section );
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

