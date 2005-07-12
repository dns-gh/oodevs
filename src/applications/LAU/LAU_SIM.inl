//*****************************************************************************
//
// Name     : LAU_SIM.inl
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 8 $
// $Workfile: LAU_SIM.inl $
//
//*****************************************************************************

//#include "LAU_SIM.h" // Pour pouvoir le compiler tout seul...


//-----------------------------------------------------------------------------
// Name: LAU_SIM::GetPort
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
INLINE
uint16 LAU_SIM::GetPort() const
{
    return nPort_;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetPort
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetPort( const uint16 nVal )
{
    nPort_ = nVal;
}



//-----------------------------------------------------------------------------
// Name: LAU_SIM::GetMagic
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
INLINE
uint LAU_SIM::GetMagic() const
{
    return nMagic_;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetMagic
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetMagic( const uint nVal )
{
    nMagic_ = nVal;
}



//-----------------------------------------------------------------------------
// Name: LAU_SIM::GetProcessHandle
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
int32 LAU_SIM::GetProcessHandle() const
{
    return nProcessHandle_;
}



//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetProcessHandle
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetProcessHandle( const int32 nProcessHandle )
{
    nProcessHandle_ = nProcessHandle;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::GetProcessID
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
int LAU_SIM::GetProcessID() const
{
    return nProcessID_;
}



//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetProcessID
// Created: AML 03-04-25
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetProcessID( const int nID )
{
    nProcessID_ = nID;
}




//-----------------------------------------------------------------------------
// Name: LAU_SIM::IsPingHasBeenSended
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
bool32 LAU_SIM::IsPingHasBeenSended() const
{
    return bPingHasBeenSended_;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetPingHasBeenSended
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetPingHasBeenSended( const bool32 bNewState )
{
    bPingHasBeenSended_ = bNewState;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetPongHasBeenReceived
// Created: AML 03-04-15
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetPongHasBeenReceived()
{
    bPingHasBeenSended_ = false;
}



//-----------------------------------------------------------------------------
// Name: LAU_SIM::IsConnectedToLauncher
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
bool32 LAU_SIM::IsConnectedToLauncher() const
{
    return pSession_ != NULL;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::IsAskToStop
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
INLINE
bool32 LAU_SIM::IsAskToStop() const
{
    return bAskToStop_;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetAskToStop
// Created: AML 03-04-17
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetAskToStop()
{
    bAskToStop_ = true;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::IsStopSended
// Created: AML 03-04-18
//-----------------------------------------------------------------------------
INLINE
bool32 LAU_SIM::IsStopSended() const
{
    return bStopSended_;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetStopSended
// Created: AML 03-04-18
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetStopSended()
{
    bStopSended_ = true;
}




//-----------------------------------------------------------------------------
// Name: LAU_SIM::GetExerciceID
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
uint32 LAU_SIM::GetExerciceID() const
{
    return nExerciceID_;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetExerciceID
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetExerciceID( const int32 nExerciceID )
{
    nExerciceID_ = nExerciceID;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::HasBeenInitialised
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
bool32 LAU_SIM::HasBeenInitialised() const
{
    return bHasBeenInitialised_;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetHasBeenInitialised
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetHasBeenInitialised( const bool32 bNewState )
{
    bHasBeenInitialised_ = bNewState;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::GetLaunchErrorCode
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
INLINE
int32 LAU_SIM::GetLaunchErrorCode() const
{
    return nLaunchErrorCode_;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetLaunchErrorCode
// Created: AML 03-06-12
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetLaunchErrorCode( const int32 nVal )
{
    nLaunchErrorCode_ = nVal;
}



//-----------------------------------------------------------------------------
// Name: LAU_SIM::SetMosClient
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
void LAU_SIM::SetMosClient( LAU_MOS* pMosClient )
{
    pMosClient_ = pMosClient;
}


//-----------------------------------------------------------------------------
// Name: LAU_SIM::GetMosClient
// Created: AML 03-04-14
//-----------------------------------------------------------------------------
INLINE
LAU_MOS* LAU_SIM::GetMosClient() const
{
    return pMosClient_;
}
