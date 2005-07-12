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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentListViewItem_ABC.inl $
// $Author: Nld $
// $Modtime: 29/12/04 15:45 $
// $Revision: 5 $
// $Workfile: MOS_AgentListViewItem_ABC.inl $
//
//*****************************************************************************


//-----------------------------------------------------------------------------
// Name: MOS_AgentListViewItem_ABC::GetAgent
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
inline
MOS_Agent& MOS_AgentListViewItem_ABC::GetAgent() const
{
    return agent_;
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListViewItem_ABC::SetName
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
inline
void MOS_AgentListViewItem_ABC::SetName( const std::string& sName )
{
    setText(0, sName.c_str() );
}

