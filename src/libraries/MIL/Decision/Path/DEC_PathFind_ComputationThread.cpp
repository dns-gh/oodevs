// *****************************************************************************
//
// $Created: NLD 2003-08-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Path/DEC_PathFind_ComputationThread.cpp $
// $Author: Nld $
// $Modtime: 22/02/05 18:02 $
// $Revision: 5 $
// $Workfile: DEC_PathFind_ComputationThread.cpp $
//
// *****************************************************************************

#include "MIL_pch.h"

#include "DEC_PathFind_ComputationThread.h"
#include "DEC_PathFind_Manager.h"
#include "DEC_Path_ABC.h"

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_ComputationThread constructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
DEC_PathFind_ComputationThread::DEC_PathFind_ComputationThread( DEC_PathFind_Manager& pathFindManager, uint nThreadID )
    : MT_Thread       ()
    , nThreadID_      ( nThreadID )
    , pathFindReplay_ ( TER_World::GetWorld().CreatePathFindReplay() )
    , pathFindManager_( pathFindManager )
    , nState_         ( eState_Initializing )
    , pCurrentPathJob_( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: DEC_PathFind_ComputationThread destructor
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
DEC_PathFind_ComputationThread::~DEC_PathFind_ComputationThread()
{
}

// =============================================================================
// MAIN
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_ComputationThread::AssignJob
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
bool DEC_PathFind_ComputationThread::AssignJob()
{
    MT_CriticalSectionLocker locker( threadCriticalSection_ );

    pCurrentPathJob_ = pathFindManager_.GetPendingJob();
    if( !pCurrentPathJob_ )
        return false;

    nState_ = eState_ComputingJob;
    return true;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_ComputationThread::WaitForJob
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void DEC_PathFind_ComputationThread::WaitForJob()
{
    SetState( eState_WaitingForJob );
    waitingForJobCondition_.Wait();
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_ComputationThread::ComputeJob
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void DEC_PathFind_ComputationThread::ComputeJob()
{
    assert( pCurrentPathJob_ );
    assert( nState_ == eState_ComputingJob );

    pCurrentPathJob_->Compute( pathFindReplay_ );
    pCurrentPathJob_->DecRef();
    pCurrentPathJob_ = 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_ComputationThread::CancelJob
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
bool DEC_PathFind_ComputationThread::CancelJob( DEC_Path_ABC& path )
{
    MT_CriticalSectionLocker locker( threadCriticalSection_ );

    if( pCurrentPathJob_ == &path )
    {
        pCurrentPathJob_->Cancel();
        return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_ComputationThread::MainLoop
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
void DEC_PathFind_ComputationThread::MainLoop()
{
    while( !IsTerminated() )
    {
        while( !AssignJob() )
            WaitForJob();
        ComputeJob();
    }
}

