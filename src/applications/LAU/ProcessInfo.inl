//*****************************************************************************
//
// $Created: AML 03-04-25 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/ProcessInfo.inl $
// $Author: Aml $
// $Modtime: 25/04/03 16:30 $
// $Revision: 2 $
// $Workfile: ProcessInfo.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Accessors
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name: ProcessInfo::GetID
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
int ProcessInfo::GetID() const
{
    return nID_;
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::GetState
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
ProcessInfo::E_State ProcessInfo::GetState() const
{
    return nState_;
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::GetExitCode
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
int ProcessInfo::GetExitCode()
{
    nState_ = eStateDestroyable;
    return nExitCode_;
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::GetHandleProcess
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
HANDLE ProcessInfo::GetHandleProcess() const
{
    return hProcess_;
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::GetHandlePrimaryThread
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
HANDLE ProcessInfo::GetHandlePrimaryThread() const
{
    return hPrimaryThread_;
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::GetProcessID
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
uint32 ProcessInfo::GetProcessID() const
{
    return nProcessID_;
}



//-----------------------------------------------------------------------------
// Name: ProcessInfo::GetPrimaryThreadID
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
uint32 ProcessInfo::GetPrimaryThreadID() const
{
    return nPrimaryThreadID_;
}




//-----------------------------------------------------------------------------
// Name: ProcessInfo::SetHandleProcess
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
void ProcessInfo::SetHandleProcess( HANDLE h )
{
    assert( h != NULL );
    nState_ = eStateLaunched;
    hProcess_ = h;
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::SetHandlePrimaryThread
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
void ProcessInfo::SetHandlePrimaryThread( HANDLE h )
{
    hPrimaryThread_ = h;
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::SetProcessID
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
void ProcessInfo::SetProcessID( uint32 nID )
{
    nProcessID_ = nID;
}


//-----------------------------------------------------------------------------
// Name: ProcessInfo::SetPrimaryThreadID
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
void ProcessInfo::SetPrimaryThreadID( uint32 nID )
{
    nPrimaryThreadID_ = nID;
}


