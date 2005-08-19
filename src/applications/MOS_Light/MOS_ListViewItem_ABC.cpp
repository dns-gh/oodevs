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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ListViewItem_ABC.cpp $
// $Author: Nld $
// $Modtime: 20/02/03 15:34 $
// $Revision: 3 $
// $Workfile: MOS_ListViewItem_ABC.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_ListViewItem_ABC.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ListViewItem_ABC.inl"
#endif

#include "MOS_Agent.h"
//-----------------------------------------------------------------------------
// Name: MOS_ListViewItem_ABC constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_ListViewItem_ABC::MOS_ListViewItem_ABC( E_ItemType nItemType, QListView& listView, const std::string& strField, const std::string& strValue )
    : QListViewItem( &listView, strField.c_str(), strValue.c_str() )
    , nItemType_( nItemType )
{
//    setOpen( true );
}


//-----------------------------------------------------------------------------
// Name: MOS_ListViewItem_ABC constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ListViewItem_ABC::MOS_ListViewItem_ABC( E_ItemType nItemType, QListViewItem& itemParent, const std::string& strField, const std::string& strValue )
    : QListViewItem( &itemParent, strField.c_str(), strValue.c_str() )
    , nItemType_( nItemType )
{
//    setOpen( true );    
}


//-----------------------------------------------------------------------------
// Name: MOS_ListViewItem_ABC constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ListViewItem_ABC::MOS_ListViewItem_ABC( E_ItemType nItemType, QListViewItem& itemParent, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue )
    : QListViewItem( &itemParent, &itemAfter, strField.c_str(), strValue.c_str() )
    , nItemType_( nItemType )
{
//    setOpen( true );    
}

//-----------------------------------------------------------------------------
// Name: MOS_ListViewItem_ABC constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ListViewItem_ABC::MOS_ListViewItem_ABC( E_ItemType nItemType, QListView& itemParent, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue )
    : QListViewItem( &itemParent, &itemAfter, strField.c_str(), strValue.c_str() )
    , nItemType_( nItemType )
{
//    setOpen( true );    
}

//-----------------------------------------------------------------------------
// Name: MOS_ListViewItem_ABC destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_ListViewItem_ABC::~MOS_ListViewItem_ABC()
{
    
}
