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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimitCheckListItem.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 8 $
// $Workfile: MOS_LimitCheckListItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_Pch.h"
#include "MOS_LimitCheckListItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_LimitCheckListItem.inl"
#endif

#include "MOS_Limit.h"
#include "MOS_PointListViewItem.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"


//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_LimitCheckListItem::MOS_LimitCheckListItem( E_ItemType nItemType, MOS_Limit& Limit, QListView& listView, QListViewItem& itemAfter, QCheckListItem::Type nCheckable, const std::string& strField, const std::string& strValue )
    : MOS_CheckListItem_ABC( nItemType, listView, itemAfter, nCheckable, strField, strValue )
    , Limit_          ( Limit )
{
    setRenameEnabled( 0, true );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_LimitCheckListItem::MOS_LimitCheckListItem( E_ItemType nItemType, MOS_Limit& Limit, QListViewItem& item, QCheckListItem::Type nCheckable, const std::string& strField, const std::string& strValue )
    : MOS_CheckListItem_ABC( nItemType, item, nCheckable, strField, strValue )
    , Limit_          ( Limit )
{
    setRenameEnabled( 0, true );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_LimitCheckListItem::MOS_LimitCheckListItem( E_ItemType nItemType,  MOS_Limit& Limit, QListView& listView, QCheckListItem::Type nCheckable, const std::string& strField, const std::string& strValue )
    : MOS_CheckListItem_ABC( nItemType, listView, nCheckable, strField, strValue )
    , Limit_          ( Limit )
{
    setRenameEnabled( 0, true );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_LimitCheckListItem::~MOS_LimitCheckListItem()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::AddPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem& MOS_LimitCheckListItem::CreatePoint( MT_Vector2D& vPoint, MOS_PointListViewItem* pPointAfter )
{
    MOS_PointListViewItem* pPointLimit = 0;
    if ( pointVector_.size() )
    {
        if ( pPointAfter )
            pPointLimit = new MOS_PointListViewItem( eItemTypeEltPoint, vPoint, *this, *pPointAfter );
        else
            pPointLimit = new MOS_PointListViewItem( eItemTypeEltPoint, vPoint, *this, **pointVector_.rbegin() );
    }
    else
        pPointLimit = new MOS_PointListViewItem( eItemTypeEltPoint, vPoint, *this );

    RegisterPoint( *pPointLimit, pPointAfter );
    return ( *pPointLimit );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::DeletePoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimitCheckListItem::DeletePoint( MT_Vector2D& vPoint )
{
    MOS_PointListViewItem* pPoint = GetPoint( vPoint );
    assert( pPoint );
    UnregisterPoint( *pPoint );
    delete pPoint;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimitCheckListItem::Draw( MOS_LimitCheckListItem* /*pSelectedLimit*/, MOS_PointListViewItem* pSelectedPoint ) const
{
    GFX_Color color( 255,255,255 );

    MOS_PointListViewItem* pLastPoint = 0;
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        if ( pLastPoint )
        {
            GFX_Tools::CreateGLLine( pLastPoint->GetPoint(), pCurPoint->GetPoint(), 2.0, color );
        }
        pLastPoint = pCurPoint;
    }

    for ( itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        (*itPoint)->Draw( pSelectedPoint );
        
    }

    MT_Float rSizeName = 500. ;
    
    GFX_Color colorName( 0,255,255 );
    for ( itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MT_Vector2D vPos( (*itPoint)->GetPoint() );
        vPos += (*itPoint)->GetSize() / 2.;
        GFX_Tools::PrintWithShadows( vPos, rSizeName, 4., 8., colorName, false, GetName().c_str() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::IsInside
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimitCheckListItem::IsInsidePoint( const MT_Vector2D& vPos ) const
{
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        if ( pCurPoint->IsInside( vPos ) )
            return *itPoint;
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::IsInsideLine
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimitCheckListItem::IsInsideLine( const MT_Vector2D& vPos ) const
{
    MT_Float rWeldValue = 4.0;
    MOS_PointListViewItem* pLastPoint = 0;
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        if ( pLastPoint )
        {
            MT_Line line( pLastPoint->GetPoint(), pCurPoint->GetPoint() );
            MT_Vector2D vPosCol = line.ClosestPointOnLine( vPos );
            MT_Float rDistance = vPosCol.Distance( vPos );
            if(  rDistance < rWeldValue )
            {
                if ( pLastPoint )
                    return pLastPoint;
                return pCurPoint;
            }

        }
        pLastPoint = pCurPoint;
    }
    return 0;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::SetPos
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimitCheckListItem::SetPos( const MT_Vector2D& vPos,  MT_Vector2D& vPosMovingLine )
{
    MT_Vector2D vPosDelta( vPos - vPosMovingLine );
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() + vPosDelta );
    }
    vPosMovingLine += vPosDelta;
   
}

//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::GetCenterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
const MT_Vector2D MOS_LimitCheckListItem::GetCenterPoint( MOS_PointListViewItem* pPoint ) const
{
    if ( pPoint == 0 &&  pointVector_.size() )
        pPoint = *pointVector_.rbegin();

    // si pas encore de point Alors au centre de la carte
    if( pointVector_.size() == 0 )
        return MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().GetCenter();

    CIT_PointPtrVector itPoint = std::find( pointVector_.begin(), pointVector_.end(), pPoint );
    assert( itPoint != pointVector_.end() );

    // Si dernier point Alors ancien point + 10.0 en X
    if ( *itPoint == *pointVector_.rbegin() )
    {
        MT_Vector2D vNewPos( MT_Vector2D( (*itPoint)->GetPoint() + (*itPoint)->GetSize() * 2.0 ) );
        const MT_Rect& rectWorld = MOS_App::GetApp().GetWorld().GetRect();
        if ( rectWorld.IsInside( vNewPos ) )
            return vNewPos;
        else
            return (*itPoint)->GetPoint();

    }
        
    ++itPoint;
    return MT_Vector2D( ( pPoint->GetPoint() + (*itPoint)->GetPoint() ) / 2.0  );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::GetPointBefore
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimitCheckListItem::GetPointBefore( const MOS_PointListViewItem& point ) const
{
    CIT_PointPtrVector itPoint = std::find( pointVector_.begin(), pointVector_.end(), &point );
    assert( itPoint != pointVector_.end() );
    if ( *itPoint == *pointVector_.begin() )
    {
        return 0;
    }
    --itPoint;
    return *itPoint;
}
//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::SetName
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_LimitCheckListItem::SetName( const std::string sName )
{
    setText(0, sName.c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimitCheckListItem::UpdateNamePos
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------

void MOS_LimitCheckListItem::UpdateNamePos()
{
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() );
    }
}

