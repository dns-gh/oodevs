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
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBAutomateListViewItem.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 5 $
// $Workfile: MOS_ODBAutomateListViewItem.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"
#include "MOS_ODBAutomateListViewItem.h"

#ifndef MOS_USE_INLINE
#   include "MOS_ODBAutomateListViewItem.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_ArmeListViewItem.h"
#include "MOS_ODBCampListViewItem.h"
#include "MOS_ODBListView.h"

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem constructor
// Created: NLD 2002-07-17
//-----------------------------------------------------------------------------
MOS_ODBAutomateListViewItem::MOS_ODBAutomateListViewItem( QListViewItem& item, E_TypeCamp nTypeCamp, E_TypeArme nTypeArme, MOS_ODBTypeAutomate* pTypeAutomate, const MT_Rect& rectLimit )
:       MOS_ListViewItem_ABC( eItemTypeODBAutomate, item )
,       nTypeCamp_( nTypeCamp )
,       nTypeArme_( nTypeArme )
,       pTypeAutomate_( pTypeAutomate )
,       bEditLeft_( false )
,       bEditRight_( false )
,       bEditCenter_( false )
,       colorRect_( 255,0,255, 0.3 )
,       nId_( ((MOS_ODBListView*)listView())->GetIdAutomate() )
{
    setRenameEnabled( 0, false );
    setOpen( true );
    setDropEnabled( true );
    setText( 0, pTypeAutomate_->GetName().c_str() );
    SetRandomPos( rectLimit );
/*
    switch ( nTypeArme_ )
    {
        case eTypeArmeInfanterie:
        {
            CreateTypePion( "Section INF VAB", "3H" );
            CreateTypePion( "Section INF HOT", "3*" );
            CreateTypePion( "Section INF MILAN", "3*" );
        }
        break;

        case eTypeArmeBlinde :
        {
            CreateTypePion( "Peloton XL", "3A" );
            CreateTypePion( "Peloton AMX", "3C" );
            CreateTypePion( "Peloton ERC", "3'" );
            CreateTypePion( "Peloton ADB", "3B" );
            CreateTypePion( "Peloton  ADF", "3B" );
            CreateTypePion( "Patrouille EI MILAN", "2J" );
            CreateTypePion( "Patrouille EI 12.7", "2J" );
            CreateTypePion( "Patrouille EI 7.62", "2J" );
        }
        break;

        case eTypeArmeGenie :
        {
            CreateTypePion( "Section GEN", "3D" );
            CreateTypePion( "Section SOUVIM", "2a" );
            CreateTypePion( "Groupe EBG", "2a" );
            CreateTypePion( "Groupe MPG", "2a" );
            CreateTypePion( "Groupe PAA", "2a" );
            CreateTypePion( "Groupe DISP", "2a" );
        }
        break;

        case eTypeArme3D :
        {
            CreateTypePion( "Section AUF1", "3(" );
            CreateTypePion( "Section Reco AUF1",    "3\xDA" );
            CreateTypePion( "Section Reco TRF1",    "3\xDA" );
            CreateTypePion( "Section Reco SAM",     "3\xDA" );
            CreateTypePion( "Lanceur LRM",          "3("    );
            CreateTypePion( "Section Reco LRM",     "3\xDA" );
            CreateTypePion( "Section Reco COBRA",   "3\xDA" );
            CreateTypePion( "COBRA",                "3#"    );
            CreateTypePion( "EO",                   "3\xDA" );
            CreateTypePion( "RATAC",                "3\xDA" );
            CreateTypePion( "Section Roland",       "3\xDA" );
            CreateTypePion( "Patrouille HR",        "3S"    );
        }
        break;
        
        case eTypeArmeAlat :
        {
        }
        break;

        case eTypeArmeLog :
        {
            CreateTypePion( "Poste Secours",    "3F" );
            CreateTypePion( "Section Ramassage","3F" );
            CreateTypePion( "Section Triage",   "3F" );
        }
        break;
    }
*/
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem destructor
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
MOS_ODBAutomateListViewItem::~MOS_ODBAutomateListViewItem()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::CreateODBPion
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_ODBPionListViewItem& MOS_ODBAutomateListViewItem::CreatePion( MOS_ODBTypePion* pTypePion )
{
    MOS_ODBPionListViewItem* pODBPionODBAutomate = new MOS_ODBPionListViewItem( *this, nTypeCamp_, nTypeArme_, pTypeAutomate_, pTypePion, rectLimit_, nId_ );
    ODBPionPtrVector_.push_back( pODBPionODBAutomate );
    return ( *pODBPionODBAutomate );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::DeleteODBPion
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::DeletePion( MOS_ODBPionListViewItem* pODBPion )
{
    assert( pODBPion );
    IT_ODBPionPtrVector itPion = std::find( ODBPionPtrVector_.begin() , ODBPionPtrVector_.end() , pODBPion );
    assert( itPion != ODBPionPtrVector_.end() );
    ODBPionPtrVector_.erase( itPion );
    delete pODBPion;
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::CreateTypePion
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::CreateTypePion( const std::string& sName, const std::string& sSymbol )
{
    ODBTypePionPtrVector_.push_back( new MOS_ODBTypePion( sName, sSymbol ) );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::DeleteAllTypePion
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::DeleteAllTypePion()
{
    for ( IT_ODBTypePionPtrVector itTypePion = ODBTypePionPtrVector_.begin() ; itTypePion != ODBTypePionPtrVector_.end() ; ++itTypePion )
        delete *itTypePion;
    ODBTypePionPtrVector_.clear();

}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::CreateMenu
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::CreateMenu( QPopupMenu* pMenu )
{
    uint i = 0;
    for ( IT_ODBTypePionPtrVector itPion = ODBTypePionPtrVector_.begin() ; itPion != ODBTypePionPtrVector_.end() ; ++itPion )
    {
        pMenu->insertItem( (*itPion)->GetName().c_str(), i );
        ++i;
    }
    
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::ExecuteMenu
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::ExecuteMenu( int nClicked )
{
    assert( nClicked < (int)ODBTypePionPtrVector_.size() );
    CreatePion( ODBTypePionPtrVector_[nClicked] );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::GetNbSons
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBAutomateListViewItem::GetNbSons()
{
    uint nValue = 1;
    for ( CIT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        nValue += (*itPion)->GetNbSons();
        (*itPion)->SetName();
    }

    return nValue;
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::SetName
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::SetName()
{
    std::string sName( "Automate : " );
    sName += pTypeAutomate_->GetName();
    sName += MT_FormatString( " - (%04d Pions)", GetNbSons() );
    setText( 0, sName.c_str() );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::SetRandomPos
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::SetRandomPos( const MT_Rect& rect )
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
// Name: MOS_ODBAutomateListViewItem::Draw
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::Draw( bool bSelected )
{
    bSelected |= isSelected();

    GFX_Color color_( 255,255,255 );

    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->Draw( true );
    }

    if ( bSelected )
    {
        MT_Float rSize = 600.;
        GFX_Tools::CreateGLAgentShadow( vPos_, rSize, 4., 8., color_ , true, pTypeAutomate_->GetSymbol(), 100.);
        MT_Float rCrossSize = 100.;
        GFX_Tools::CreateGLCross( vPos_, rCrossSize, 4.0, color_ );
    }

}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::DrawRectLimit
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::DrawRectLimit( bool bSelected )
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

    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->DrawRectLimit( bSelected );
    }

}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::StartMoveExtent
// Created: FBD 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBAutomateListViewItem::StartMoveExtent( const MT_Vector2D& vPosGL )
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
// Name: MOS_ODBAutomateListViewItem::MoveExtent
// Created: MOS_ODBListView 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBAutomateListViewItem::MoveExtent( const MT_Vector2D& vPosGL )
{
    if ( bEditLeft_ )
    {
        rectLimit_ = MT_Rect( vPosGL.rX_, vPosGL.rY_, rectLimit_.GetRight(), rectLimit_.GetBottom() );
        ClipLimit();
        SetInside( rectLimit_ );
        UpdateAll();
        if ( parent() )
            ((MOS_ODBArmeListViewItem*)parent())->FitParent( rectLimit_ );
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
            ((MOS_ODBArmeListViewItem*)parent())->FitParent( rectLimit_ );
        return true;
    }
    else
    if ( bEditCenter_ )
    {
        SetCenter( rectLimit_.GetCenter() - vPosGL );
        UpdateAll();
        if ( parent() )
            ((MOS_ODBArmeListViewItem*)parent())->FitParent( rectLimit_ );
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
// Name: MOS_ODBAutomateListViewItem::EndMoveExtent
// Created: FBD 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBAutomateListViewItem::EndMoveExtent()
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
// Name: MOS_ODBAutomateListViewItem::SetInside
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::SetInside( const MT_Rect& rectLimit )
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

    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->SetInside( rectLimit_ );
    }
}





//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::UpdateAll
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::UpdateAll()
{
    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->UpdateAll();
    }
    SetRandomPos( rectLimit_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::SetCenter
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::SetCenter( const MT_Vector2D& vDeltaPos )
{
    rectLimit_ = MT_Rect( rectLimit_.GetLeft() - vDeltaPos.rX_, rectLimit_.GetTop() - vDeltaPos.rY_, rectLimit_.GetRight() - vDeltaPos.rX_, rectLimit_.GetBottom() - vDeltaPos.rY_ );
    ClipLimit();

    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->SetCenter( vDeltaPos );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::FitParent
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::FitParent( const MT_Rect& rectLimit )
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
        ((MOS_ODBArmeListViewItem*)parent())->FitParent( rectLimit_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::SetLimit
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::ClipLimit()
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
// Name: MOS_ODBAutomateListViewItem::WriteAutomates
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::WriteAutomate( MT_OutputArchive_ABC& archive )
{
    archive.Section( "Agent" );

    // Read the agent category
    std::string strCategory;
    archive.WriteAttribute( "categorie", pTypeAutomate_->GetName() );

    archive.WriteField( "ID", nId_ );

    archive.WriteField( "Camp", GetNameTypeCamp( nTypeCamp_ ) );
    
    std::string strMGRS;
    MOS_App::GetApp().GetWorld().SimToMosMgrsCoord( vPos_, strMGRS );
    archive.WriteField( "Position", strMGRS );

    archive.WriteField( "Embraye", 0 );

    archive.EndSection(); // Agent

}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::WritePions
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
void MOS_ODBAutomateListViewItem::WritePions( MT_OutputArchive_ABC& archive )
{
    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        (*itPion)->WritePions( archive );
    }
}


//-----------------------------------------------------------------------------
// Name: std::string 
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
std::string MOS_ODBAutomateListViewItem::GetNameTypeCamp( E_TypeCamp nTypeCamp )
{
    std::string sName( "" );
    switch( nTypeCamp )
    {
        case    eTypeCampAmi:      sName = "Ami";       break;
        case    eTypeCampEnnemi:   sName = "Ennemi";    break;
        case    eTypeCampNeutre:   sName = "Neutre";    break;
        case    eTypeCampInconnu:  sName = "Inconnu";   break;
        default: assert( false );
    }
    return sName;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::GetNbAutomates
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBAutomateListViewItem::GetNbAutomates()
{
    uint nAutomates = 1;
    return nAutomates;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBAutomateListViewItem::GetNbPions
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBAutomateListViewItem::GetNbPions()
{
    uint nPions = 0;
    for ( IT_ODBPionPtrVector itPion = ODBPionPtrVector_.begin() ; itPion != ODBPionPtrVector_.end() ; ++itPion )
    {
        nPions += (*itPion)->GetNbPions();
    }
    return nPions;
}