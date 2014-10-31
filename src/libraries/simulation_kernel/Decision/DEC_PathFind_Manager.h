// *****************************************************************************
//
// $Created: NLD 2003-08-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/decision/path/DEC_PathFind_Manager.h $
// $Author: Age $
// $Modtime: 10/05/05 16:50 $
// $Revision: 9 $
// $Workfile: DEC_PathFind_Manager.h $
//
// *****************************************************************************

#ifndef __DEC_PathFind_Manager_h_
#define __DEC_PathFind_Manager_h_

#pragma warning( push, 0 )
#include <tools/thread/MessageQueue_ABC.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#pragma warning( pop )
#include <deque>
#include <vector>

class DEC_PathFindRequest;
class TER_PathComputer_ABC;
class TER_PathFinderThread;
class TER_PathFindRequest_ABC;

namespace tools
{
    class Path;
}

// =============================================================================
// Created: NLD 2003-08-14
// =============================================================================
class DEC_PathFind_Manager : private tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >
                           , private boost::noncopyable
{
public:
             DEC_PathFind_Manager( unsigned int threads, double distanceThreshold,
                    double maxAvoidanceDistance, unsigned int maxEndConnections,
                    unsigned int maxComputationDuration,
                    const tools::Path& pathfindDir, const std::string& pathfindFilter );
    virtual ~DEC_PathFind_Manager();

    //! @name Main
    //@{
    // Returns computation time since last update.
    double Update();
    void UpdateInSimulationThread();
    void StartCompute( const boost::shared_ptr< TER_PathComputer_ABC >& pPath, const sword::Pathfind& pathfind );
    void CleanPathAfterComputation( double duration );
    //@}

    //! @name Accessors
    //@{
    int GetCurrentThread() const;
    unsigned int GetNbrShortRequests() const;
    unsigned int GetNbrLongRequests() const;
    unsigned int GetNbrTreatedRequests() const;
    unsigned int GetMaxComputationDuration() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< TER_PathFinderThread* > T_PathFindThreads;
    typedef std::deque< boost::shared_ptr< DEC_PathFindRequest > > T_Requests;
    //@}

private:
    //! @name Tools
    //@{
    virtual boost::shared_ptr< TER_PathFindRequest_ABC > GetMessage();
    boost::shared_ptr< TER_PathFindRequest_ABC > GetMessage( unsigned int nThread );
    T_Requests& GetRequests();
    //@}

private:
    //! @name Member data
    //@{
    mutable boost::mutex mutex_;
    boost::condition condition_;
    T_Requests shortRequests_;
    T_Requests longRequests_;
    double rDistanceThreshold_;
    unsigned int nMaxComputationDuration_;
    unsigned int treatedRequests_;
    T_PathFindThreads pathFindThreads_;
    boost::mutex cleanAndDestroyMutex_;
    std::vector< double > toCleanup_;
    bool bUseInSameThread_;
    double pathfindTime_;
    //@}
};

#endif // __DEC_PathFind_Manager_h_
