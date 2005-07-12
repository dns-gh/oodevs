// *****************************************************************************
//
// $Created: NLD 2003-08-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Path/DEC_PathFind_ComputationThread.h $
// $Author: Nld $
// $Modtime: 22/02/05 15:39 $
// $Revision: 7 $
// $Workfile: DEC_PathFind_ComputationThread.h $
//
// *****************************************************************************

#ifndef __DEC_PathFind_ComputationThread_h_
#define __DEC_PathFind_ComputationThread_h_

#include "MIL.h"

#include "MT/MT_Thread/MT_Thread.h"
#include "TER_PathFindReplay.h"

class DEC_Path_ABC;
class DEC_PathFind_Manager;

// =============================================================================
// Created: NLD 2003-08-14
// =============================================================================
class DEC_PathFind_ComputationThread : public MT_Thread
{
    MT_COPYNOTALLOWED( DEC_PathFind_ComputationThread )

public:
    //! @name Types
    //@{
    enum E_State
    {
        eState_Initializing,
        eState_ComputingJob,
        eState_WaitingForJob
    };
    //@}

public:
     DEC_PathFind_ComputationThread( DEC_PathFind_Manager& pathFindManager, uint nThreadID );
    ~DEC_PathFind_ComputationThread();

    //! @name Main
    //@{
    void MainLoop ();
    bool CancelJob( DEC_Path_ABC& path );
    void Signal   ();
    //@}

    //! @name Accessors
    //@{
    E_State GetState() const;
    //@}

private:
    //! @name Main parts
    //@{
    void WaitForJob();
    void ComputeJob();
    bool AssignJob ();
    //@}

    //! @name Tools
    //@{
    void SetState( E_State nNewState );
    //@}

private:
    uint                  nThreadID_;
    TER_PathFindReplay&   pathFindReplay_;
    DEC_PathFind_Manager& pathFindManager_;
    E_State               nState_;     
    DEC_Path_ABC*         pCurrentPathJob_;

    // Threads synchronization
    MT_Condition               waitingForJobCondition_;
    mutable MT_CriticalSection threadCriticalSection_;
};

#include "DEC_PathFind_ComputationThread.inl"

#endif // __DEC_PathFind_ComputationThread_h_
