// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_PathfindManager2.h"
#include "TER_PathComputer_ABC.h"
#include "TER_PathFindManager.h"
#include "TER_PathFindRequest.h"
#include "TER_World.h"
#include "MT_Tools/MT_Profiler.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2 constructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
TER_PathfindManager2::TER_PathfindManager2( unsigned int threads,
        double distanceThreshold, double maxAvoidanceDistance,
        unsigned int maxEndConnections, unsigned int maxComputationDuration,
        const tools::Path& pathfindDir, const std::string& pathfindFilter )
    : nMaxComputationDuration_( maxComputationDuration )
    , rDistanceThreshold_     ( distanceThreshold )
    , treatedRequests_        ( 0 )
    , pathfindTime_           ( 0 )
{
    if( nMaxComputationDuration_ <= 0 )
        throw MASA_EXCEPTION( "pathfind maximum computation duration must be greater than zero");
    if( maxEndConnections <= 0 )
        throw MASA_EXCEPTION( "pathfind maximum end connections count must be greater than zero");
    bUseInSameThread_ = threads == 0;
    if( threads == 0 )
        threads = 1;
    for( unsigned i = 0; i < threads; ++i )
        pathFindThreads_.push_back(
            &TER_World::GetWorld().GetPathFindManager().CreatePathFinderThread(
                *this, maxEndConnections, maxAvoidanceDistance, bUseInSameThread_,
                pathfindDir, pathfindFilter ) );
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2 destructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
TER_PathfindManager2::~TER_PathfindManager2()
{
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::StartCompute
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void TER_PathfindManager2::StartCompute( const boost::shared_ptr< TER_PathComputer_ABC >& path, const sword::Pathfind& pathfind )
{
    auto p = boost::make_shared< TER_PathfindRequest >( *this, path, pathfind );
    boost::mutex::scoped_lock locker( mutex_ );
    if( path->GetLength() > rDistanceThreshold_ )
        longRequests_.push_back( p );
    else
        shortRequests_.push_back( p );
    condition_.notify_all();
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::GetNbrShortRequests
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
unsigned int TER_PathfindManager2::GetNbrShortRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return static_cast< unsigned int >( shortRequests_.size() );
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::GetNbrLongRequests
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
unsigned int TER_PathfindManager2::GetNbrLongRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return static_cast< unsigned int >( longRequests_.size() );
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::GetNbrTreatedRequests
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
unsigned int TER_PathfindManager2::GetNbrTreatedRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return treatedRequests_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::GetMessage
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathFindRequest_ABC > TER_PathfindManager2::GetMessage()
{
    unsigned int nIndex = 0;
    for( ; nIndex < pathFindThreads_.size(); ++nIndex )
        if( pathFindThreads_[ nIndex ]->IsCurrent() )
            break;
    return GetMessage( nIndex );
}

namespace
{
    static const unsigned maximumShortRequest = 5;
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::GetRequests
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
TER_PathfindManager2::T_Requests& TER_PathfindManager2::GetRequests()
{
    const bool shortHavePriority =
           ( ! shortRequests_.empty() )
           && ( pathFindThreads_.size() == 1 || shortRequests_.size() > maximumShortRequest );
    if( shortHavePriority || longRequests_.empty() )
        return shortRequests_;
    return longRequests_;
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::GetMessage
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathFindRequest_ABC > TER_PathfindManager2::GetMessage( unsigned int nThread )
{
    boost::shared_ptr< TER_PathFindRequest_ABC > pRequest;
    boost::mutex::scoped_lock locker( mutex_ );
    if( ( nThread % 2 ) )
    {
        condition_.wait( locker, [&]() { return !shortRequests_.empty(); } );
        pRequest = shortRequests_.front();
        shortRequests_.pop_front();
    }
    else
    {
        condition_.wait( locker, [&]() { return !shortRequests_.empty() || !longRequests_.empty(); } );
        T_Requests& requests = GetRequests();
        pRequest = requests.front();
        requests.pop_front();
    }
    ++treatedRequests_;
    return pRequest;
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::GetCurrentThread
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
int TER_PathfindManager2::GetCurrentThread() const
{
    for( unsigned int nIndex = 0; nIndex < pathFindThreads_.size(); ++nIndex )
        if( pathFindThreads_[ nIndex ]->IsCurrent() )
            return nIndex;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::CleanPathAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void TER_PathfindManager2::CleanPathAfterComputation( double duration )
{
    boost::mutex::scoped_lock locker( cleanAndDestroyMutex_ );
    toCleanup_.push_back( duration );
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::Update
// Created: NLD 2005-09-20
// -----------------------------------------------------------------------------
double TER_PathfindManager2::Update()
{
    boost::mutex::scoped_lock locker( cleanAndDestroyMutex_ );
    double pathfindTime = pathfindTime_;
    pathfindTime_ = 0;
    while( ! toCleanup_.empty() )
    {
        auto rq = toCleanup_.back();
        toCleanup_.pop_back();
        pathfindTime += rq;
    }
    return pathfindTime;
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::UpdateInSimulationThread
// Created: JSR 2010-06-16
// -----------------------------------------------------------------------------
void TER_PathfindManager2::UpdateInSimulationThread()
{
    if( bUseInSameThread_ ) // Pathfind in same thread than simulation
        while( ! shortRequests_.empty() || ! longRequests_.empty() )
        {
            T_Requests& requests = GetRequests();
            boost::shared_ptr< TER_PathFindRequest_ABC > pRequest = requests.front();
            requests.pop_front();
            pathFindThreads_[ 0 ]->ProcessInSimulationThread( pRequest );
            ++treatedRequests_;
        }
}

// -----------------------------------------------------------------------------
// Name: TER_PathfindManager2::GetMaxComputationDuration
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
unsigned int TER_PathfindManager2::GetMaxComputationDuration() const
{
    return nMaxComputationDuration_;
}
