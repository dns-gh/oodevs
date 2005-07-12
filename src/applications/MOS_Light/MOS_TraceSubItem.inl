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
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_TraceSubItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_TraceSubItem.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_TraceSubItem::SetValue
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
inline
void MOS_TraceSubItem::SetValue( int nValue )
{
    nValue_ = nValue;
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceSubItem::GetValue
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
inline
int MOS_TraceSubItem::GetValue() const
{
    return nValue_;
}
