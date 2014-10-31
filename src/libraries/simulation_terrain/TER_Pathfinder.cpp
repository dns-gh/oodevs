// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "simulation_terrain_pch.h"
#include "TER_Pathfinder.h"
#include "TER_PathComputer_ABC.h"
#include "TER_Pathfinder.h"
#include "TER_PathFindRequest.h"
#include "TER_PathFinderThread.h"
#include "TER_World.h"
#include "MT_Tools/MT_Profiler.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder constructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
TER_Pathfinder::TER_Pathfinder( const boost::shared_ptr< TER_StaticData >& staticData,
        unsigned int threads, double distanceThreshold, double maxAvoidanceDistance,
        unsigned int maxEndConnections, unsigned int maxComputationDuration,
        const tools::Path& pathfindDir, const std::string& pathfindFilter )
    : staticData_( staticData )
    , nMaxComputationDuration_( maxComputationDuration )
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
    {
        threads_.push_back( new TER_PathFinderThread( *staticData_, *this,
            maxEndConnections, maxAvoidanceDistance, bUseInSameThread_,
            pathfindDir, pathfindFilter ) );
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder destructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
TER_Pathfinder::~TER_Pathfinder()
{
    for( auto it = threads_.begin(); it != threads_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::StartCompute
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void TER_Pathfinder::StartCompute( const boost::shared_ptr< TER_PathComputer_ABC >& path, const sword::Pathfind& pathfind )
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
// Name: TER_Pathfinder::GetNbrShortRequests
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
unsigned int TER_Pathfinder::GetNbrShortRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return static_cast< unsigned int >( shortRequests_.size() );
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::GetNbrLongRequests
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
unsigned int TER_Pathfinder::GetNbrLongRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return static_cast< unsigned int >( longRequests_.size() );
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::GetNbrTreatedRequests
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
unsigned int TER_Pathfinder::GetNbrTreatedRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return treatedRequests_;
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::GetMessage
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathfindRequest > TER_Pathfinder::GetMessage()
{
    unsigned int nIndex = 0;
    for( ; nIndex < threads_.size(); ++nIndex )
        if( threads_[ nIndex ]->IsCurrent() )
            break;
    return GetMessage( nIndex );
}

namespace
{
    static const unsigned maximumShortRequest = 5;
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::GetRequests
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
TER_Pathfinder::T_Requests& TER_Pathfinder::GetRequests()
{
    const bool shortHavePriority =
           ( ! shortRequests_.empty() )
           && ( threads_.size() == 1 || shortRequests_.size() > maximumShortRequest );
    if( shortHavePriority || longRequests_.empty() )
        return shortRequests_;
    return longRequests_;
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::GetMessage
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathfindRequest > TER_Pathfinder::GetMessage( unsigned int nThread )
{
    boost::shared_ptr< TER_PathfindRequest > pRequest;
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
// Name: TER_Pathfinder::CleanPathAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void TER_Pathfinder::CleanPathAfterComputation( double duration )
{
    boost::mutex::scoped_lock locker( cleanAndDestroyMutex_ );
    toCleanup_.push_back( duration );
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::Update
// Created: NLD 2005-09-20
// -----------------------------------------------------------------------------
double TER_Pathfinder::Update()
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
// Name: TER_Pathfinder::UpdateInSimulationThread
// Created: JSR 2010-06-16
// -----------------------------------------------------------------------------
void TER_Pathfinder::UpdateInSimulationThread()
{
    if( bUseInSameThread_ ) // Pathfind in same thread than simulation
        while( ! shortRequests_.empty() || ! longRequests_.empty() )
        {
            T_Requests& requests = GetRequests();
            boost::shared_ptr< TER_PathfindRequest > pRequest = requests.front();
            requests.pop_front();
            threads_[ 0 ]->ProcessInSimulationThread( pRequest );
            ++treatedRequests_;
        }
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::GetMaxComputationDuration
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
unsigned int TER_Pathfinder::GetMaxComputationDuration() const
{
    return nMaxComputationDuration_;
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::AddDynamicData
// Created: AGE 2005-10-07
// -----------------------------------------------------------------------------
void TER_Pathfinder::AddDynamicData( const DynamicDataPtr& data )
{
    for( auto it = threads_.begin(); it != threads_.end(); ++it )
        (*it)->AddDynamicDataToRegister( data );
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::RemoveDynamicData
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_Pathfinder::RemoveDynamicData( const DynamicDataPtr& data )
{
    for( auto it = threads_.begin(); it != threads_.end(); ++it )
        (*it)->AddDynamicDataToUnregister( data );
}
