//*****************************************************************************
//
// $Created: AGN 03-05-20 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_EnemyInfo_ListItem.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 2 $
// $Workfile: MOS_EnemyInfo_ListItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"
#include "MOS_EnemyInfo_ListItem.h"

#ifndef MOS_USE_INLINE
#	include "MOS_EnemyInfo_ListItem.inl"
#endif

#include "MOS_EnemyInfo.h"

//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_ListItem constructor
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
MOS_EnemyInfo_ListItem::MOS_EnemyInfo_ListItem( QListView* pParent, const std::string& strLabel, MOS_EnemyInfo& info )
: QListViewItem( pParent, strLabel.c_str() )
, info_( info )
{
    setRenameEnabled( 0, true );
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_ListItem destructor
// Created: AGN 03-05-20
//-----------------------------------------------------------------------------
MOS_EnemyInfo_ListItem::~MOS_EnemyInfo_ListItem()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_EnemyInfo_ListItem::okRename
// Created: AGN 03-05-21
//-----------------------------------------------------------------------------
void MOS_EnemyInfo_ListItem::okRename( int col )
{
    QListViewItem::okRename( col );
    info_.SetName( text( col ) );
}

