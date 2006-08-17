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
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_ASN_MsgsMosSimWithContextController.inl $
// $Author: Age $
// $Modtime: 5/02/04 16:04 $
// $Revision: 2 $
// $Workfile: NET_ASN_MsgsMosSimWithContextController.inl $
//
// *****************************************************************************


// -----------------------------------------------------------------------------
// Name: NET_ASN_MsgsMosSimWithContextController::Decode
/** @return 
*/
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
inline
int NET_ASN_MsgsMosSimWithContextController::Decode( MIL_MOSContextID nCtx )
{
    nCtx_ = nCtx;
    return NET_ASN_MessageController< ASN1T_MsgsMosSimWithContext, ASN1C_MsgsMosSimWithContext >::Decode();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_MsgsMosSimWithContextController::GetContext
/** @return 
*/
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
inline
MIL_MOSContextID NET_ASN_MsgsMosSimWithContextController::GetContext() const
{
    return nCtx_;
}
