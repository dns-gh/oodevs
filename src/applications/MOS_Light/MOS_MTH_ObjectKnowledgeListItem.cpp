//*****************************************************************************
//
// $Created: AGN 03-04-23 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_ObjectKnowledgeListItem.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_MTH_ObjectKnowledgeListItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_MTH_ObjectKnowledgeListItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_MTH_ObjectKnowledgeListItem.inl"
#endif

#include "MOS_MTH_ObjectKnowledgeList.h"

//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeListItem constructor
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_ObjectKnowledgeListItem::MOS_MTH_ObjectKnowledgeListItem( MOS_ObjectKnowledge& represented, QListView* pParent, Type checkType, const std::string& strLabel )
: QCheckListItem( pParent, strLabel.c_str(), checkType )
,represented_( represented ) 
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeListItem constructor
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_ObjectKnowledgeListItem::MOS_MTH_ObjectKnowledgeListItem( MOS_ObjectKnowledge& represented, QCheckListItem* pParent, Type checkType, const std::string& strLabel )
: QCheckListItem( pParent, strLabel.c_str(), checkType )
,represented_( represented ) 
{
}


//-----------------------------------------------------------------------------
// Name: MOS_MTH_ObjectKnowledgeListItem destructor
// Created: AGN 03-04-23
//-----------------------------------------------------------------------------
MOS_MTH_ObjectKnowledgeListItem::~MOS_MTH_ObjectKnowledgeListItem()
{
    
}

