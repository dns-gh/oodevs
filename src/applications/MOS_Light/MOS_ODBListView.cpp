//*****************************************************************************
//
// $Created: FBD 03-07-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_ODBListView.cpp $
// $Author: Age $
// $Modtime: 7/02/05 16:01 $
// $Revision: 7 $
// $Workfile: MOS_ODBListView.cpp $
//
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_ODBListView.h"
#include "moc_MOS_ODBListView.cpp"

#ifndef MOS_USE_INLINE
#	include "MOS_ODBListView.inl"
#endif

#include <QHeader.h>


#include "MOS_ODBCampListViewItem.h"
#include "MOS_AgentListView.h"
#include "MOS_AgentEditor.h"
#include "MOS_App.h"
#include "MOS_World.h"


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView constructor
// Created: FBD 03-07-24
//-----------------------------------------------------------------------------
MOS_ODBListView::MOS_ODBListView( QWidget* pParent )
:       QListView( pParent )
,       MOS_GLMouse_ABC ()
,       pTotalPion_( 0 )
,       bEditLeft_( false )
,       bEditRight_( false )
,       bEditCenter_( false )
,       colorRect_( 255,255,255, 0.3 )
,       bShift_( false )
,       bCtrl_( false )
,       nLastIdAutomate_( 0 )
,       nLastIdPion_    ( 0 )
{

    setSorting( -1 );
    header()->hide();
    addColumn( "" );
    setSelectionMode( QListView::Extended );
//    setMultiSelection( false );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );
    connect( this, SIGNAL( selectionChanged( QListViewItem * ) ), this, SLOT( SlotItemPressed( QListViewItem * ) ) );

    setRootIsDecorated( false );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView destructor
// Created: FBD 03-07-24
//-----------------------------------------------------------------------------
MOS_ODBListView::~MOS_ODBListView()
{
    
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_ODBListView::Initialize()
{
    pTotalPion_ = new MOS_ListViewItem_ABC( eItemTypeNone, *this );
    pTotalPion_->setRenameEnabled( 0, false );
    pTotalPion_->setOpen( true );
    SetRandomPos( MOS_App::GetApp().GetWorld().GetRect() );

    CreateCamp( eTypeCampAmi );
    CreateCamp( eTypeCampEnnemi );
    SetName();

    
    MOS_GLMouse_ABC::Initialize();

}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_ODBListView::Terminate()
{
    MOS_GLMouse_ABC::Terminate();
    DeleteCamp( eTypeCampAmi );
    DeleteCamp( eTypeCampEnnemi );
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::CreateCamp
// Created: FBD 03-07-24
//-----------------------------------------------------------------------------
MOS_ODBCampListViewItem* MOS_ODBListView::CreateCamp( E_TypeCamp nTypeCamp )
{
    assert( pTotalPion_ );
    MOS_ODBCampListViewItem* pCamp = new MOS_ODBCampListViewItem( *pTotalPion_, nTypeCamp, rectLimit_ );
    campPtrMap_.insert( std::make_pair( nTypeCamp, pCamp ) );
    pCamp->Initialize();
    return pCamp;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::DeleteArme
// Created: FBD 03-07-24
//-----------------------------------------------------------------------------
void MOS_ODBListView::DeleteCamp( E_TypeCamp nType ){
    IT_ODBCampPtrMap itCamp = campPtrMap_.find( nType );
    assert( itCamp != campPtrMap_.end() );
    delete itCamp->second;
     campPtrMap_.erase( itCamp );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_ODBListView::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    bool bReturn = false;
    return bReturn;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_ODBListView::RMBPressEvent( const MT_Vector2D& vPos )
{
    return StartMoveExtent( vPos );
}



//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_ODBListView::MouseMoveEvent( const MT_Vector2D& vPos )
{
    MoveExtent( vPos );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_ODBListView::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_ODBListView::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    EndMoveExtent();

}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_ODBListView::IsActive( bool bMove )
{
    MOS_AgentEditor* pParent = (MOS_AgentEditor*)parent();
    return pParent->IsActive( bMove );
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::Draw
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_ODBListView::Draw( )
{
    bool bSelected = pTotalPion_->isSelected();

    for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
    {
        itCamp->second->Draw( bSelected );
    }
    
    if ( bSelected )
    {
        GFX_Color color_( 255,255,255 );
        MT_Float rCrossSize = 100 ;
        GFX_Tools::CreateGLCross( vPos_, rCrossSize, 4.0, color_ );
    }

    DrawRectLimit( false );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::DrawRectLimit
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBListView::DrawRectLimit( bool bSelected )
{
   bSelected |= pTotalPion_->isSelected();

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

    for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
    {
        itCamp->second->DrawRectLimit( bSelected );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_ODBListView::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
{
    // PopupMenu
    QPopupMenu* pMenu = new QPopupMenu( this );

    if ( !pItem )
    {

    }
    else
    if( pItem )
    {
        MOS_ListViewItem_ABC* pItemType = (MOS_ListViewItem_ABC*)pItem;
        if ( pItemType->GetItemType() == eItemTypeODBArme )
        {
            MOS_ODBArmeListViewItem* pArme = (MOS_ODBArmeListViewItem*)pItem;
            pArme->CreateMenu( pMenu );
            int nSelected = pMenu->exec( point ) ;
            if( nSelected != -1)
            {
                pArme->ExecuteMenu( nSelected );
            }
        }
        else
        if ( pItemType->GetItemType() == eItemTypeODBAutomate )            
        {
            MOS_ODBAutomateListViewItem* pAutomate = (MOS_ODBAutomateListViewItem*)pItem;
            pAutomate->CreateMenu( pMenu );
            int nSelected = pMenu->exec( point ) ;
            if( nSelected != -1)
            {
                pAutomate->ExecuteMenu( nSelected );
            }
        }
        else
        if ( pItemType->GetItemType() == eItemTypeODBPion )
        {
            MOS_ODBPionListViewItem* pPion = (MOS_ODBPionListViewItem*)pItem;
            pPion->CreateMenu( pMenu );
            int nSelected = pMenu->exec( point ) ;
            if( nSelected != -1)
            {
                pPion->ExecuteMenu( nSelected );
            }
        }
        SetName();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentListView::SlotItemPressed
// Created: NLD 2002-07-18
//-----------------------------------------------------------------------------
void MOS_ODBListView::SlotItemPressed( QListViewItem* pItem )
{
    if( !pItem )
    {
         return;
    }

    E_ItemType nType = eItemTypeNone;
    MOS_ListViewItem_ABC* pTmpItem = (MOS_ListViewItem_ABC*)pItem;
    nType = pTmpItem->GetItemType();

    // Agent
    if( nType == eItemTypeODBAutomate || nType == eItemTypeODBPion )
    {
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::SetName
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBListView::SetName()
{
    std::string sName( "ODB" );
    sName += MT_FormatString( " - (%04d Pions)", GetNbSons() );
    pTotalPion_->setText( 0, sName.c_str() );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::GetNbSons
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBListView::GetNbSons()
{
    uint nPions = 0;
    for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
    {
        nPions += itCamp->second->GetNbSons();
        itCamp->second->SetName();
    }
    return nPions;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::GetNbAutomates
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBListView::GetNbAutomates()
{
    uint nAutomates = 0;
    for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
    {
        nAutomates += itCamp->second->GetNbAutomates();
    }
    return nAutomates;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::GetNbPions
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
uint MOS_ODBListView::GetNbPions()
{
    uint nPions = 0;
    for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
    {
        nPions += itCamp->second->GetNbPions();
    }
    return nPions;
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::SetRandomPos
// Created: FBD 03-07-28
//-----------------------------------------------------------------------------
void MOS_ODBListView::SetRandomPos( const MT_Rect& rect )
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
// Name: MOS_ODBListView::StartMoveExtent
// Created: FBD 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBListView::StartMoveExtent( const MT_Vector2D& vPosGL )
{
    if ( pTotalPion_->isSelected() )
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
        for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
        {
           itCamp->second->StartMoveExtent( vPosGL );
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: GEN_GLExtent::MoveExtent
// Created: MOS_ODBListView 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBListView::MoveExtent( const MT_Vector2D& vPosGL )
{
    if ( bEditLeft_ )
    {
        rectLimit_ = MT_Rect( vPosGL.rX_, vPosGL.rY_, rectLimit_.GetRight(), rectLimit_.GetBottom() );
        ClipLimit();
        SetInside( rectLimit_ );
        UpdateAll();
        return true;
    }
    else
    if( bEditRight_ )
    {
        rectLimit_ = MT_Rect( rectLimit_.GetLeft(), rectLimit_.GetTop(), vPosGL.rX_, vPosGL.rY_ );
        ClipLimit();
        SetInside( rectLimit_ );
        UpdateAll();
        return true;
    }
    else
    if ( bEditCenter_ )
    {
        SetCenter( rectLimit_.GetCenter() - vPosGL );
        UpdateAll();
        return true;
    }
    else
    {
        for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
        {
            if ( itCamp->second->MoveExtent( vPosGL ) )
                return true;
        }
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::EndMoveExtent
// Created: FBD 02-12-04
//-----------------------------------------------------------------------------
bool MOS_ODBListView::EndMoveExtent()
{
    if ( bEditLeft_ || bEditRight_ || bEditCenter_ )
    {
        bEditLeft_ = false;
        bEditRight_ = false;
        bEditCenter_ = false;
    }
    for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
    {
        itCamp->second->EndMoveExtent();
    }
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::SetInside
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBListView::SetInside( const MT_Rect& rectLimit )
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

    for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
    {
        itCamp->second->SetInside( rectLimit_ );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::SetCenter
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBListView::SetCenter( const MT_Vector2D& vDeltaPos )
{
    rectLimit_ = MT_Rect( rectLimit_.GetLeft() - vDeltaPos.rX_, rectLimit_.GetTop() - vDeltaPos.rY_, rectLimit_.GetRight() - vDeltaPos.rX_, rectLimit_.GetBottom() - vDeltaPos.rY_ );

    ClipLimit();

    for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
    {
        itCamp->second->SetCenter( vDeltaPos );
    }
   
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::UpdateAll
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBListView::UpdateAll()
{
    for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
    {
        itCamp->second->UpdateAll();
    }
    SetRandomPos( rectLimit_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::FitParent
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBListView::FitParent( const MT_Rect& rectLimit )
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

}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::SetLimit
// Created: FBD 03-07-29
//-----------------------------------------------------------------------------
void MOS_ODBListView::ClipLimit()
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
// Name: MOS_ODBListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_ODBListView::keyPressEvent( QKeyEvent * e )
{
   if( e->key() == Qt::Key_Shift )
   {
//        setMultiSelection( true );
        bShift_ = true;
   }
   else
   if( e->key() == Qt::Key_Control )
   {
//        setMultiSelection( true );
        bCtrl_ = true;
   }
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::keyReleaseEvent
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
void MOS_ODBListView::keyReleaseEvent( QKeyEvent * e )
{
   if( e->key() == Qt::Key_Shift )
   {
//        selectAll( false );
//        setMultiSelection( false );
        bShift_ = false;
   }
   else
   if( e->key() == Qt::Key_Control )
   {
//        selectAll( false );
//        setMultiSelection( false );
        bCtrl_ = false;
   }
}






//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::SlotWriteODB
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
void MOS_ODBListView::SlotWriteODB()
{
    MT_XXmlOutputArchive archive;
    archive.Section( "Configuration" );

    archive.Section( "Agents" );

            archive.BeginList( "Automates", GetNbAutomates() );
            for ( IT_ODBCampPtrMap itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
            {
                itCamp->second->WriteAutomates( archive );
            }
            archive.EndList(); // Automates

            archive.BeginList( "Pions", GetNbPions() );
            for ( itCamp = campPtrMap_.begin() ; itCamp != campPtrMap_.end() ; ++itCamp )
            {
                itCamp->second->WritePions( archive );
            }
            archive.EndList(); // Pions

    archive.EndSection(); // Agents   

    archive.EndSection(); // Configuration   

    archive.WriteToFile( "ODB_MosLight.xml" );

}

//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::GetIdAutomate
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
uint MOS_ODBListView::GetIdAutomate()
{
    uint nId = nLastIdAutomate_;
    ++nLastIdAutomate_;
    return nId;
}


//-----------------------------------------------------------------------------
// Name: MOS_ODBListView::GetIdPion
// Created: FBD 03-07-30
//-----------------------------------------------------------------------------
uint MOS_ODBListView::GetIdPion()
{
    uint nId = nLastIdPion_;
    ++nLastIdPion_;
    return nId;
}

