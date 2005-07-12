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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBArmeListViewItem.cpp $
// $Author: Nld $
// $Modtime: 25/10/04 15:59 $
// $Revision: 5 $
// $Workfile: MOS_ODBArmeListViewItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_Pch.h"
#include "MOS_ODBArmeListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ODBArmeListViewItem.inl"
#endif

#include "MOS_ODBAutomateListViewItem.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ODBListView.h"
#include "MOS_ODBTypeAutomate.h"
#include "MOS_ODBCampListViewItem.h"

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ODBArmeListViewItem::MOS_ODBArmeListViewItem( QListViewItem& item, E_TypeCamp nTypeCamp, E_TypeArme nTypeArme, const MT_Rect& rectLimit )
:       MOS_ListViewItem_ABC( eItemTypeODBArme, item, BuildNameArme( nTypeArme ) )
,       nTypeCamp_( nTypeCamp )
,       nTypeArme_( nTypeArme )
,       bEditLeft_( false )
,       bEditRight_( false )
,       bEditCenter_( false )
,       colorRect_( 0,255,255, 0.3 )
{
    setRenameEnabled( 0, false );
    setOpen( true );
    SetRandomPos( rectLimit );
/*
    switch ( nTypeArme_ )
    {
        case eTypeArmeInfanterie:
        {
            CreateTypeAutomate( "Compagnie INF", "4I" );
        }
        break;

        case eTypeArmeBlinde :
        {
            CreateTypeAutomate( "Escadron XL",  "4A" );
            CreateTypeAutomate( "Peloton ER",   "4A" );
            CreateTypeAutomate( "Escadron AMX", "4A" );
        }
        break;

        case eTypeArmeGenie :
        {
            CreateTypeAutomate( "Compagnie GEN",        "4E" );
            CreateTypeAutomate( "Compagnie GEN APPUI",  "4E" );
        }
        break;

        case eTypeArme3D :
        {
            CreateTypeAutomate( "CompagnieART",         "4#" );
        }
        break;
        
        case eTypeArmeAlat :
        {
        }
        break;

        case eTypeArmeLog :
        {
            CreateTypeAutomate( "PC Soutien Sanitaire", "4F" );
        }
        break;
    }*/
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_ODBArmeListViewItem::~MOS_ODBArmeListViewItem()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::Initialize()
{
    for ( IT_ODBTypeAutomatePtrVector itTypeAutomate = ODBTypeAutomatePtrVector_.begin() ; itTypeAutomate != ODBTypeAutomatePtrVector_.end() ; ++itTypeAutomate )
        CreateAutomate( *itTypeAutomate );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::Terminate()
{
    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
        delete *itAutomate;
    ODBAutomatePtrVector_.clear();
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::AddAutomate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
QListViewItem& MOS_ODBArmeListViewItem::CreateAutomate( MOS_ODBTypeAutomate* pTypeAutomate )
{
    MOS_ODBAutomateListViewItem* pAutomateODBArme = new MOS_ODBAutomateListViewItem( *this, nTypeCamp_, nTypeArme_, pTypeAutomate, rectLimit_ );

    ODBAutomatePtrVector_.push_back( pAutomateODBArme );

    return ( *pAutomateODBArme );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::CreateTypeAutomate
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::CreateTypeAutomate( const std::string& sName, const std::string& sSymbol )
{
    ODBTypeAutomatePtrVector_.push_back( new MOS_ODBTypeAutomate( sName, sSymbol ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::DeleteAllTypeAutomate
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::DeleteAllTypeAutomate()
{
    for ( IT_ODBTypeAutomatePtrVector itTypeAutomate = ODBTypeAutomatePtrVector_.begin() ; itTypeAutomate != ODBTypeAutomatePtrVector_.end() ; ++itTypeAutomate )
        delete *itTypeAutomate;
    ODBTypeAutomatePtrVector_.clear();
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::DeleteAutomate
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::DeleteAutomate( QListViewItem* pAutomate  )
{
    assert( pAutomate );
    IT_ODBAutomatePtrVector itAutomate = std::find( ODBAutomatePtrVector_.begin() , ODBAutomatePtrVector_.end() , pAutomate );
    assert( itAutomate != ODBAutomatePtrVector_.end() );
    ODBAutomatePtrVector_.erase( itAutomate );
    delete pAutomate;
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: FBD 03-07-25
//-----------------------------------------------------------------------------
std::string MOS_ODBArmeListViewItem::BuildNameArme( E_TypeArme nTypeArme )
{
    std::string sName( "" );
    switch( nTypeArme )
    {
//        case eTypeArme3D         : sName = "3D";           break;
//    case eTypeArmeBlinde     : sName = "Blinde";       break;
//    case eTypeArmeGenie      : sName = "Genie";        break;
//    case eTypeArmeInfanterie : sName = "Infanterie";   break;
//    case eTypeArmeAlat       : sName = "Alat";         break;
//    case eTypeArmeLog        : sName = "Logistique";   break;
        case eTypeArmeUnknown : sName = "ODB"; break;
    }
    return sName;
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::CreateMenu
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::CreateMenu( QPopupMenu* pMenu )
{
    uint i = 0;
    for ( IT_ODBTypeAutomatePtrVector itAutomate = ODBTypeAutomatePtrVector_.begin() ; itAutomate != ODBTypeAutomatePtrVector_.end() ; ++itAutomate )
    {
        pMenu->insertItem( (*itAutomate)->GetName().c_str(), i );
        ++i;
    }
    
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::ExecuteMenu
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::ExecuteMenu( int nClicked )
{
    assert( nClicked < (int)ODBTypeAutomatePtrVector_.size() );
    CreateAutomate( ODBTypeAutomatePtrVector_[nClicked] );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::SetName
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::SetName()
{
    std::string sName( BuildNameArme( nTypeArme_ ) );
    sName += MT_FormatString( " - (%04d Pions)", GetNbSons() );
    setText( 0, sName.c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::GetNbSons
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBArmeListViewItem::GetNbSons()
{
    uint nPions = 0;
    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        nPions += (*itAutomate)->GetNbSons();
        (*itAutomate)->SetName();
    }
    return nPions;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::SetRandomPos
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::SetRandomPos( const MT_Rect& rect )
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
// Name: MOS_ODBArmeListViewItem::Draw
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::Draw( bool bSelected )
{

    GFX_Color color_( 255,255,255 );

    bSelected |= isSelected();

    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        (*itAutomate)->Draw( bSelected );
    }
    
    if ( bSelected )
    {
//        MT_Float rCrossSize = 100 / MOS_App::GetApp().GetWorld().GetExtent().GetMeterPerPixel();
//        GFX_Tools::CreateGLCross( vPos_, rCrossSize, 4.0, color_ );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::DrawRectLimit
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::DrawRectLimit( bool bSelected )
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

    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        (*itAutomate)->DrawRectLimit( bSelected );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::StartMoveExtent
// Created: FBD 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBArmeListViewItem::StartMoveExtent( const MT_Vector2D& vPosGL )
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
        for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
        {
            (*itAutomate)->StartMoveExtent( vPosGL );
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::MoveExtent
// Created: MOS_ODBListView 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBArmeListViewItem::MoveExtent( const MT_Vector2D& vPosGL )
{
    if ( bEditLeft_ )
    {
        rectLimit_ = MT_Rect( vPosGL.rX_, vPosGL.rY_, rectLimit_.GetRight(), rectLimit_.GetBottom() );
        ClipLimit();
        SetInside( rectLimit_ );
        UpdateAll();
        if ( parent() )
            ((MOS_ODBCampListViewItem*)parent())->FitParent( rectLimit_ );
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
            ((MOS_ODBCampListViewItem*)parent())->FitParent( rectLimit_ );
        return true;
    }
    else
    if ( bEditCenter_ )
    {
        SetCenter( rectLimit_.GetCenter() - vPosGL );
        UpdateAll();
        if ( parent() )
            ((MOS_ODBCampListViewItem*)parent())->FitParent( rectLimit_ );
        return true;
    }
    else
    {
        for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
        {
            if ( (*itAutomate)->MoveExtent( vPosGL ) )
                return true;

        }
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::EndMoveExtent
// Created: FBD 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBArmeListViewItem::EndMoveExtent()
{
    if ( bEditLeft_ || bEditRight_ || bEditCenter_ )
    {
        bEditLeft_ = false;
        bEditRight_ = false;
        bEditCenter_ = false;
    }
    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        (*itAutomate)->EndMoveExtent();
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::SetInside
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::SetInside( const MT_Rect& rectLimit )
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

    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        (*itAutomate)->SetInside( rectLimit_ );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::UpdateAll
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::UpdateAll()
{
    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        (*itAutomate)->UpdateAll();
    }
    SetRandomPos( rectLimit_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::SetCenter
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::SetCenter( const MT_Vector2D& vDeltaPos )
{
    rectLimit_ = MT_Rect( rectLimit_.GetLeft() - vDeltaPos.rX_, rectLimit_.GetTop() - vDeltaPos.rY_, rectLimit_.GetRight() - vDeltaPos.rX_, rectLimit_.GetBottom() - vDeltaPos.rY_ );
    ClipLimit();

    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        (*itAutomate)->SetCenter( vDeltaPos );
    }
  
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::FitParent
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::FitParent( const MT_Rect& rectLimit )
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
        ((MOS_ODBCampListViewItem*)parent())->FitParent( rectLimit_ );
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::SetLimit
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::ClipLimit()
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
// Name: MOS_ODBArmeListViewItem::WriteAutomates
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::WriteAutomates( MT_OutputArchive_ABC& archive )
{
    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        (*itAutomate)->WriteAutomate( archive );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::WritePions
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
void MOS_ODBArmeListViewItem::WritePions( MT_OutputArchive_ABC& archive )
{
    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        (*itAutomate)->WritePions( archive );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::GetNbAutomates
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBArmeListViewItem::GetNbAutomates()
{
    uint nAutomates = 0;
    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        nAutomates += (*itAutomate)->GetNbAutomates( );
    }
    return nAutomates;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBArmeListViewItem::GetNbPions
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBArmeListViewItem::GetNbPions()
{
    uint nPions = 0;
    for ( IT_ODBAutomatePtrVector itAutomate = ODBAutomatePtrVector_.begin() ; itAutomate != ODBAutomatePtrVector_.end() ; ++itAutomate )
    {
        nPions += (*itAutomate)->GetNbPions( );
    }
    return nPions;
}