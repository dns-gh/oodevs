//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RCSubItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_RCSubItem.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_RCSubItem::SetValue
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
inline
void MOS_RCSubItem::SetValue( int nValue )
{
    nValue_ = nValue;
}


//-----------------------------------------------------------------------------
// Name: MOS_RCSubItem::GetValue
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
inline
int MOS_RCSubItem::GetValue() const
{
    return nValue_;
}
