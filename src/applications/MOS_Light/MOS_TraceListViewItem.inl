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
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_TraceListViewItem.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 3 $
// $Workfile: MOS_TraceListViewItem.inl $
//
//*****************************************************************************

//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem::GetTrace
// Created: NLD 2002-08-08
//-----------------------------------------------------------------------------
inline
MOS_Trace& MOS_TraceListViewItem::GetTrace() const
{
    return Trace_;
}

//-----------------------------------------------------------------------------
// Name: std::MOS_TraceListViewItem::GetName
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
inline
const std::string MOS_TraceListViewItem::GetName() const
{
    return text(0);
}
