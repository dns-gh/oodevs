//*****************************************************************************
//
// Name     : LAU_MsgMgr.inl
//
// $Created : AML 03-03-26 $
// $Archive : $
// $Author  : $
// $Modtime : $
// $Revision: 4 $
// $Workfile: LAU_MsgMgr.inl $
//
//*****************************************************************************

//#include "LAU_MsgMgr.h" // Pour pouvoir le compiler tout seul...


//-------------------------------------------------------------------------
// Messages tools
//-------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// Name   : DIN::DIN_BufferedMessage 
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-03
//-----------------------------------------------------------------------------
INLINE
DIN::DIN_BufferedMessage LAU_MsgMgr::BuildMessageMos()
{
    assert( pMessageServiceMos_ );
    return DIN::DIN_BufferedMessage( *pMessageServiceMos_ );
}


//-----------------------------------------------------------------------------
// Name: DIN::DIN_BufferedMessage 
// Created: AML 03-04-16
//-----------------------------------------------------------------------------
INLINE
DIN::DIN_BufferedMessage LAU_MsgMgr::BuildMessageSim()
{
    assert( pMessageServiceSim_ );
    return DIN::DIN_BufferedMessage( *pMessageServiceSim_ );
}
