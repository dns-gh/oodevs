//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentListViewItem_ABC.cpp $
// $Author: Nld $
// $Modtime: 29/12/04 15:45 $
// $Revision: 8 $
// $Workfile: MOS_AgentListViewItem_ABC.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_AgentListViewItem_ABC.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentListViewItem_ABC.inl"
#endif

#include "MOS_Agent.h"
#include "MOS_AgentServerMsgMgr.h"

using namespace DIN;


//-----------------------------------------------------------------------------
// Name: MOS_AgentListViewItem_ABC constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_AgentListViewItem_ABC::MOS_AgentListViewItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListView& listView )
    : MOS_ListViewItem_ABC( nItemType, listView, agent.GetName() )
    , agent_              ( agent )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListViewItem_ABC constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_AgentListViewItem_ABC::MOS_AgentListViewItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListView& listView, QListViewItem& itemAfter )
    : MOS_ListViewItem_ABC( nItemType, listView, itemAfter, agent.GetName() )
    , agent_              ( agent )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListViewItem_ABC constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AgentListViewItem_ABC::MOS_AgentListViewItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListViewItem& itemParent )
    : MOS_ListViewItem_ABC( nItemType, itemParent, agent.GetName() )
    , agent_              ( agent )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListViewItem_ABC constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_AgentListViewItem_ABC::MOS_AgentListViewItem_ABC( E_ItemType nItemType, MOS_Agent& agent, QListViewItem& itemParent, QListViewItem& itemAfter )
    : MOS_ListViewItem_ABC( nItemType, itemParent, itemAfter, agent.GetName() )
    , agent_              ( agent )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_AgentListViewItem_ABC destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_AgentListViewItem_ABC::~MOS_AgentListViewItem_ABC()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem::WriteMsg
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_AgentListViewItem_ABC::WriteMsg( ASN1T_OID& asnMsg, std::string& sParam )
{
    asnMsg = agent_.GetAgentID();
    sParam += MT_FormatString( "----->AgentId:%d | ", agent_.GetAgentID() );
}


//-----------------------------------------------------------------------------
// Name: std::MOS_AgentListViewItem_ABC::GetName
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
const std::string MOS_AgentListViewItem_ABC::GetName() const
{
    return text(0);
}
