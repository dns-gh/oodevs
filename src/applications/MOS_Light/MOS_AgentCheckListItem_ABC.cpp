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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentCheckListItem_ABC.cpp $
// $Author: Nld $
// $Modtime: 29/12/04 15:45 $
// $Revision: 5 $
// $Workfile: MOS_AgentCheckListItem_ABC.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_AgentCheckListItem_ABC.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentCheckListItem_ABC.inl"
#endif

#include "MOS_Agent.h"
//-----------------------------------------------------------------------------
// Name: MOS_AgentCheckListItem_ABC constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_AgentCheckListItem_ABC::MOS_AgentCheckListItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListView& listView, QCheckListItem::Type nCheckable )
    : MOS_CheckListItem_ABC( nItemType, listView, nCheckable, agent.GetName() )
    , agent_              ( agent )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentCheckListItem_ABC constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_AgentCheckListItem_ABC::MOS_AgentCheckListItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListView& listView, QListViewItem& itemAfter, QCheckListItem::Type nCheckable )
    : MOS_CheckListItem_ABC( nItemType, listView, itemAfter, nCheckable, agent.GetName() )
    , agent_              ( agent )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentCheckListItem_ABC constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AgentCheckListItem_ABC::MOS_AgentCheckListItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListViewItem& itemParent, QCheckListItem::Type nCheckable )
    : MOS_CheckListItem_ABC( nItemType, itemParent, nCheckable, agent.GetName() )
    , agent_              ( agent )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentCheckListItem_ABC constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AgentCheckListItem_ABC::MOS_AgentCheckListItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListViewItem& itemParent, QListViewItem& itemAfter, QCheckListItem::Type nCheckable )
    : MOS_CheckListItem_ABC( nItemType, itemParent, itemAfter, nCheckable, agent.GetName() )
    , agent_              ( agent )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentCheckListItem_ABC destructor
// Created: FBD 03-01-07
//-----------------------------------------------------------------------------
MOS_AgentCheckListItem_ABC::~MOS_AgentCheckListItem_ABC()
{
    
}

//-----------------------------------------------------------------------------
// Name: std::MOS_AgentCheckListItem_ABC::GetName
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
const std::string MOS_AgentCheckListItem_ABC::GetName() const
{
    return text(0);
}