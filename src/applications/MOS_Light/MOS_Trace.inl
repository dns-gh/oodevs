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
// $Created: NLD 2002-07-15 $
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_Trace.inl $
// $Author: Nld $
// $Modtime: 10/09/04 11:45 $
// $Revision: 5 $
// $Workfile: MOS_Trace.inl $
//
//*****************************************************************************
//-----------------------------------------------------------------------------
// Name: MOS_Trace::GetPosition
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
inline
const MT_Vector2D& MOS_Trace::GetPos() const
{
    return vPos_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Trace::GetTime
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
inline
int MOS_Trace::GetTime() const
{
    return nTime_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Trace::GetAgent
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
inline
MOS_Agent& MOS_Trace::GetAgent()
{
    return agent_;
}


//-----------------------------------------------------------------------------
// Name: MOS_Trace::SetTraceItem
// Created: AGN 03-06-03
//-----------------------------------------------------------------------------
inline
void MOS_Trace::SetTraceItem( QListViewItem* pItem )
{
    pItem_ = pItem;
}

