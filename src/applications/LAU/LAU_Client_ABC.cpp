//*****************************************************************************
//
// $Created : AML 03-04-01 $
//  $Archive: /MVW_v10/Build/SDK/LAU/src/LAU_Client_ABC.cpp $
//   $Author: Nld $
//  $Modtime: 5/05/04 10:43 $
// $Revision: 5 $
// $Workfile: LAU_Client_ABC.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"
#include "LAU_Client_ABC.h"

#ifndef USE_INLINE
#   include "LAU_Client_ABC.inl"
#endif


//-----------------------------------------------------------------------------
// Name   : LAU_Client_ABC constructor
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
LAU_Client_ABC::LAU_Client_ABC()
{
    pSession_ = 0;
}


//-----------------------------------------------------------------------------
// Name   : LAU_Client_ABC destructor
// Created: AML 03-04-01
//-----------------------------------------------------------------------------
LAU_Client_ABC::~LAU_Client_ABC()
{
    Terminate();
}


