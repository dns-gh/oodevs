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

#include "DEC_PathFind_Manager_ABC.h"
#pragma warning( push, 0 )
#include <tools/thread/MessageQueue_ABC.h>
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#pragma warning( pop )
#include <deque>
#include <vector>

class DEC_Path_ABC;
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
    virtual void Update();
    virtual void UpdateInSimulationThread();
    virtual void StartCompute( const boost::shared_ptr< DEC_Path_ABC >& pPath );
    virtual void CancelJob( DEC_Path_ABC* pPath );
    virtual void CleanPathAfterComputation( const boost::shared_ptr< TER_PathFindRequest_ABC >& pPath );
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
    typedef std::vector< TER_PathFinderThread* >        T_PathFindThreadPtrVector;
    typedef T_PathFindThreadPtrVector::iterator        IT_PathFindThreadPtrVector;
    typedef T_PathFindThreadPtrVector::const_iterator CIT_PathFindThreadPtrVector;

    typedef std::deque< boost::shared_ptr< TER_PathFindRequest_ABC > > T_Requests;
    //@}

private:
    //! @name Tools
    //@{
    virtual boost::shared_ptr< TER_PathFindRequest_ABC > GetMessage();
    boost::shared_ptr< TER_PathFindRequest_ABC > GetMessage( unsigned int nThread );
    void AddPendingJob( const boost::shared_ptr< DEC_Path_ABC >& pPath );
    T_Requests& GetRequests();
    void ReadPathfind( xml::xistream& xis, const std::vector< unsigned int >& dangerousObjects );
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
    T_PathFindThreadPtrVector pathFindThreads_;
    boost::mutex cleanAndDestroyMutex_;
    T_Requests requestsToCleanAfterComputation_;
    bool bUseInSameThread_;
    //@}
};

#include "DEC_PathFind_Manager.inl"

#endif // __DEC_PathFind_Manager_h_
