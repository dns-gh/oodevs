//*****************************************************************************
//
// $Created: AGN 03-04-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_AgentKnowledgeListItem.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_MTH_AgentKnowledgeListItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_MTH_AgentKnowledgeListItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_AgentKnowledgeListItem.inl"
#endif

#include "MOS_MTH_AgentKnowledgeList.h"

//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeListItem constructor
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_AgentKnowledgeListItem::MOS_MTH_AgentKnowledgeListItem( MOS_AgentKnowledge& represented, QListView* pParent, Type checkType, const std::string& strLabel )
: QCheckListItem( pParent, strLabel.c_str(), checkType )
,represented_( represented ) 
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeListItem constructor
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_AgentKnowledgeListItem::MOS_MTH_AgentKnowledgeListItem( MOS_AgentKnowledge& represented, QCheckListItem* pParent, Type checkType, const std::string& strLabel )
: QCheckListItem( pParent, strLabel.c_str(), checkType )
,represented_( represented ) 
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_AgentKnowledgeListItem destructor
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_AgentKnowledgeListItem::~MOS_MTH_AgentKnowledgeListItem()
{
    
}

