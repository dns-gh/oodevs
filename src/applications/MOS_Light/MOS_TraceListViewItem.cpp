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
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_TraceListViewItem.cpp $
// $Author: Agn $
// $Modtime: 3/06/03 14:09 $
// $Revision: 6 $
// $Workfile: MOS_TraceListViewItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_TraceListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_TraceListViewItem.inl"
#endif

#include "MOS_Trace.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_TraceListView.h"
#include "MOS_Agent.h"
#include "MOS_TraceSubItem.h"



using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_TraceListViewItem::MOS_TraceListViewItem( E_ItemType nItemType, MOS_Trace& Trace, QListView& listView, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, listView, itemAfter, strField, strValue )
    , Trace_          ( Trace )
{
    Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_TraceListViewItem::MOS_TraceListViewItem( E_ItemType nItemType, MOS_Trace& Trace, QListViewItem& item, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, item, strField, strValue )
    , Trace_          ( Trace )
{
    Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_TraceListViewItem::MOS_TraceListViewItem( E_ItemType nItemType,  MOS_Trace& Trace, QListView& listView, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, listView, strField, strValue )
    , Trace_          ( Trace )
{
    Initialize();
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem::Initialize
// Created: AGN 03-06-03
//-----------------------------------------------------------------------------
void MOS_TraceListViewItem::Initialize()
{
    CreateRect();
    setRenameEnabled( 0, false );
    setText( 0, MT_FormatString( "%02d:%02d:%02d", ( Trace_.GetTime() / 3600 ) % 24, ( Trace_.GetTime() / 60 ) % 60 , Trace_.GetTime() % 60  ).c_str() );
    setText( 1, Trace_.GetName().c_str() );
    
    Trace_.SetTraceItem( this );
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem::CreateRect
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
void MOS_TraceListViewItem::CreateRect()
{
    MT_Vector2D point = Trace_.GetPos();
    MT_Float rSizeButton = 10.0 / 2.0;
    vPos1_ = MT_Vector2D( point.rX_ - rSizeButton, point.rY_ - rSizeButton);
    vPos2_ = MT_Vector2D( point.rX_ + rSizeButton, point.rY_ - rSizeButton);
    vPos3_ = MT_Vector2D( point.rX_, point.rY_ + rSizeButton);
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_TraceListViewItem::~MOS_TraceListViewItem()
{
    Trace_.SetTraceItem( 0 );
}

 
//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_TraceListViewItem::Draw( MOS_TraceListViewItem* pSelectedTrace ) const
{
    if ( pSelectedTrace == this )
    {
        GFX_Color colorTriangle = GFX_Color( 255. ,000. ,0., 0.4 );
        GFX_Tools::CreateGLTriangleShadow( vPos1_, vPos2_, vPos3_, 1., 6., colorTriangle  );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem::IsInside
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_TraceListViewItem::IsInside( const MT_Vector2D& vPos ) const
{
    MT_Triangle triangle( vPos1_, vPos2_, vPos3_ );
    
    if ( triangle.IsInside( vPos ) )
        return true;
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem::AddOrdeTraceonduite
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
MOS_TraceSubItem& MOS_TraceListViewItem::CreateItem( E_ItemType nType, int nValue )
{
    if ( itemVector_.size() )
    {
        MOS_TraceSubItem& item = *new MOS_TraceSubItem( nType, *this, **itemVector_.rbegin(), "" );
        item.SetValue( nValue );
        RegisterItem( item );
        return item;
    }
    else
    {
        MOS_TraceSubItem& item = *new MOS_TraceSubItem( nType, *this, "" );
        item.SetValue( nValue );
        RegisterItem( item );
        return item;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem::AddOrdeTraceonduite
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
void MOS_TraceListViewItem::DeleteItem( MOS_TraceSubItem& item )
{
    UnregisterItem( item );
    delete &item;
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem::RegisterItem
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
void MOS_TraceListViewItem::RegisterItem( MOS_TraceSubItem& item )
{
    IT_TraceItemPtrVector itItem = std::find( itemVector_.begin(), itemVector_.end(), &item );
    assert ( itItem == itemVector_.end() );
    itemVector_.push_back( &item );
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListViewItem::UnregisterItem
// Created: FBD 03-01-28
//-----------------------------------------------------------------------------
void MOS_TraceListViewItem::UnregisterItem( MOS_TraceSubItem& item )
{
    IT_TraceItemPtrVector itItem = std::find( itemVector_.begin(), itemVector_.end(), &item );
    assert ( itItem != itemVector_.end() );
    itemVector_.erase( itItem );
}


