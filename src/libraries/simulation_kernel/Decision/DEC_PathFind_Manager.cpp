// *****************************************************************************
//
// $Created: NLD 2003-08-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Decision/Path/DEC_PathFind_Manager.cpp $
// $Author: Age $
// $Modtime: 10/05/05 16:50 $
// $Revision: 15 $
// $Workfile: DEC_PathFind_Manager.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_PathFind_Manager.h"
#include "DEC_PathFindRequest.h"
#include "simulation_terrain/TER_PathComputer_ABC.h"
#include "simulation_terrain/TER_PathFindManager.h"
#include "simulation_terrain/TER_World.h"
#include "MT_Tools/MT_Profiler.h"
#include <boost/make_shared.hpp>

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager constructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
DEC_PathFind_Manager::DEC_PathFind_Manager( unsigned int threads,
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
// Name: DEC_PathFind_Manager destructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
DEC_PathFind_Manager::~DEC_PathFind_Manager()
{
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::StartCompute
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::StartCompute( const boost::shared_ptr< TER_PathComputer_ABC >& path, const sword::Pathfind& pathfind )
{
    auto p = boost::make_shared< DEC_PathFindRequest >( *this, path, pathfind );
    boost::mutex::scoped_lock locker( mutex_ );
    if( path->GetLength() > rDistanceThreshold_ )
        longRequests_.push_back( p );
    else
        shortRequests_.push_back( p );
    condition_.notify_all();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetNbrShortRequests
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
unsigned int DEC_PathFind_Manager::GetNbrShortRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return static_cast< unsigned int >( shortRequests_.size() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetNbrLongRequests
// Created: NLD 2005-04-01
// -----------------------------------------------------------------------------
unsigned int DEC_PathFind_Manager::GetNbrLongRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return static_cast< unsigned int >( longRequests_.size() );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetNbrTreatedRequests
// Created: AGE 2006-04-20
// -----------------------------------------------------------------------------
unsigned int DEC_PathFind_Manager::GetNbrTreatedRequests() const
{
    boost::mutex::scoped_lock locker( mutex_ );
    return treatedRequests_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetMessage
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathFindRequest_ABC > DEC_PathFind_Manager::GetMessage()
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
// Name: DEC_PathFind_Manager::GetRequests
// Created: AGE 2006-05-11
// -----------------------------------------------------------------------------
DEC_PathFind_Manager::T_Requests& DEC_PathFind_Manager::GetRequests()
{
    const bool shortHavePriority =
           ( ! shortRequests_.empty() )
           && ( pathFindThreads_.size() == 1 || shortRequests_.size() > maximumShortRequest );
    if( shortHavePriority || longRequests_.empty() )
        return shortRequests_;
    return longRequests_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::GetMessage
// Created: AGE 2005-02-25
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathFindRequest_ABC > DEC_PathFind_Manager::GetMessage( unsigned int nThread )
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
// Name: DEC_PathFind_Manager::GetCurrentThread
// Created: AGE 2005-02-28
// -----------------------------------------------------------------------------
int DEC_PathFind_Manager::GetCurrentThread() const
{
    for( unsigned int nIndex = 0; nIndex < pathFindThreads_.size(); ++nIndex )
        if( pathFindThreads_[ nIndex ]->IsCurrent() )
            return nIndex;
    return -1;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::CleanPathAfterComputation
// Created: NLD 2006-01-23
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::CleanPathAfterComputation( double duration )
{
    boost::mutex::scoped_lock locker( cleanAndDestroyMutex_ );
    toCleanup_.push_back( duration );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::Update
// Created: NLD 2005-09-20
// -----------------------------------------------------------------------------
double DEC_PathFind_Manager::Update()
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
// Name: DEC_PathFind_Manager::UpdateInSimulationThread
// Created: JSR 2010-06-16
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::UpdateInSimulationThread()
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
// Name: DEC_PathFind_Manager::GetMaxComputationDuration
// Created: NLD 2005-10-11
// -----------------------------------------------------------------------------
unsigned int DEC_PathFind_Manager::GetMaxComputationDuration() const
{
    return nMaxComputationDuration_;
}
