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
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_TraceSubItem.cpp $
// $Author: Nld $
// $Modtime: 20/02/03 15:34 $
// $Revision: 3 $
// $Workfile: MOS_TraceSubItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_Pch.h"
#include "MOS_TraceSubItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_TraceSubItem.inl"
#endif

#include "MOS_Trace.h"
#include "MOS_PointListViewItem.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_TraceListView.h"
#include "MOS_Agent.h"



using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_TraceSubItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_TraceSubItem::MOS_TraceSubItem( E_ItemType nItemType, QListView& listView, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, listView, itemAfter, strField, strValue )
    , nValue_   ( 0 )
{
    setRenameEnabled( 0, false );
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceSubItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_TraceSubItem::MOS_TraceSubItem( E_ItemType nItemType, QListViewItem& item, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, item, strField, strValue )
    , nValue_   ( 0 )
{
    setRenameEnabled( 0, false );
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceSubItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_TraceSubItem::MOS_TraceSubItem( E_ItemType nItemType, QListViewItem& item, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, item, itemAfter, strField, strValue )
    , nValue_   ( 0 )
{
    setRenameEnabled( 0, false );
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceSubItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_TraceSubItem::MOS_TraceSubItem( E_ItemType nItemType, QListView& listView, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, listView, strField, strValue )
    , nValue_   ( 0 )
{
    setRenameEnabled( 0, false );
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceSubItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_TraceSubItem::~MOS_TraceSubItem()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceSubItem::SetName
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_TraceSubItem::SetName( const std::string& sName )
{
    setText( 1, sName.c_str() );
}
 
