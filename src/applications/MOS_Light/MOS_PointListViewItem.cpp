//*****************************************************************************
//
// $Created: NLD 2002-07-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_PointListViewItem.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 9 $
// $Workfile: MOS_PointListViewItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_Pch.h"
#include "MOS_PointListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_PointListViewItem.inl"
#endif

#include "MOS_Lima.h"
#include "MOS_App.h"
#include "MOS_World.h"

#include "MOS_AgentServerMsgMgr.h"
#include "MOS_App.h"

using namespace DIN;


//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_PointListViewItem::MOS_PointListViewItem( E_ItemType nItemType, MT_Vector2D& point, QListView& listView )
    : MOS_ListViewItem_ABC( nItemType, listView )
    , point_          ( point )
{
    SetPos( point );
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_PointListViewItem::MOS_PointListViewItem( E_ItemType nItemType, MT_Vector2D& point, QListView& listView, QListViewItem& itemAfter )
    : MOS_ListViewItem_ABC( nItemType, listView, itemAfter )
    , point_          ( point )
{
    SetPos( point );
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem::MOS_PointListViewItem( E_ItemType nItemType, MT_Vector2D& point, QListViewItem& item )
    : MOS_ListViewItem_ABC( nItemType, item )
    , point_          ( point )
{
    SetPos( point );
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem constructor
// Created: NLD 2002-07-17
//----------------------------------------------------0-------------------------
MOS_PointListViewItem::MOS_PointListViewItem( E_ItemType nItemType, MT_Vector2D& point, QListViewItem& item, QListViewItem& itemAfter )
    : MOS_ListViewItem_ABC( nItemType, item, itemAfter )
    , point_          ( point )
{
    SetPos( point );
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_PointListViewItem::~MOS_PointListViewItem()
{
    
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem::SetPos
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_PointListViewItem::SetPos( const MT_Vector2D& vPos )
{
    T_PointVector collisionPointVector;
    MT_Line line( point_, vPos);
    const MT_Rect& rectWorld = MOS_App::GetApp().GetWorld().GetRect();
    if ( rectWorld.IsInside( vPos ) )
    {
        point_ = vPos;
    }

    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( point_, sMGRS_ );
    setText( 0, sMGRS_.c_str() );
    MT_Float rSizeButton = 150.;

    rect_ = MT_Rect( point_.rX_ - rSizeButton, point_.rY_ - rSizeButton, point_.rX_ + rSizeButton, point_.rY_ + rSizeButton );
}
//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_PointListViewItem::Draw( MOS_PointListViewItem* pSelectedPoint ) const
{
    GFX_Color colorButton( 000. ,000. ,255. , 0.5 );
    if ( pSelectedPoint == this )
    {
        colorButton = GFX_Color( 255. ,000. ,000., 0.5 );
    }
    MT_Float rSizeBorder = 10.0;
    GFX_Tools::CreateGLButton( rect_, colorButton, rSizeBorder );
}


//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem::IsInside
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_PointListViewItem::IsInside( const MT_Vector2D& vPos ) const
{
    return rect_.IsInside( vPos );
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem::WriteOrder
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_PointListViewItem::WriteOrderMsg( DIN_BufferedMessage& msg )
{
    std::string sMGRS("");
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( point_, sMGRS );
    msg << ConvertPositionValue( sMGRS );
    MT_LOG_INFO_MSG( MT_FormatString( "Coord:%s(string) X=%f/Y=%f", sMGRS.c_str(), (float)point_.rX_, (float)point_.rY_  ).c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem::WriteMsg
// Created: NLD 03-03-01
//-----------------------------------------------------------------------------
void MOS_PointListViewItem::WriteMsg( ASN1T_CoordUTM& asnPoint, std::string& sParam )
{
    ConvertPositionValue( sMGRS_ );
    asnPoint = sMGRS_.c_str();
    sParam += sMGRS_;
    sParam += " | ";
}

//-----------------------------------------------------------------------------
// Name: MOS_PointListViewItem::ConvertPositionValue
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
std::string& MOS_PointListViewItem::ConvertPositionValue( std::string& strValue )
{
    strValue.erase( std::find( strValue.begin(), strValue.end(), ' ' ), strValue.end() );
    assert( strValue.size() == 15 );
    return strValue;
}
