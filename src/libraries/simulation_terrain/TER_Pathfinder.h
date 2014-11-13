// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef SIMULATION_TERRAIN_PATHFINDER
#define SIMULATION_TERRAIN_PATHFINDER

#include <tools/Path.h>
#pragma warning( push, 0 )
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#pragma warning( pop )
#include <boost/thread.hpp>
#include <boost/shared_ptr.hpp>
#include <deque>
#include <functional>
#include <vector>

class TER_DynamicData;
class TER_PathfindRequest;
class TER_PathFinderThread;
class TER_PathfindRequest;
class TER_PathSection;
struct TER_PathResult;
class TER_StaticData;

namespace sword
{
    class Pathfind;
}

namespace tools
{
    class Path;
}

typedef boost::shared_ptr< TER_DynamicData > DynamicDataPtr;

// Simple "future" for pathfind result, supporting only polling.
class TER_PathFuture : private boost::noncopyable
{
public:
     TER_PathFuture();
    ~TER_PathFuture();

    void Set( const boost::shared_ptr< TER_PathResult >& path );
    boost::shared_ptr< TER_PathResult > Get() const;
    void Cancel();
    bool IsCanceled() const;

private:
    bool canceled_;
    mutable boost::mutex mutex_;
    boost::shared_ptr< TER_PathResult > path_;
};

// =============================================================================
// Created: NLD 2003-08-14
// =============================================================================
class TER_Pathfinder : private boost::noncopyable
{
public:
             TER_Pathfinder( const boost::shared_ptr< TER_StaticData >& staticData,
                    unsigned int threads, double distanceThreshold,
                    double maxAvoidanceDistance, unsigned int maxEndConnections,
                    unsigned int maxComputationDuration,
                    const tools::Path& pathfindDir, const std::string& pathfindFilter,
                    bool debugPath );
    virtual ~TER_Pathfinder();

    //! @name Main
    //@{
    // Returns computation time since last update.
    double Update();
    void UpdateInSimulationThread();

    // Starts computing a path, returned future will contain a valid result
    // when the computation terminates, successfully and on error. Note the
    // result never contains TER_Path_ABC::eComputing.
    boost::shared_ptr< TER_PathFuture > StartCompute(
            std::size_t callerId,
            const std::vector< boost::shared_ptr< TER_PathSection > > sections,
            const sword::Pathfind& pathfind );

    void AddDynamicData   ( const DynamicDataPtr& data );
    void RemoveDynamicData( const DynamicDataPtr& data );
    //@}

    //! @name Accessors
    //@{
    unsigned int GetNbrShortRequests() const;
    unsigned int GetNbrLongRequests() const;
    unsigned int GetNbrTreatedRequests() const;
    unsigned int GetMaxComputationDuration() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::deque< boost::shared_ptr< TER_PathfindRequest > > T_Requests;
    //@}

private:
    boost::shared_ptr< TER_PathfindRequest > GetMessage( unsigned int nThread );
    T_Requests& GetRequests();
    void ProcessRequest( TER_PathFinderThread& data, TER_PathfindRequest& rq );

private:
    const boost::shared_ptr< TER_StaticData > staticData_;
    std::vector< std::unique_ptr< TER_PathFinderThread > > pathfindData_;
    std::vector< std::unique_ptr< boost::thread > > threads_;
    const tools::Path dumpDir_; // empty if dump is disabled
    const std::set< size_t > dumpFilter_; // empty if no id filters
    const bool debugPath_;
    std::size_t queryId_;

    mutable boost::mutex mutex_;
    boost::condition condition_;
    T_Requests shortRequests_;
    T_Requests longRequests_;
    double rDistanceThreshold_;
    unsigned int nMaxComputationDuration_;
    unsigned int treatedRequests_;
    bool bUseInSameThread_;
    bool stopped_;

    boost::mutex pathfindTimeMutex_;
    // Total computation time of terminated pathfinds since last Update() call
    double pathfindTime_;
};

#endif // SIMULATION_TERRAIN_PATHFINDER
