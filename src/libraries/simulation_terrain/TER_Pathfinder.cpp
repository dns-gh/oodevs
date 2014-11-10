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
#include "TER_EdgeMatcher.h"
#include "TER_PathComputer.h"
#include "TER_Pathfinder.h"
#include "TER_Pathfinder_ABC.h"
#include "TER_PathFindRequest.h"
#include "TER_PathFinderThread.h"
#include "TER_World.h"
#include "MT_Tools/MT_Logger.h"
#include "MT_Tools/MT_Profiler.h"
#include "protocol/Simulation.h"
#include <pathfind/PathfindFileDumper.h>
#include <pathfind/TerrainPathfinder.h>
#include <boost/interprocess/detail/atomic.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/tokenizer.hpp>

namespace bii = boost::interprocess::ipcdetail;

namespace
{

std::set< size_t > ParseFilter( const std::string& filter )
{
    std::set< size_t > reply;
    boost::tokenizer< boost::escaped_list_separator< char > > tokens( filter );
    for( auto it = tokens.begin(); it != tokens.end(); ++it )
        reply.insert( boost::lexical_cast< size_t >( *it ) );
    return reply;
}

struct PathfindDumper : public TER_Pathfinder_ABC
                      , public boost::noncopyable
{
    PathfindDumper( const tools::Path& dump, const std::set< size_t >& filter,
                     const boost::shared_ptr< TerrainPathfinder >& root )
        : dump_  ( dump )
        , filter_( filter )
        , root_  ( root )
        , id_    ( 0 )
    {
        // NOTHING
    }
    virtual void SetId( size_t id )
    {
        id_ = id;
    }
    virtual void SetChoiceRatio( float ratio )
    {
        root_->SetChoiceRatio( ratio );
    }
    virtual void SetConfiguration( unsigned nRefining, unsigned int nSubdivisions )
    {
        root_->SetConfiguration( nRefining, nSubdivisions );
    }
    virtual PathResultPtr ComputePath( const geometry::Point2f& from,
                              const geometry::Point2f& to,
                              TerrainRule_ABC& rule )
    {
        const bool dump = !dump_.IsEmpty() &&
            ( filter_.empty() || filter_.count( id_ ) );
        if( dump )
        {
            PathfindFileDumper dumper( GetFilename(), rule );
            return root_->ComputePath( from, to, dumper );
        }
        return root_->ComputePath( from, to, rule );
    }
private:
    tools::Path GetFilename() const
    {
        std::stringstream name;
        name << "pathfind_"
             << id_
             << "_"
             << bii::atomic_inc32( &s_idx_ );
        return dump_ / name.str().c_str();
    }
private:
    static boost::uint32_t    s_idx_;
    const tools::Path&        dump_;
    const std::set< size_t >& filter_;
    boost::shared_ptr< TerrainPathfinder> root_;
    size_t                    id_;
};

boost::uint32_t PathfindDumper::s_idx_ = 0;

}  // namespace

TER_PathFuture::TER_PathFuture()
    : canceled_( false )
{
}

TER_PathFuture::~TER_PathFuture()
{
}

void TER_PathFuture::Set( const boost::shared_ptr< TER_PathResult >& path )
{
    boost::mutex::scoped_lock lock( mutex_ );
    if( !canceled_ )
        path_ = path;
}

boost::shared_ptr< TER_PathResult > TER_PathFuture::Get() const
{
    boost::mutex::scoped_lock lock( mutex_ );
    return path_;
}

void TER_PathFuture::Cancel()
{
    boost::mutex::scoped_lock lock( mutex_ );
    if( !canceled_ )
    {
        canceled_ = true;
        path_ = boost::make_shared< TER_PathResult >();
        path_->state = TER_Path_ABC::eCanceled;
    }
}

bool TER_PathFuture::IsCanceled() const
{
    // What is important is canceled computations appear canceled from the
    // client point of view. The computation does not have to end immediately
    // or even end at all as long as it appears canceled. But IsCanceled()
    // has to be fast, it is called repeatedly in pathfinder main loop.
    return canceled_;
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder constructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
TER_Pathfinder::TER_Pathfinder( const boost::shared_ptr< TER_StaticData >& staticData,
        unsigned int threads, double distanceThreshold, double maxAvoidanceDistance,
        unsigned int maxEndConnections, unsigned int maxComputationDuration,
        const tools::Path& pathfindDir, const std::string& pathfindFilter,
        bool debugPath )
    : staticData_( staticData )
    , dumpDir_( pathfindDir )
    , dumpFilter_( ParseFilter( pathfindFilter ) )
    , debugPath_( debugPath )
    , queryId_( 1 )
    , nMaxComputationDuration_( maxComputationDuration )
    , rDistanceThreshold_     ( distanceThreshold )
    , treatedRequests_        ( 0 )
    , stopped_                ( false )
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
        pathfindData_.push_back( std::unique_ptr< TER_PathFinderThread >(
            new TER_PathFinderThread( *staticData_, maxEndConnections,
                maxAvoidanceDistance ) ) );
        if( bUseInSameThread_ )
            continue;
        auto* data = pathfindData_.back().get();
        threads_.push_back( std::unique_ptr< boost::thread >( new boost::thread( [&,i,data]()
        {
            for( ;; )
            {
                const auto rq = GetMessage( i );
                if( !rq )
                    break;
                ProcessRequest( *data, *rq );
            }
        })));
    }
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder destructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
TER_Pathfinder::~TER_Pathfinder()
{
    {
        boost::mutex::scoped_lock locker( mutex_ );
        stopped_ = true;
    }
    condition_.notify_all();
    threads_.clear();
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::StartCompute
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
boost::shared_ptr< TER_PathFuture > TER_Pathfinder::StartCompute(
        std::size_t callerId,
        const std::vector< boost::shared_ptr< TER_PathSection > > sections,
        const sword::Pathfind& pathfind )
{
    // $$$$$ PMD: storing the callback in the request is not elegant but harmless
    // for now as the request object is private to the pathfinder. Make a local
    // struct later.
    const auto future = boost::make_shared< TER_PathFuture >();
    const auto p = boost::make_shared< TER_PathfindRequest >(
            queryId_++, callerId, sections, pathfind, future );
    boost::mutex::scoped_lock locker( mutex_ );
    if( p->GetLength() > rDistanceThreshold_ )
        longRequests_.push_back( p );
    else
        shortRequests_.push_back( p );
    condition_.notify_all();
    return future;
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

namespace
{
    const unsigned maximumShortRequest = 5;
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
        condition_.wait( locker, [&]()
        {
            return stopped_ || !shortRequests_.empty();
        });
        if( stopped_ )
            return pRequest;
        pRequest = shortRequests_.front();
        shortRequests_.pop_front();
    }
    else
    {
        condition_.wait( locker, [&]()
        {
            return stopped_ || !shortRequests_.empty() || !longRequests_.empty();
        });
        if( stopped_ )
            return pRequest;
        T_Requests& requests = GetRequests();
        pRequest = requests.front();
        requests.pop_front();
    }
    ++treatedRequests_;
    return pRequest;
}

void TER_Pathfinder::ProcessRequest( TER_PathFinderThread& data, TER_PathfindRequest& rq )
{
    const unsigned int deadline = nMaxComputationDuration_ == std::numeric_limits< unsigned int >::max()
        ? std::numeric_limits< unsigned int >::max()
        : static_cast< unsigned int >( std::time( 0 ) ) + nMaxComputationDuration_;
    double duration = 0;
    try
    {
        data.ProcessDynamicData();
        const auto pathfinder = data.GetPathfinder( !rq.IgnoreDynamicObjects() );
        boost::shared_ptr< TER_Pathfinder_ABC > wrapper =
            boost::make_shared< PathfindDumper >( dumpDir_, dumpFilter_, pathfinder );
        MT_Profiler profiler;
        profiler.Start();
        if( rq.IsItinerary() )
            wrapper = boost::make_shared< TER_EdgeMatcher >( wrapper, rq.GetPathfind() );
        const auto res = TER_PathComputer().Execute( rq.GetQueryId(), rq.GetCallerId(),
                rq.GetSections(), *wrapper, *rq.GetFuture(), deadline, debugPath_ );
        rq.GetFuture()->Set( res );
        duration = profiler.Stop();
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( "Exception caught in pathfinder thread : "
                << tools::GetExceptionMsg( e ) );
        rq.GetFuture()->Cancel();
    }
    catch( ... )
    {
        MT_LOG_ERROR_MSG( "Unknown exception caught in pathfinder thread" );
        rq.GetFuture()->Cancel();
    }

    boost::mutex::scoped_lock locker( pathfindTimeMutex_ );
    pathfindTime_ += duration;
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::Update
// Created: NLD 2005-09-20
// -----------------------------------------------------------------------------
double TER_Pathfinder::Update()
{
    boost::mutex::scoped_lock locker( pathfindTimeMutex_ );
    double pathfindTime = pathfindTime_;
    pathfindTime_ = 0;
    return pathfindTime;
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::UpdateInSimulationThread
// Created: JSR 2010-06-16
// -----------------------------------------------------------------------------
void TER_Pathfinder::UpdateInSimulationThread()
{
    if( !bUseInSameThread_ )
         // Pathfind in same thread as simulation
        return;
    while( !stopped_ && ( ! shortRequests_.empty() || ! longRequests_.empty() ) )
    {
        T_Requests& requests = GetRequests();
        boost::shared_ptr< TER_PathfindRequest > pRequest = requests.front();
        requests.pop_front();
        ProcessRequest( *pathfindData_.front(), *pRequest );
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
    for( auto it = pathfindData_.begin(); it != pathfindData_.end(); ++it )
        (*it)->AddDynamicDataToRegister( data );
}

// -----------------------------------------------------------------------------
// Name: TER_Pathfinder::RemoveDynamicData
// Created: NLD 2005-10-10
// -----------------------------------------------------------------------------
void TER_Pathfinder::RemoveDynamicData( const DynamicDataPtr& data )
{
    for( auto it = pathfindData_.begin(); it != pathfindData_.end(); ++it )
        (*it)->AddDynamicDataToUnregister( data );
}
