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

#include "DEC_Pathfind_Manager_ABC.h"
#pragma warning( push, 0 )
#include <tools/thread/MessageQueue_ABC.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#pragma warning( pop )
#include <deque>
#include <vector>

class DEC_PathFindRequest;
class TER_PathFinderThread;
class TER_PathFindRequest_ABC;
class MIL_Config;

namespace xml
{
    class xistream;
}

// =============================================================================
// Created: NLD 2003-08-14
// =============================================================================
class DEC_PathFind_Manager : public DEC_PathFind_Manager_ABC
                           , private tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >
{
public:
             DEC_PathFind_Manager( MIL_Config& config, double maxAvoidanceDistance,
                                   const std::vector< unsigned int >& dangerousObjects );
    virtual ~DEC_PathFind_Manager();

    //! @name Main
    //@{
    // Returns computation time since last update.
    virtual double Update();
    virtual void UpdateInSimulationThread();
    virtual void StartCompute( const boost::shared_ptr< DEC_PathComputer_ABC >& pPath, const sword::Pathfind& pathfind );
    virtual void CancelJobForUnit( MIL_Agent_ABC* pion );
    virtual void CleanPathAfterComputation( double duration );
    //@}

    //! @name Accessors
    //@{
    virtual int GetCurrentThread() const;
    virtual unsigned int GetNbrShortRequests() const;
    virtual unsigned int GetNbrLongRequests() const;
    virtual unsigned int GetNbrTreatedRequests() const;
    virtual unsigned int GetMaxComputationDuration() const;
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
    void ReadPathfind( xml::xistream& xis, MIL_Config& config, const std::vector< unsigned int >& dangerousObjects );
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
    unsigned int nMaxEndConnections_;
    unsigned int treatedRequests_;
    T_PathFindThreads pathFindThreads_;
    boost::mutex cleanAndDestroyMutex_;
    std::vector< double > toCleanup_;
    bool bUseInSameThread_;
    double pathfindTime_;
    //@}
};

#endif // __DEC_PathFind_Manager_h_
