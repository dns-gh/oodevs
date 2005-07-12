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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBCampListViewItem.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 5 $
// $Workfile: MOS_ODBCampListViewItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_Pch.h"
#include "MOS_ODBCampListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ODBCampListViewItem.inl"
#endif

#include "MOS_ODBArmeListViewItem.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ODBListView.h"

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ODBCampListViewItem::MOS_ODBCampListViewItem( QListViewItem& item, E_TypeCamp nTypeCamp, const MT_Rect& rectLimit )
:       MOS_ListViewItem_ABC( eItemTypeODBCamp, item, BuildNameCamp( nTypeCamp ) )
,       nTypeCamp_( nTypeCamp )
,       bEditLeft_( false )
,       bEditRight_( false )
,       bEditCenter_( false )
,       colorRect_( 255,255,0, 0.3 )
{
    SetRandomPos( rectLimit );

    setRenameEnabled( 0, false );
    setOpen( true );
    SetName();

}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_ODBCampListViewItem::~MOS_ODBCampListViewItem()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::Initialize()
{
    CreateArme( eTypeArmeUnknown );
//    CreateArme( eTypeArmeAlat );
//    CreateArme( eTypeArmeInfanterie );
//    CreateArme( eTypeArmeBlinde );
//    CreateArme( eTypeArmeGenie );
//    CreateArme( eTypeArmeLog );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::Terminate()
{
    DeleteArme( eTypeArmeUnknown );
//    DeleteArme( eTypeArmeInfanterie );
//    DeleteArme( eTypeArmeBlinde );
//    DeleteArme( eTypeArmeGenie );
//    DeleteArme( eTypeArme3D );
//    DeleteArme( eTypeArmeAlat );
//    DeleteArme( eTypeArmeLog );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::AddArme
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
QListViewItem& MOS_ODBCampListViewItem::CreateArme( E_TypeArme nTypeArme )
{
    MOS_ODBArmeListViewItem* pArme = new MOS_ODBArmeListViewItem( *this, nTypeCamp_, nTypeArme, rectLimit_ );
    armePtrMap_.insert( std::make_pair( nTypeArme, pArme ) );
    return *pArme;
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::DeleteArme
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::DeleteArme( E_TypeArme nType )
{
    IT_ODBArmePtrMap itArme = armePtrMap_.find( nType );
    assert( itArme != armePtrMap_.end() );
    delete itArme->second;
    armePtrMap_.erase( itArme );
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: FBD 03-07-25
//-----------------------------------------------------------------------------
std::string MOS_ODBCampListViewItem::BuildNameCamp( E_TypeCamp nTypeCamp )
{
    std::string sName( "" );
    switch( nTypeCamp )
    {
        case eTypeCampAmi :     sName = "Ami";           break;
        case eTypeCampEnnemi:   sName = "Ennemi";           break;
        case eTypeCampNeutre:   sName = "Neutre";           break;
        case eTypeCampInconnu:  sName = "Inconnu";           break;
    }
    return sName;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::SetName
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::SetName()
{
    std::string sName( BuildNameCamp( nTypeCamp_ ) );
    sName += MT_FormatString( " - (%04d Pions)", GetNbSons() );
    setText( 0, sName.c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::GetNbSons
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBCampListViewItem::GetNbSons()
{
    uint nPions = 0;
    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        nPions += itArme->second->GetNbSons();
        itArme->second->SetName();
    }
    return nPions;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::SetRandomPos
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::SetRandomPos( const MT_Rect& rect )
{
    rectLimit_=  rect;
    uint nWidth = (uint)rect.GetWidth();
    if ( nWidth == 0 )
        nWidth = 1;
    uint nHeight = (uint)rect.GetHeight();
    if ( nHeight == 0 )
        nHeight = 1;

    vPos_ = MT_Vector2D( ( rand() % nWidth ) + rect.GetLeft() , ( rand() % nHeight ) + rect.GetBottom() );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::Draw
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::Draw( bool bSelected )
{
    GFX_Color color_( 255,255,255 );

    bSelected |= isSelected();

    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        itArme->second->Draw( bSelected );
    }

    if ( bSelected )
    {
        MT_Float rCrossSize = 100;
        GFX_Tools::CreateGLCross( vPos_, rCrossSize, 4.0, color_ );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::DrawRectLimit
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::DrawRectLimit( bool bSelected )
{
    bSelected |= isSelected();

    if ( bSelected )
    {
        MT_Float rSizeSelect = MOS_SIZESELECT;;
        GFX_Tools::CreateGLRectPoly( rectLimit_, colorRect_ );
        GFX_Tools::CreateGLRect( rectLimit_, 2.0, GFX_Color( 255, 0, 0, 0.5 ) );
        MT_Vector2D vPosLeft( rectLimit_.GetLeft(), rectLimit_.GetTop() );
        MT_Vector2D vPosRight( rectLimit_.GetRight(), rectLimit_.GetBottom() );
        MT_Vector2D vPosCenter( rectLimit_.GetCenter() );
        MT_Rect buttonLeft( vPosLeft.rX_ - rSizeSelect, vPosLeft.rY_ + rSizeSelect, vPosLeft.rX_ + rSizeSelect, vPosLeft.rY_ - rSizeSelect );
        MT_Rect buttonRight( vPosRight.rX_ - rSizeSelect, vPosRight.rY_ + rSizeSelect, vPosRight.rX_ + rSizeSelect, vPosRight.rY_ - rSizeSelect );
        MT_Rect buttonCenter( vPosCenter.rX_ - rSizeSelect, vPosCenter.rY_ + rSizeSelect, vPosCenter.rX_ + rSizeSelect, vPosCenter.rY_ - rSizeSelect );
        GFX_Tools::CreateGLButton( buttonLeft, GFX_Color( 255, 255, 255, 0.5 ), 0.6 );
        GFX_Tools::CreateGLButton( buttonRight, GFX_Color( 255, 255, 255, 0.5 ), 0.6 );
        GFX_Tools::CreateGLButton( buttonCenter, GFX_Color( 255, 255, 255, 0.5 ), 0.6 );
    }
    bSelected = false;

    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        itArme->second->DrawRectLimit( bSelected );
    }

}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::StartMoveExtent
// Created: FBD 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBCampListViewItem::StartMoveExtent( const MT_Vector2D& vPosGL )
{
    if ( isSelected() )
    {
        if ( bEditLeft_ || bEditRight_ || bEditCenter_ )
            return false;
        MT_Vector2D vPosTopLeft( rectLimit_.GetLeft(), rectLimit_.GetTop() );
        MT_Vector2D vPosBottomRight( rectLimit_.GetRight(), rectLimit_.GetBottom() );
        MT_Vector2D vPosCenter( rectLimit_.GetCenter() );

        MT_Float rSizeSelect = MOS_SIZESELECT;
        if ( vPosGL.Distance( vPosTopLeft ) < rSizeSelect )
        {
            bEditLeft_ = true;
        }
        else
        if ( vPosGL.Distance( vPosBottomRight ) < rSizeSelect )
        {
            bEditRight_ = true;
        }
        else
        if( vPosGL.Distance( vPosCenter ) < rSizeSelect )
        {
            bEditCenter_ = true;
        }
//        return ( bEditLeft_ || bEditRight_ || bEditCenter_ );
    }
//    else
    {
        for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
        {
            itArme->second->StartMoveExtent( vPosGL );
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::MoveExtent
// Created: MOS_ODBListView 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBCampListViewItem::MoveExtent( const MT_Vector2D& vPosGL )
{
    if ( bEditLeft_ )
    {
        rectLimit_ = MT_Rect( vPosGL.rX_, vPosGL.rY_, rectLimit_.GetRight(), rectLimit_.GetBottom() );
        ClipLimit();
        SetInside( rectLimit_ );
        UpdateAll();
        if ( parent() )
        {
            MOS_ListViewItem_ABC* pItem = (MOS_ListViewItem_ABC*)parent();
            if ( pItem->GetItemType() == eItemTypeNone )
            {
                if ( pItem->listView() )
                {
                    MOS_ODBListView* pListView = (MOS_ODBListView*)pItem->listView();
                    if ( pListView )
                    {
                        pListView->FitParent( rectLimit_ );
                    }
                }
            }
        }
        return true;
    }
    else
    if( bEditRight_ )
    {
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), vPosGL.rX_, vPosGL.rY_ );
        ClipLimit();
        SetInside( rectLimit_ );
        UpdateAll();
        if ( parent() )
        {
            MOS_ListViewItem_ABC* pItem = (MOS_ListViewItem_ABC*)parent();
            if ( pItem->GetItemType() == eItemTypeNone )
            {
                if ( pItem->listView() )
                {
                    MOS_ODBListView* pListView = (MOS_ODBListView*)pItem->listView();
                    if ( pListView )
                    {
                        pListView->FitParent( rectLimit_ );
                    }
                }
            }
        }
        return true;
    }
    else
    if ( bEditCenter_ )
    {
        SetCenter( rectLimit_.GetCenter() - vPosGL );
        UpdateAll();
        if ( parent() )
        {
            MOS_ListViewItem_ABC* pItem = (MOS_ListViewItem_ABC*)parent();
            if ( pItem->GetItemType() == eItemTypeNone )
            {
                if ( pItem->listView() )
                {
                    MOS_ODBListView* pListView = (MOS_ODBListView*)pItem->listView();
                    if ( pListView )
                    {
                        pListView->FitParent( rectLimit_ );
                    }
                }
            }
        }
        return true;
    }
    else
    {
        for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
        {
            if ( itArme->second->MoveExtent( vPosGL ) )
                return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::EndMoveExtent
// Created: FBD 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBCampListViewItem::EndMoveExtent()
{
    if ( bEditLeft_ || bEditRight_ || bEditCenter_ )
    {
        bEditLeft_ = false;
        bEditRight_ = false;
        bEditCenter_ = false;
    }
    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        itArme->second->EndMoveExtent();
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::SetInside
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::SetInside( const MT_Rect& rectLimit )
{
    if( rectLimit_.GetLeft() < rectLimit.GetLeft() )
        rectLimit_ = MT_Rect( rectLimit.GetLeft(), rectLimit_.GetTop(), rectLimit_.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetRight() > rectLimit.GetRight() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), rectLimit.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetTop() > rectLimit.GetTop() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit.GetTop(), rectLimit_.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetBottom() < rectLimit.GetBottom() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), rectLimit_.GetRight(), rectLimit.GetBottom() );

    ClipLimit();

    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        itArme->second->SetInside( rectLimit_ );
    }
}






//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::UpdateAll
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::UpdateAll()
{
    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        itArme->second->UpdateAll();
    }
    SetRandomPos( rectLimit_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::SetCenter
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::SetCenter( const MT_Vector2D& vDeltaPos )
{
    rectLimit_ = MT_Rect( rectLimit_.GetLeft() - vDeltaPos.rX_, rectLimit_.GetTop() - vDeltaPos.rY_, rectLimit_.GetRight() - vDeltaPos.rX_, rectLimit_.GetBottom() - vDeltaPos.rY_ );
    ClipLimit();

    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        itArme->second->SetCenter( vDeltaPos );
    }
  
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::FitParent
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::FitParent( const MT_Rect& rectLimit )
{
    if( rectLimit_.GetLeft() > rectLimit.GetLeft() )
        rectLimit_ = MT_Rect( rectLimit.GetLeft(), rectLimit_.GetTop(), rectLimit_.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetRight() < rectLimit.GetRight() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), rectLimit.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetTop() < rectLimit.GetTop() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit.GetTop(), rectLimit_.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetBottom() > rectLimit.GetBottom() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), rectLimit_.GetRight(), rectLimit.GetBottom() );

    ClipLimit();

    if ( parent() )
    {
        MOS_ListViewItem_ABC* pItem = (MOS_ListViewItem_ABC*)parent();
        if ( pItem->GetItemType() == eItemTypeNone )
        {
            if ( pItem->listView() )
            {
                MOS_ODBListView* pListView = (MOS_ODBListView*)pItem->listView();
                if ( pListView )
                {
                    pListView->FitParent( rectLimit_ );
                }
            }
        }
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::SetLimit
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::ClipLimit()
{
    const MT_Rect& rectWorld = MOS_App::GetApp().GetWorld().GetRect();
    if( rectLimit_.GetLeft() < rectWorld.GetLeft() )
        rectLimit_ = MT_Rect( rectWorld.GetLeft(), rectLimit_.GetTop(), rectLimit_.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetRight() > rectWorld.GetRight() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), rectWorld.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetTop() > rectWorld.GetTop() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectWorld.GetTop(), rectLimit_.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetBottom() < rectWorld.GetBottom() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), rectLimit_.GetRight(), rectWorld.GetBottom() );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::WriteAutomates
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::WriteAutomates( MT_OutputArchive_ABC& archive )
{
    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        itArme->second->WriteAutomates( archive );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::WritePions
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
void MOS_ODBCampListViewItem::WritePions( MT_OutputArchive_ABC& archive )
{
    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        itArme->second->WritePions( archive );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::GetNbAutomates
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBCampListViewItem::GetNbAutomates()
{
    uint nAutomates = 0;
    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        nAutomates += itArme->second->GetNbAutomates( );
    }
    return nAutomates;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBCampListViewItem::GetNbPions
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBCampListViewItem::GetNbPions()
{
    uint nPions = 0;
    for ( IT_ODBArmePtrMap itArme = armePtrMap_.begin() ; itArme != armePtrMap_.end() ; ++itArme )
    {
        nPions += itArme->second->GetNbPions( );
    }
    return nPions;
}