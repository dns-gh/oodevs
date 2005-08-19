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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RCSubItem.cpp $
// $Author: Nld $
// $Modtime: 20/02/03 15:34 $
// $Revision: 3 $
// $Workfile: MOS_RCSubItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_RCSubItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_RCSubItem.inl"
#endif

#include "MOS_RC.h"
#include "MOS_PointListViewItem.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_RCListView.h"
#include "MOS_Agent.h"



using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_RCSubItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_RCSubItem::MOS_RCSubItem( E_ItemType nItemType, QListView& listView, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, listView, itemAfter, strField, strValue )
    , nValue_   ( 0 )
{
    setRenameEnabled( 0, false );
}

//-----------------------------------------------------------------------------
// Name: MOS_RCSubItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_RCSubItem::MOS_RCSubItem( E_ItemType nItemType, QListViewItem& item, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, item, strField, strValue )
    , nValue_   ( 0 )
{
    setRenameEnabled( 0, false );
}

//-----------------------------------------------------------------------------
// Name: MOS_RCSubItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_RCSubItem::MOS_RCSubItem( E_ItemType nItemType, QListViewItem& item, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, item, itemAfter, strField, strValue )
    , nValue_   ( 0 )
{
    setRenameEnabled( 0, false );
}


//-----------------------------------------------------------------------------
// Name: MOS_RCSubItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_RCSubItem::MOS_RCSubItem( E_ItemType nItemType, QListView& listView, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, listView, strField, strValue )
    , nValue_   ( 0 )
{
    setRenameEnabled( 0, false );
}

//-----------------------------------------------------------------------------
// Name: MOS_RCSubItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_RCSubItem::~MOS_RCSubItem()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_RCSubItem::SetName
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_RCSubItem::SetName( const std::string& sName )
{
    setText( 1, sName.c_str() );
}
 
