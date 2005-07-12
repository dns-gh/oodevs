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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBPionListViewItem.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 4 $
// $Workfile: MOS_ODBPionListViewItem.cpp $
//
//*****************************************************************************

#include "MOS_Light_Pch.h"
#include "MOS_ODBPionListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ODBPionListViewItem.inl"
#endif

#include "MOS_ODBTypePion.h"
#include "MOS_ODBNumberSelector.h"
#include "MOS_ListViewItem_ABC.h"
#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ODBAutomateListViewItem.h"
#include "MOS_ODBListView.h"

//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ODBPionListViewItem::MOS_ODBPionListViewItem( QListViewItem& item, E_TypeCamp nTypeCamp, E_TypeArme nTypeArme, MOS_ODBTypeAutomate* pTypeAutomate, MOS_ODBTypePion* pTypePion, const MT_Rect& rectLimit, uint nIdAutomate )
:       MOS_ListViewItem_ABC( eItemTypeODBPion, item )
,       nTypeCamp_      ( nTypeCamp )
,       nTypeArme_      ( nTypeArme )
,       pTypeAutomate_  ( pTypeAutomate )
,       pTypePion_      ( pTypePion )
,       bEditLeft_      ( false )
,       bEditRight_     ( false )
,       bEditCenter_    ( false )
,       colorRect_      ( 0,0,255, 0.3 )
,       nWantedPion_    ( 0 )
,       nId_            ( ((MOS_ODBListView*)listView())->GetIdAutomate() )
,       nIdAutomate_    ( nIdAutomate )
{
    SetName();
    setRenameEnabled( 0, false );
    setDragEnabled( true );
    setDropEnabled( false );
    SetRandomPos( rectLimit );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_ODBPionListViewItem::~MOS_ODBPionListViewItem()
{
    DeleteAllPions();
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::CreateMenu
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::CreateMenu( QPopupMenu* pMenu )
{
    pMenu->insertItem( "Set Number", 0 );
    pMenu->insertItem( "Delete Pion", 1 );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::ExecuteMenu
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::ExecuteMenu( int nClicked )
{
    if ( nClicked == 0 )
    {
        uint nValue = 0;
        MOS_ODBNumberSelector* pNumber = new MOS_ODBNumberSelector( &nValue, 0 );
        pNumber->exec();
        delete pNumber;
        nWantedPion_ = nValue;
        if ( ODBPionPtrVector_.size() != nWantedPion_ )
        {
            DeleteAllPions();
            CreatePions( nWantedPion_ );
        }

    }
    else
    if ( nClicked == 1 )
    {
        if ( parent() )
        {
            if ( ((MOS_ListViewItem_ABC*)parent())->GetItemType() == eItemTypeODBAutomate )
                ((MOS_ODBAutomateListViewItem*)parent())->DeletePion( this );
            else
            if ( ((MOS_ListViewItem_ABC*)parent())->GetItemType() == eItemTypeODBPion )
                ((MOS_ODBPionListViewItem*)parent())->DeletePion( this );
        }
        return;
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::UpdateAll
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::UpdateAll()
{
    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->UpdateAll();
    }
    SetRandomPos( rectLimit_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::DeleteAllPions
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::DeleteAllPions()
{
    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->DeleteAllPions();
        delete *itPion;
    }
    ODBPionPtrVector_.clear();
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::DeletePion
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::DeletePion( MOS_ODBPionListViewItem* pPion )
{
    assert( pPion );
    IT_ODBPionPtrVector itPion = std::find( ODBPionPtrVector_.begin() , ODBPionPtrVector_.end() , pPion );
    assert( itPion != ODBPionPtrVector_.end() );
    ODBPionPtrVector_.erase( itPion );
    delete pPion;
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::CreatePions
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::CreatePions( uint nValue )
{
    if ( nValue < 1 )
        return;

    for ( uint i = 0 ; i < nValue - 1 ; ++i )
    {
        MOS_ODBPionListViewItem* pPion = new MOS_ODBPionListViewItem( *this, nTypeCamp_, nTypeArme_, pTypeAutomate_, pTypePion_, rectLimit_, nIdAutomate_ );
        ODBPionPtrVector_.push_back( pPion );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::GetNbSons
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBPionListViewItem::GetNbSons() const
{
    uint nValue = 1;
    for ( CIT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
        nValue += (*itPion)->GetNbSons();

    return nValue;
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::SetName
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::SetName()
{
    std::string sName( "Pion : " );
    sName += pTypePion_->GetName();
    sName += MT_FormatString( " - (%04d Pions)", GetNbSons() );
    setText( 0, sName.c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::SetRandomPos
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::SetRandomPos( const MT_Rect& rect )
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
// Name: MOS_ODBPionListViewItem::Draw
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::Draw( bool bSelected )
{
    bSelected |= isSelected();

    GFX_Color color_( 255,255,255 );
    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->Draw( bSelected );
    }

    if ( bSelected )
    {

        MT_Float rSize = 600. ;
        GFX_Tools::CreateGLAgentShadow( vPos_, rSize, 4., 8., color_ , true, pTypePion_->GetSymbol(), 100. );
        MT_Float rCrossSize = 100 ;
        GFX_Tools::CreateGLCross( vPos_, rCrossSize, 4.0, color_ );
    }

}


//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::DrawRectLimit
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::DrawRectLimit( bool bSelected )
{
    bSelected = isSelected();

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

    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->DrawRectLimit( bSelected );
    }



}



//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::StartMoveExtent
// Created: FBD 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBPionListViewItem::StartMoveExtent( const MT_Vector2D& vPosGL )
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
        for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
        {
             (*itPion)->StartMoveExtent( vPosGL );
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::MoveExtent
// Created: MOS_ODBListView 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBPionListViewItem::MoveExtent( const MT_Vector2D& vPosGL )
{
    if ( bEditLeft_ )
    {
        rectLimit_ = MT_Rect( vPosGL.rX_, vPosGL.rY_, rectLimit_.GetRight(), rectLimit_.GetBottom() );
        ClipLimit();
        SetInside( rectLimit_, true );
        UpdateAll();
        if ( parent() )
        {
            if ( ((MOS_ListViewItem_ABC*)parent())->GetItemType() == eItemTypeODBAutomate )
                ((MOS_ODBAutomateListViewItem*)parent())->FitParent( rectLimit_ );
            else
            if ( ((MOS_ListViewItem_ABC*)parent())->GetItemType() == eItemTypeODBPion )
                ((MOS_ODBPionListViewItem*)parent())->FitParent( rectLimit_ );
        }
        return true;
    }
    else
    if( bEditRight_ )
    {
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), vPosGL.rX_, vPosGL.rY_ );
        ClipLimit();
        SetInside( rectLimit_, true );
        UpdateAll();
        if ( parent() )
        {
            if ( ((MOS_ListViewItem_ABC*)parent())->GetItemType() == eItemTypeODBAutomate )
                ((MOS_ODBAutomateListViewItem*)parent())->FitParent( rectLimit_ );
            else
            if ( ((MOS_ListViewItem_ABC*)parent())->GetItemType() == eItemTypeODBPion )
                ((MOS_ODBPionListViewItem*)parent())->FitParent( rectLimit_ );
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
            if ( ((MOS_ListViewItem_ABC*)parent())->GetItemType() == eItemTypeODBAutomate )
                ((MOS_ODBAutomateListViewItem*)parent())->FitParent( rectLimit_ );
            else
            if ( ((MOS_ListViewItem_ABC*)parent())->GetItemType() == eItemTypeODBPion )
                ((MOS_ODBPionListViewItem*)parent())->FitParent( rectLimit_ );
        }
        return true;
    }
    else
    {
        for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
        {
            if ( (*itPion)->MoveExtent( vPosGL ) )
                return true;
        }
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::EndMoveExtent
// Created: FBD 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBPionListViewItem::EndMoveExtent()
{
    if ( bEditLeft_ || bEditRight_ || bEditCenter_ )
    {
        bEditLeft_ = false;
        bEditRight_ = false;
        bEditCenter_ = false;
    }

    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->EndMoveExtent();
    }
    return false;
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::SetInside
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::SetInside( const MT_Rect& rectLimit, bool bForce )
{
    if ( bForce )
        rectLimit_ = rectLimit;

    if( rectLimit_.GetLeft() < rectLimit.GetLeft() )
        rectLimit_ = MT_Rect( rectLimit.GetLeft(), rectLimit_.GetTop(), rectLimit_.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetRight() > rectLimit.GetRight() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), rectLimit.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetTop() > rectLimit.GetTop() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit.GetTop(), rectLimit_.GetRight(), rectLimit_.GetBottom() );

    if( rectLimit_.GetBottom() < rectLimit.GetBottom() )
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), rectLimit_.GetRight(), rectLimit.GetBottom() );

    ClipLimit();

    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->SetInside( rectLimit_, bForce );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::SetCenter
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::SetCenter( const MT_Vector2D& vDeltaPos )
{
    rectLimit_ = MT_Rect( rectLimit_.GetLeft() - vDeltaPos.rX_, rectLimit_.GetTop() - vDeltaPos.rY_, rectLimit_.GetRight() - vDeltaPos.rX_, rectLimit_.GetBottom() - vDeltaPos.rY_ );

    ClipLimit();

    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->SetCenter( vDeltaPos );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::FitParent
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::FitParent( const MT_Rect& rectLimit )
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
        if ( ((MOS_ListViewItem_ABC*)parent())->GetItemType() == eItemTypeODBAutomate )
            ((MOS_ODBAutomateListViewItem*)parent())->FitParent( rectLimit_ );
        else
        if ( ((MOS_ListViewItem_ABC*)parent())->GetItemType() == eItemTypeODBPion )
            ((MOS_ODBPionListViewItem*)parent())->FitParent( rectLimit_ );
    }
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::SetLimit
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::ClipLimit()
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
// Name: MOS_ODBPionListViewItem::WritePions
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
void MOS_ODBPionListViewItem::WritePions( MT_OutputArchive_ABC& archive )
{
    archive.Section( "Agent" );

    // Read the agent category
    std::string strCategory;
    archive.WriteAttribute( "categorie", pTypePion_->GetName() );

    archive.WriteField( "ID", nId_ );

    archive.WriteField( "Automate", nIdAutomate_ );

    std::string strMGRS;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vPos_, strMGRS );
    archive.WriteField( "Position", strMGRS );

    archive.EndSection(); // Agent

    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->WritePions( archive );
    }


}

//-----------------------------------------------------------------------------
// Name: MOS_ODBPionListViewItem::GetNbPions
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBPionListViewItem::GetNbPions()
{
    uint nPions = 1;
    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        nPions += (*itPion)->GetNbPions();
    }
    return nPions;
}