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
#include "DEC_Path_ABC.h"
#include "DEC_PathFindRequest.h"
#include "DEC_PathType.h"
#include "DEC_Agent_PathClass.h"
#include "DEC_Population_PathClass.h"
#include "simulation_terrain/TER_PathFindManager.h"
#include "simulation_terrain/TER_World.h"
#include "Tools/MIL_Config.h"
#include "tools/PhyLoader.h"
#include "tools/Codec.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Profiler.h"
#include <boost/make_shared.hpp>
#include <boost/range/algorithm_ext/erase.hpp>

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager constructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
DEC_PathFind_Manager::DEC_PathFind_Manager( MIL_Config& config, double maxAvoidanceDistance,
                                            const std::vector< unsigned int >& dangerousObjects )
    : nMaxComputationDuration_( std::numeric_limits< unsigned int >::max() )
    , nMaxEndConnections_     ( 8 )
    , rDistanceThreshold_     ( 0 )
    , treatedRequests_        ( 0 )
    , pathfindTime_           ( 0 )
{
    config.GetPhyLoader().LoadPhysicalFile( "pathfinder",
        boost::bind( &DEC_PathFind_Manager::ReadPathfind, this, _1, boost::ref( config ), boost::cref( dangerousObjects ) ) );
    bUseInSameThread_ = config.GetPathFinderThreads() == 0;
    MT_LOG_INFO_MSG( MT_FormatString( "Starting %d pathfind thread(s)", config.GetPathFinderThreads() ) );
    const unsigned len = bUseInSameThread_ ? 1 : config.GetPathFinderThreads();
    for( unsigned i = 0; i < len; ++i )
        pathFindThreads_.push_back( &TER_World::GetWorld().GetPathFindManager().CreatePathFinderThread(
                                    *this, nMaxEndConnections_, maxAvoidanceDistance, bUseInSameThread_,
                                    config.GetPathfindDir(), config.GetPathfindFilter() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::ReadPathfind
// Created: LDC 2010-11-30
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::ReadPathfind( xml::xistream& xis, MIL_Config& config, const std::vector< unsigned int >& dangerousObjects )
{
    xis >> xml::start( "pathfind" )
            >> xml::start( "configuration" )
                >> xml::attribute( "distance-threshold", rDistanceThreshold_ );
    boost::optional< unsigned int > duration = config.GetPathFinderMaxComputationTime();
    if( duration )
        nMaxComputationDuration_ = *duration;
    else
        tools::ReadTimeAttribute( xis, "max-calculation-time", nMaxComputationDuration_ );
    if( nMaxComputationDuration_ <= 0 )
        throw MASA_EXCEPTION( "Pathfind configuration : max-calculation-time <= 0" );

    xis         >> xml::attribute( "max-end-connections", nMaxEndConnections_ )
            >> xml::end;
    if( nMaxEndConnections_ <= 0 )
        throw MASA_EXCEPTION( "Pathfind configuration : max-end-connections <= 0" );

    MT_LOG_INFO_MSG( "Setting pathfind.max-calculation-time=" << nMaxComputationDuration_ );
    DEC_PathType   ::Initialize();
    DEC_Agent_PathClass::Initialize( xis, dangerousObjects );
    DEC_Population_PathClass::Initialize( xis, dangerousObjects );
    xis >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager destructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
DEC_PathFind_Manager::~DEC_PathFind_Manager()
{
    DEC_Population_PathClass::Terminate();
    DEC_Agent_PathClass::Terminate();
    DEC_PathType::Terminate();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::StartCompute
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::StartCompute( const boost::shared_ptr< DEC_Path_ABC >& path )
{
    MT_LOG_DEBUG_MSG( MT_FormatString( "DEC_PathFind_Manager: New job pending : path 0x%p", path.get() ).c_str() );
    AddPendingJob( path );
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_Manager::CancelJobForUnit
// Created: JSR 2013-03-11
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::CancelJobForUnit( MIL_Agent_ABC* pion )
{
    std::vector< boost::shared_ptr< DEC_Path_ABC > > paths;
    const auto f =
        [&]( const boost::shared_ptr< DEC_PathFindRequest >& request ) -> bool
        {
            const auto path = request->GetPathForUnit( pion );
            if( !path )
                return false;
            paths.push_back( path );
            path->Destroy();
            return true;
        };
    {
        boost::mutex::scoped_lock locker( mutex_ );
        boost::remove_erase_if( longRequests_, f );
        boost::remove_erase_if( shortRequests_, f );
    }
    boost::mutex::scoped_lock locker( cleanAndDestroyMutex_ );
    for( auto it = paths.begin(); it != paths.end(); ++it )
        boost::remove_erase_if( toCleanup_,
            [&]( const T_Cleanups::value_type& v ) -> bool
            {
                if( v.first != *it )
                    return false;
                pathfindTime_ += v.second;
                return true;
            } );
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
// Name: DEC_PathFind_Manager::AddPendingJob
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void DEC_PathFind_Manager::AddPendingJob( const boost::shared_ptr< DEC_Path_ABC >& pPath )
{
    auto p = boost::make_shared< DEC_PathFindRequest >( this, pPath );
    boost::mutex::scoped_lock locker( mutex_ );
    if( pPath->GetLength() > rDistanceThreshold_ )
        longRequests_.push_back( p );
    else
        shortRequests_.push_back( p );
    condition_.notify_all();
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
void DEC_PathFind_Manager::CleanPathAfterComputation( const boost::shared_ptr< DEC_Path_ABC >& pPath, double duration )
{
    boost::mutex::scoped_lock locker( cleanAndDestroyMutex_ );
    toCleanup_.push_back( std::make_pair( pPath, duration ) );
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
        rq.first->CleanAfterComputation();
        pathfindTime += rq.second;
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
