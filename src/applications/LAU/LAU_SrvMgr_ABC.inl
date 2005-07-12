//*****************************************************************************
//
// Name     : LAU_SrvMgr_ABC.inl
//
// $Created : AML 03-03-31 $
//  $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_SrvMgr_ABC.inl $
//   $Author: Aml $
//  $Modtime: 3/04/03 17:15 $
// $Revision: 3 $
// $Workfile: LAU_SrvMgr_ABC.inl $
//
//*****************************************************************************

//#include "LAU_SrvMgr_ABC.h" // Pour pouvoir le compiler tout seul...


//-----------------------------------------------------------------------------
// Name   : LAU_SrvMgr_ABC::GetNetMgr
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-03
//-----------------------------------------------------------------------------
INLINE
const LAU_NetMgr& LAU_SrvMgr_ABC::GetNetMgr() const
{
    return netMgr_;
}


//-----------------------------------------------------------------------------
// Name   : LAU_SrvMgr_ABC::GetNetMgr
// Object : 
// Return : 
// Warning: 
// Created: AML 03-04-03
//-----------------------------------------------------------------------------
INLINE
LAU_NetMgr& LAU_SrvMgr_ABC::GetNetMgr()
{
    return netMgr_;
}

