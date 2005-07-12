//*****************************************************************************
//
// Name     : LAU_NetMgr.inl
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 5 $
// $Workfile: LAU_NetMgr.inl $
//
//*****************************************************************************

//#include "LAU_NetMgr.h" // Pour pouvoir le compiler tout seul...

//-----------------------------------------------------------------------------
/** @name Accessors */
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Name   : DIN::DIN_Link
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
const DIN::DIN_Engine& LAU_NetMgr::GetDIN_Engine() const
{
    return dinEngine_;
}


//-----------------------------------------------------------------------------
// Name   : DIN::DIN_Link
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
DIN::DIN_Engine& LAU_NetMgr::GetDIN_Engine()
{
    return dinEngine_;
}



//-----------------------------------------------------------------------------
// Name   : LAU_NetMgr::GetPortMos
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
uint16 LAU_NetMgr::GetPortMos() const
{
    return nPortMos_;
}

//-----------------------------------------------------------------------------
// Name   : LAU_NetMgr::GetPortSim
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
uint16 LAU_NetMgr::GetPortSim() const
{
    return nPortSim_;
}

//-----------------------------------------------------------------------------
// Name   : LAU_NetMgr::GetMagic
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
uint LAU_NetMgr::GetMagic() const
{
    return nMagic_;
}


//-----------------------------------------------------------------------------
// Name   : DIN::DIN_ConnectionProtocols
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
DIN::DIN_ConnectionProtocols& LAU_NetMgr::GetConnectionProtocols()
{
    return connectionProtocols_;
}



//-----------------------------------------------------------------------------
// Name   : LAU_NetMgr::GetConMgr
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
LAU_ConMgr& LAU_NetMgr::GetConMgr() const
{
    return *pConMgr_;
}


//-----------------------------------------------------------------------------
// Name   : LAU_NetMgr::GetMsgMgr
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
INLINE
LAU_MsgMgr& LAU_NetMgr::GetMsgMgr() const
{
    return *pMsgMgr_;
}


//-----------------------------------------------------------------------------
// Name   : LAU_NetMgr::GetWorkspace
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-03
//-----------------------------------------------------------------------------
INLINE
const LAU_Workspace& LAU_NetMgr::GetWorkspace() const
{
    return workspace_;
}



//-----------------------------------------------------------------------------
// Name   : LAU_NetMgr::GetWorkspace
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-03
//-----------------------------------------------------------------------------
INLINE
LAU_Workspace& LAU_NetMgr::GetWorkspace()
{
    return workspace_;
}

