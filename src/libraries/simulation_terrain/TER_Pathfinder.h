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

#pragma warning( push, 0 )
#include <tools/thread/MessageQueue_ABC.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#pragma warning( pop )
#include <boost/shared_ptr.hpp>
#include <deque>
#include <vector>

class TER_DynamicData;
class TER_PathfindRequest;
class TER_PathComputer_ABC;
class TER_PathFinderThread;
class TER_PathfindRequest;
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

// =============================================================================
// Created: NLD 2003-08-14
// =============================================================================
class TER_Pathfinder : private tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathfindRequest > >
                           , private boost::noncopyable
{
public:
             TER_Pathfinder( const boost::shared_ptr< TER_StaticData >& staticData,
                    unsigned int threads, double distanceThreshold,
                    double maxAvoidanceDistance, unsigned int maxEndConnections,
                    unsigned int maxComputationDuration,
                    const tools::Path& pathfindDir, const std::string& pathfindFilter );
    virtual ~TER_Pathfinder();

    //! @name Main
    //@{
    // Returns computation time since last update.
    double Update();
    void UpdateInSimulationThread();
    void StartCompute( const boost::shared_ptr< TER_PathComputer_ABC >& pPath, const sword::Pathfind& pathfind );
    void CleanPathAfterComputation( double duration );

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
    //! @name Tools
    //@{
    virtual boost::shared_ptr< TER_PathfindRequest > GetMessage();
    boost::shared_ptr< TER_PathfindRequest > GetMessage( unsigned int nThread );
    T_Requests& GetRequests();
    //@}

private:
    const boost::shared_ptr< TER_StaticData > staticData_;

    mutable boost::mutex mutex_;
    boost::condition condition_;
    T_Requests shortRequests_;
    T_Requests longRequests_;
    double rDistanceThreshold_;
    unsigned int nMaxComputationDuration_;
    unsigned int treatedRequests_;
    std::vector< TER_PathFinderThread* > threads_;
    boost::mutex cleanAndDestroyMutex_;
    std::vector< double > toCleanup_;
    bool bUseInSameThread_;
    double pathfindTime_;
};

#endif // SIMULATION_TERRAIN_PATHFINDER
