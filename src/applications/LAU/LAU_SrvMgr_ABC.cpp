//*****************************************************************************
//
// $Created : AML 03-03-31 $
//  $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_SrvMgr_ABC.cpp $
//   $Author: Aml $
//  $Modtime: 17/04/03 16:49 $
// $Revision: 4 $
// $Workfile: LAU_SrvMgr_ABC.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"
#include "LAU_SrvMgr_ABC.h"


#ifndef USE_INLINE
#   include "LAU_SrvMgr_ABC.inl"
#endif


//-----------------------------------------------------------------------------
// Name   : LAU_SrvMgr_ABC constructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
LAU_SrvMgr_ABC::LAU_SrvMgr_ABC( LAU_NetMgr& netMgr  )
    : netMgr_               (       netMgr          )
    , pServer_              (       NULL            )
{

}



//-----------------------------------------------------------------------------
// Name   : LAU_SrvMgr_ABC destructor
// Object : 
// Return : 
// Warning: 
// Created: AML 03-03-31
//-----------------------------------------------------------------------------
LAU_SrvMgr_ABC::~LAU_SrvMgr_ABC()
{

}

