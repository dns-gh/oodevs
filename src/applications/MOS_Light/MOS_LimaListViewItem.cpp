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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_LimaListViewItem.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 15 $
// $Workfile: MOS_LimaListViewItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_Pch.h"
#include "MOS_LimaListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_LimaListViewItem.inl"
#endif

#include "MOS_Lima.h"
#include "MOS_PointListViewItem.h"
#include "MOS_App.h"
#include "MOS_LimaFuncCheckListItem.h"
#include "MOS_LimaListView.h"
#include "MOS_Agent.h"
#include "MOS_World.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_LimaListViewItem::MOS_LimaListViewItem( E_ItemType nItemType, MOS_Lima& lima, QListView& listView, QListViewItem& itemAfter, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, listView, itemAfter, strField, strValue )
    , lima_          ( lima )
{
    setRenameEnabled( 0, true );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_LimaListViewItem::MOS_LimaListViewItem( E_ItemType nItemType, MOS_Lima& lima, QListViewItem& item, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, item, strField, strValue )
    , lima_          ( lima )
{
    setRenameEnabled( 0, true );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_LimaListViewItem::MOS_LimaListViewItem( E_ItemType nItemType,  MOS_Lima& lima, QListView& listView, const std::string& strField, const std::string& strValue )
    : MOS_ListViewItem_ABC( nItemType, listView, strField, strValue )
    , lima_          ( lima )
{
    setRenameEnabled( 0, true );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_LimaListViewItem::~MOS_LimaListViewItem()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::AddPoint
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem& MOS_LimaListViewItem::CreatePoint( MT_Vector2D& vPoint, MOS_PointListViewItem* pPointAfter )
{
    MOS_PointListViewItem* pPointLima = 0;
    if ( pointVector_.size() )
    {
        if ( pPointAfter )
            pPointLima = new MOS_PointListViewItem( eItemTypeEltPoint, vPoint, *this, *pPointAfter );
        else
            pPointLima = new MOS_PointListViewItem( eItemTypeEltPoint, vPoint, *this, **pointVector_.rbegin() );
    }
    else
        pPointLima = new MOS_PointListViewItem( eItemTypeEltPoint, vPoint, *this );

    RegisterPoint( *pPointLima, pPointAfter );
    return ( *pPointLima );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::DeletePoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_LimaListViewItem::DeletePoint( MT_Vector2D& vPoint )
{
    MOS_PointListViewItem* pPoint = GetPoint( vPoint );
    assert( pPoint );
    UnregisterPoint( *pPoint );
    delete pPoint;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_LimaListViewItem::Draw( MOS_LimaListViewItem* /*pSelectedLima*/, MOS_PointListViewItem* pSelectedPoint ) const
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

    MT_Float rSizeName = 500.;

    GFX_Color colorName( 0,255,255 );
    for ( itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MT_Vector2D vPos( (*itPoint)->GetPoint() );
        vPos += (*itPoint)->GetSize() / 2.;
        GFX_Tools::PrintWithShadows( vPos, rSizeName, 4., 8., colorName, false, GetName().c_str() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::IsInside
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimaListViewItem::IsInsidePoint( const MT_Vector2D& vPos ) const
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
// Name: MOS_LimaListViewItem::IsInsideLine
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimaListViewItem::IsInsideLine( const MT_Vector2D& vPos ) const
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
// Name: MOS_LimaListViewItem::SetPos
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_LimaListViewItem::SetPos( const MT_Vector2D& vPos,  MT_Vector2D& vPosMovingLine )
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
// Name: MOS_LimaListViewItem::GetCenterPoint
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
const MT_Vector2D MOS_LimaListViewItem::GetCenterPoint( MOS_PointListViewItem* pPoint ) const
{
    MT_Vector2D vNewPos;

    MOS_LimaListView* pListView = (MOS_LimaListView*)listView();
    uint nPoint = pointVector_.size();

    if ( pPoint == 0 && nPoint )
        pPoint = *pointVector_.rbegin();

    if( pointVector_.size() )
    {
        CIT_PointPtrVector itPoint = std::find( pointVector_.begin(), pointVector_.end(), pPoint );
        assert( itPoint != pointVector_.end() );

        if( nPoint > 1 )
        {
            // Si premier point
            if ( *itPoint == *pointVector_.begin() )
            {
                MOS_PointListViewItem* pPointAfter = GetPointAfter( **itPoint );
                assert( pPointAfter );
                vNewPos = ( ( pPointAfter->GetPos() - (*itPoint)->GetPos() ) / 2.0 + (*itPoint)->GetPos() );

            }
            else
            {
                MOS_PointListViewItem* pPointBefore = GetPointBefore( **itPoint );
                assert( pPointBefore );
                vNewPos = ( pPoint->GetPos() - pPointBefore->GetPos()  + pPoint->GetPos() );
            }
        }
        else
        {
            MT_Vector2D vPos = (*itPoint)->GetPos();
            vNewPos = MT_Vector2D( vPos.rX_ + ( (MT_Float)( rand() % 50 ) ), vPos.rY_ + ( (MT_Float)( rand() % 50 ) )  );
        }
    }
    else
    {
        vNewPos = MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().GetCenter();
        // Truc imbitable pour que ca marche la faute à FBD :p
        vNewPos.rY_ = - vNewPos.rY_ + MOS_App::GetApp().GetWorld().GetRect().GetHeight();
    }
    //. Clipping dans le monde
    const MT_Rect& rectWorld = MOS_App::GetApp().GetWorld().GetRect();
    if ( !rectWorld.IsInside( vNewPos ) )
    {
        if ( pPoint )
            vNewPos = pPoint->GetPoint();
                        
    }
    
    pListView->SetOldPos( vNewPos );

    return vNewPos;    
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::GetPointBefore
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimaListViewItem::GetPointBefore( const MOS_PointListViewItem& point ) const
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
// Name: MOS_LimaListViewItem::GetPointAfter
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
MOS_PointListViewItem* MOS_LimaListViewItem::GetPointAfter( const MOS_PointListViewItem& point ) const
{
    CIT_PointPtrVector itPoint = std::find( pointVector_.begin(), pointVector_.end(), &point );
    assert( itPoint != pointVector_.end() );
    if ( *itPoint == *pointVector_.rbegin() )
    {
        return 0;
    }
    ++itPoint;
    return *itPoint;
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::SetName
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------
void MOS_LimaListViewItem::SetName( const std::string sName )
{
    setText(0, sName.c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::UpdateNamePos
// Created: FBD 03-01-02
//-----------------------------------------------------------------------------

void MOS_LimaListViewItem::UpdateNamePos()
{
    for ( CIT_PointPtrVector itPoint = pointVector_.begin() ; itPoint != pointVector_.end() ; ++itPoint )
    {
        MOS_PointListViewItem* pCurPoint = *itPoint;
        pCurPoint->SetPos( pCurPoint->GetPoint() );
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::CreateFuncLima
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
MOS_LimaFuncCheckListItem* MOS_LimaListViewItem::CreateFuncLima( E_FuncLimaType nFuncType )
{
    MOS_LimaFuncCheckListItem* pFunc = new MOS_LimaFuncCheckListItem( nFuncType, *this );
    RegisterFuncLima( *pFunc );
    if ( lima_.GetTypeFunc() == nFuncType )
    {
        pFunc->setOn( true );
    }
    return pFunc;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::DeleteFuncLima
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_LimaListViewItem::DeleteFuncLima( MOS_LimaFuncCheckListItem& func )
{
    UnregisterFuncLima( func );
    delete &func;
    
}
//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::RegisterFuncLima
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_LimaListViewItem::RegisterFuncLima( MOS_LimaFuncCheckListItem& func )
{
    funcVector_.push_back( &func );
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::UnregisterFuncLima
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_LimaListViewItem::UnregisterFuncLima( MOS_LimaFuncCheckListItem& func )
{
    IT_FuncPtrVector itFunc = std::find( funcVector_.begin() , funcVector_.end() , &func );
    assert( itFunc != funcVector_.end() );
    funcVector_.erase( itFunc );
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::GetNbSelectFuncLima
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
int MOS_LimaListViewItem::GetNbSelectFuncLima() const
{
    int nFunc = 0;
    for ( CIT_FuncPtrVector itFunc = funcVector_.begin() ; itFunc != funcVector_.end() ; ++itFunc )
    {
        if ( (*itFunc)->isOn() )
            ++nFunc;
    }
    return nFunc;
}


//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::WriteMsg
// Created: NLD 2003-03-01
//-----------------------------------------------------------------------------
void MOS_LimaListViewItem::WriteMsg( ASN1T_OID& asnOID, std::string& sParam )
{
    asnOID = lima_.GetID();
    sParam += "----->";
    sParam += lima_.GetName();
    sParam += " | ";
}

//-----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::FuncStateChanged
// Created: FBD 03-02-28
//-----------------------------------------------------------------------------
void MOS_LimaListViewItem::FuncStateChanged( MOS_LimaFuncCheckListItem& funcItem )
{
    for ( CIT_FuncPtrVector itFunc = funcVector_.begin() ; itFunc != funcVector_.end() ; ++itFunc )
    {
        if ( (*itFunc) != &funcItem )
        {
            (*itFunc)->setOn( false );
        }
        lima_.SetTypeFunc( funcItem.GetType() );
        lima_.SetState( eStateLimaCoordModified );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_LimaListViewItem::SetFuncState
// Created: NLD 2004-05-25
// -----------------------------------------------------------------------------
void MOS_LimaListViewItem::SetFuncState( E_FuncLimaType nFuncType )
{
    for ( CIT_FuncPtrVector itFunc = funcVector_.begin() ; itFunc != funcVector_.end() ; ++itFunc )
    {
        if ( (**itFunc).GetType() == nFuncType )
            (*itFunc)->setOn( true );
        else
            (*itFunc)->setOn( false );
    }    
}
