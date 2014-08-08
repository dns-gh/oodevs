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
#include "DEC_PathFind_Manager.h"
#include "DEC_PathSection.h"
#include "DEC_PathPoint.h"
#include "MIL_AgentServer.h"
#include "SlopeSpeedModifier.h"
#include "Network/NET_ASN_Tools.h"
#include "Tools/MIL_Config.h"
#include "meteo/PHY_MeteoDataManager.h"
#include "meteo/RawVisionData/Elevation.h"
#include "meteo/RawVisionData/PHY_RawVisionData.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Line.h"
#include "protocol/Protocol.h"
#include "simulation_terrain/TER_Pathfinder_ABC.h"
#include <boost/make_shared.hpp>

DEC_PathComputer::DEC_PathComputer( std::size_t id )
    : id_( id )
    , nState_( DEC_Path_ABC::eComputing )
    , bJobCanceled_( false )
    , bSectionJustStarted_( false )
{
    // NOTHING
}

DEC_PathComputer::~DEC_PathComputer()
{
    for( auto it = pathSections_.begin(); it != pathSections_.end(); ++it )
        delete *it;
}

double DEC_PathComputer::GetLength() const
{
    double length = 0;
    for( auto it = pathSections_.begin(); it != pathSections_.end(); ++it )
        length += (*it)->GetLength();
    return length;
}

void DEC_PathComputer::Execute( TER_Pathfinder_ABC& pathfind )
{
    if( !resultList_.empty() )
        throw MASA_EXCEPTION( "List of path points is not empty before running pathfind" );
    if( MIL_AgentServer::IsInitialized() && MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        MT_LOG_MESSAGE_MSG( "DEC_PathComputer::Execute: " << this << " computation begin" <<
                            ", Thread    : " << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() <<
                            ", Entity    : " << id_ );
        MT_LOG_MESSAGE_MSG( GetPathAsString() );
        profiler_.Start();
    }
    pathfind.SetId( id_ );
    try
    {
        DoExecute( pathfind );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "DEC_PathComputer::Execute failed: " << e.what() );
        bJobCanceled_ = true;
        nState_ = DEC_Path_ABC::eCanceled;
        return;
    }
    if( MIL_AgentServer::IsInitialized() && MIL_AgentServer::GetWorkspace().GetConfig().UsePathDebug() )
    {
        const double rComputationTime = profiler_.Stop();
        std::stringstream stream;
        if( ! resultList_.empty() )
            stream << "[" << resultList_.front()->GetPos() << "] -> [" << resultList_.back()->GetPos() << "]";
        MT_LOG_MESSAGE_MSG( "DEC_PathComputer::Execute: " << this <<
                            ", Thread : " << MIL_AgentServer::GetWorkspace().GetPathFindManager().GetCurrentThread() <<
                            ", Time : " << rComputationTime <<
                            ", State : " << GetStateAsString() <<
                            ", Result : " << stream.str() );
    }
}

void DEC_PathComputer::DoExecute( TER_Pathfinder_ABC& pathfind )
{
    unsigned int nComputationEndTime = 0;
    const unsigned int nMaxComputationDuration = MIL_AgentServer::IsInitialized() ? MIL_AgentServer::GetWorkspace().GetPathFindManager().GetMaxComputationDuration() : std::numeric_limits< unsigned int >::max();
    if( nMaxComputationDuration == std::numeric_limits< unsigned int >::max() )
        nComputationEndTime = std::numeric_limits< unsigned int >::max();
    else
        nComputationEndTime = static_cast< unsigned int >( time( 0 ) ) + nMaxComputationDuration;

    if( pathSections_.empty() )
        throw MASA_EXCEPTION( "List of path sections is empty" );
    lastWaypoint_ = pathSections_.back()->GetPosEnd();
    computedWaypoints_.clear();
    nState_ = DEC_Path_ABC::eComputing;
    for( auto it = pathSections_.begin(); it != pathSections_.end(); ++it )
    {
        if( bJobCanceled_ )
        {
            nState_ = DEC_Path_ABC::eCanceled;
            return;
        }
        DEC_PathSection& pathSection = **it;
        NotifySectionStarted();
        if( !pathSection.Execute( pathfind, nComputationEndTime ) )
        {
            if( auto last = GetLastPosition() )
                computedWaypoints_.push_back( *last );

            if( bJobCanceled_ )
            {
                nState_ = DEC_Path_ABC::eCanceled;
                return;
            }
            else if( it == pathSections_.begin() && pathSection.IsImpossible() )
            {
                nState_ = DEC_Path_ABC::eImpossible;
                return;
            }
            else
            {
                NotifyPartialSection();
                T_PathSectionVector::iterator itNextPathSection = it + 1;
                if( itNextPathSection == pathSections_.end() )
                {
                    nState_ = DEC_Path_ABC::ePartial;
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
    nState_ = DEC_Path_ABC::eValid;
}

void DEC_PathComputer::Cancel()
{
    bJobCanceled_ = true;
    nState_ = DEC_Path_ABC::eCanceled;
    for( auto it = pathSections_.begin(); it != pathSections_.end(); ++it )
        ( *it )->Cancel();
}

DEC_Path_ABC::E_State DEC_PathComputer::GetState() const
{
    return nState_;
}

namespace
{
    sword::TerrainData Convert( const TerrainData& point )
    {
        sword::TerrainData data;
        data.set_linear( point.Linear() );
        data.set_area( point.Area() );
        data.set_left( point.Left() );
        data.set_right( point.Right() );
        return data;
    }
}

void DEC_PathComputer::Serialize( sword::PathResult& msg ) const
{
    unsigned int index = 0;
    for( auto it = resultList_.begin(); it != resultList_.end(); ++it )
    {
        auto point = msg.add_points();
        const MT_Vector2D& position = (*it)->GetPos();
        const bool partial = (*it)->IsPartial();
        if( (*it)->IsWaypoint() || partial )
        {
            point->set_waypoint( index++ );
            point->set_reached( !partial );
        }
        *point->mutable_current() = Convert( (*it)->GetObjectTypes() );
        *point->mutable_next() = Convert( (*it)->GetObjectTypesToNextPoint() );
        NET_ASN_Tools::WritePoint( position, *point->mutable_coordinate() );
    }
}

std::string DEC_PathComputer::GetStateAsString() const
{
    switch( nState_ )
    {
        case DEC_Path_ABC::eInvalid    : return "Invalid";
        case DEC_Path_ABC::eComputing  : return "Computing";
        case DEC_Path_ABC::eValid      : return "Valid";
        case DEC_Path_ABC::eImpossible : return "Impossible";
        case DEC_Path_ABC::ePartial    : return "Partial";
        case DEC_Path_ABC::eCanceled   : return "Canceled";
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

void DEC_PathComputer::RegisterPathSection( DEC_PathSection& section )
{
    pathSections_.push_back( &section );
}

const MT_Vector2D& DEC_PathComputer::GetLastWaypoint() const
{
    return lastWaypoint_;
}

void DEC_PathComputer::RemoveComputedWaypoint()
{
    if( !computedWaypoints_.empty() )
        computedWaypoints_.erase( computedWaypoints_.begin() );
}

const std::vector< MT_Vector2D >& DEC_PathComputer::GetComputedWaypoints() const
{
    return computedWaypoints_;
}

void DEC_PathComputer::AddResultPoint( const MT_Vector2D& vPos, const TerrainData& nObjectTypes, const TerrainData& nObjectTypesToNextPoint, bool beginPoint )
{
    if( bSectionJustStarted_ )
    {
        bSectionJustStarted_ = false;
        // skip the first next point of the new section
        if( !resultList_.empty() )
            return;
    }
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
                auto point = boost::make_shared< DEC_PathPoint >( projected, nObjectTypes, nObjectTypesToNextPoint, beginPoint );
                resultList_.push_back( point );
            }
        }
    }
    auto point = boost::make_shared< DEC_PathPoint >( vPos, nObjectTypes, nObjectTypesToNextPoint, beginPoint );
    resultList_.push_back( point );
}

void DEC_PathComputer::NotifySectionStarted()
{
    bSectionJustStarted_ = true;
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

DEC_PathComputer::T_PathPoints DEC_PathComputer::GetResult() const
{
    return resultList_;
}
