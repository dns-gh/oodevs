// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-02-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_ASN_MsgsMosSimWithContextController.cpp $
// $Author: Age $
// $Modtime: 5/02/04 12:56 $
// $Revision: 2 $
// $Workfile: NET_ASN_MsgsMosSimWithContextController.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "simulation_kernel_pch.h"
#include "NET_ASN_MsgsMosSimWithContextController.h"

// -----------------------------------------------------------------------------
// Name: NET_ASN_MsgsMosSimWithContextController constructor
/** @param  decodeBuffer 
*/
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
NET_ASN_MsgsMosSimWithContextController::NET_ASN_MsgsMosSimWithContextController( uint8* pBuffer, unsigned long nBufferSize )
    : NET_ASN_MessageController< ASN1T_MsgsMosSimWithContext, ASN1C_MsgsMosSimWithContext >( pBuffer, nBufferSize )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_MsgsMosSimWithContextController destructor
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
NET_ASN_MsgsMosSimWithContextController::~NET_ASN_MsgsMosSimWithContextController()
{
    //NOTHING
}


MT_POOLING_FUNCTIONS( NET_ASN_MsgsMosSimWithContextController, 20 );