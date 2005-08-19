//*****************************************************************************
//
// $Created: AGN 03-05-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_KnowledgeList_ViewItem.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 3 $
// $Workfile: MOS_KnowledgeList_ViewItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_KnowledgeList_ViewItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_KnowledgeList_ViewItem.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_ViewItem constructor
// Created: AGN 03-05-22
//-----------------------------------------------------------------------------
MOS_KnowledgeList_ViewItem::MOS_KnowledgeList_ViewItem( MOS_AgentKnowledge& represented, QListViewItem* pParent, const std::string& strLabel )
: QListViewItem( pParent, strLabel.c_str() )
,represented_( represented ) 
{
    setRenameEnabled( 0, true );
}


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_ViewItem constructor
// Created: AGN 03-05-22
//-----------------------------------------------------------------------------
MOS_KnowledgeList_ViewItem::MOS_KnowledgeList_ViewItem( MOS_AgentKnowledge& represented, QListView* pParent, const std::string& strLabel )
: QListViewItem( pParent, strLabel.c_str() )
,represented_( represented ) 
{
    setRenameEnabled( 0, true );
}


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_ViewItem destructor
// Created: AGN 03-05-22
//-----------------------------------------------------------------------------
MOS_KnowledgeList_ViewItem::~MOS_KnowledgeList_ViewItem()
{
    
}

