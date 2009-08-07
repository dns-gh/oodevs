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

#include "tools/thread/MessageQueue_ABC.h"
#pragma warning( disable : 4275 )
#include <boost/thread/mutex.hpp>
#include <boost/thread/condition.hpp>
#include <boost/shared_ptr.hpp>

namespace xml
{
    class xistream;
}

class DEC_Path_ABC;
class TER_PathFinderThread;
class MIL_Config;

// =============================================================================
// Created: NLD 2003-08-14
// =============================================================================
class DEC_PathFind_Manager : private tools::thread::MessageQueue_ABC< boost::shared_ptr< TER_PathFindRequest_ABC > >
                           , private boost::noncopyable
{

public:
    explicit DEC_PathFind_Manager( MIL_Config& config );
    virtual ~DEC_PathFind_Manager();

    //! @name Main
    //@{
    void Update                   ();
    void StartCompute             ( boost::shared_ptr< DEC_Path_ABC > pPath );
    void CleanPathAfterComputation( const boost::shared_ptr< TER_PathFindRequest_ABC >& pPath );
    //@}

    //! @name Accessors
    //@{
    int  GetCurrentThread         () const;
    uint GetNbrShortRequests      () const;
    uint GetNbrLongRequests       () const;
    uint GetNbrRequests           () const;
    uint GetNbrTreatedRequests    () const;
    uint GetMaxComputationDuration() const;
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< TER_PathFinderThread* >        T_PathFindThreadPtrVector;
    typedef T_PathFindThreadPtrVector::iterator        IT_PathFindThreadPtrVector;
    typedef T_PathFindThreadPtrVector::const_iterator CIT_PathFindThreadPtrVector;

    typedef std::deque< boost::shared_ptr< TER_PathFindRequest_ABC > >      T_Requests;
    //@}

private:
    //! @name Tools
    //@{
    virtual boost::shared_ptr< TER_PathFindRequest_ABC > GetMessage   ();
            boost::shared_ptr< TER_PathFindRequest_ABC > GetMessage   ( unsigned int nThread );
    void        AddPendingJob( boost::shared_ptr< DEC_Path_ABC > pPath );
    T_Requests& GetRequests();
    //@}

private:
    mutable boost::mutex mutex_;
    boost::condition     condition_;

    T_Requests shortRequests_;
    T_Requests longRequests_;
    MT_Float   rDistanceThreshold_;
    uint       nMaxComputationDuration_;
    uint       treatedRequests_;

    T_PathFindThreadPtrVector  pathFindThreads_;

    boost::mutex cleanAndDestroyMutex_;
    T_Requests   requestsToCleanAfterComputation_;
};

#include "DEC_PathFind_Manager.inl"

#endif // __DEC_PathFind_Manager_h_
