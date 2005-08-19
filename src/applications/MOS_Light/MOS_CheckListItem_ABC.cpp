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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_CheckListItem_ABC.cpp $
// $Author: Nld $
// $Modtime: 21/02/03 9:55 $
// $Revision: 4 $
// $Workfile: MOS_CheckListItem_ABC.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_CheckListItem_ABC.h"

#ifndef MOS_USE_INLINE
#   include "MOS_CheckListItem_ABC.inl"
#endif

#include "MOS_Agent.h"
//-----------------------------------------------------------------------------
// Name: MOS_CheckListItem_ABC constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_CheckListItem_ABC::MOS_CheckListItem_ABC( E_ItemType nItemType, QListView& listView, QCheckListItem::Type nCheckable, const std::string& strField, const std::string& /*strValue*/ )
    : QCheckListItem( &listView, strField.c_str(), nCheckable  )
    , nItemType_( nItemType )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_CheckListItem_ABC constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_CheckListItem_ABC::MOS_CheckListItem_ABC( E_ItemType nItemType, QListViewItem& itemParent, QCheckListItem::Type  nCheckable, const std::string& strField, const std::string& /*strValue*/ )
    : QCheckListItem( &itemParent, strField.c_str(), nCheckable )
    , nItemType_( nItemType )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_CheckListItem_ABC constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_CheckListItem_ABC::MOS_CheckListItem_ABC( E_ItemType nItemType, QListViewItem& itemParent, QListViewItem& /*itemAfter*/, QCheckListItem::Type nCheckable, const std::string& strField, const std::string& /*strValue*/ )
    : QCheckListItem( &itemParent, strField.c_str(), nCheckable )
    , nItemType_( nItemType )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_CheckListItem_ABC constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_CheckListItem_ABC::MOS_CheckListItem_ABC( E_ItemType nItemType, QListView& itemParent, QListViewItem& /*itemAfter*/, QCheckListItem::Type  nCheckable, const std::string& strField, const std::string& /*strValue*/ )
    : QCheckListItem( &itemParent, strField.c_str(), nCheckable )
    , nItemType_( nItemType )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_CheckListItem_ABC destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_CheckListItem_ABC::~MOS_CheckListItem_ABC()
{
    
}
