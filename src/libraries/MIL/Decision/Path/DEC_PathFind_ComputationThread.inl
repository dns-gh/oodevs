// *****************************************************************************
//
// $Created: NLD 2003-08-14 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Decision/Path/DEC_PathFind_ComputationThread.inl $
// $Author: Nld $
// $Modtime: 22/02/05 17:58 $
// $Revision: 2 $
// $Workfile: DEC_PathFind_ComputationThread.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_ComputationThread::GetState
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
inline
DEC_PathFind_ComputationThread::E_State DEC_PathFind_ComputationThread::GetState() const
{
    MT_CriticalSectionLocker locker( threadCriticalSection_ );
    return nState_;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_ComputationThread::SetState
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
inline
void DEC_PathFind_ComputationThread::SetState( E_State nNewState )
{
    MT_CriticalSectionLocker locker( threadCriticalSection_ );
    nState_ = nNewState;
}

// -----------------------------------------------------------------------------
// Name: DEC_PathFind_ComputationThread::Signal
// Created: NLD 2003-08-14
// -----------------------------------------------------------------------------
inline
void DEC_PathFind_ComputationThread::Signal()
{
    waitingForJobCondition_.Signal();
}
