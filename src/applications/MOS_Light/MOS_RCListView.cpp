//*****************************************************************************
// 
// $Created: FBD 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_RCListView.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 22 $
// $Workfile: MOS_RCListView.cpp $
// 
//*****************************************************************************

#include "MOS_Light_Pch.h"

#include "MOS_RCListView.h"
#include "moc_MOS_RCListView.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_RCListView.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_Mainwindow.h"

#include "MOS_ListViewItem_ABC.h"
#include "MOS_RCListViewItem.h"
#include "MOS_RCSubItem.h"

#include "MOS_LineManager.h"
#include "MOS_RC.h"
#include "MOS_RCEditor.h"
#include "MOS_GLMouse_ABC.h"
#include "MOS_AgentEditor.h"
#include "MOS_AgentListView.h"

#include "MOS_Agent.h"

#include "MOS_ASN_Messages.h"

using namespace DIN;


#include <QHeader.h>
#include <QSplitter.h>


//-----------------------------------------------------------------------------
// Name: MOS_RCListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_RCListView::MOS_RCListView( QWidget* pParent, E_ListViewType nType )
    :   QListView       ( pParent )
    ,   MOS_GLMouse_ABC ()
    ,   pSelectedRC_  ( 0 )
    ,   bDrawAll_       ( false )
    ,   nType_          ( nType )
    ,   bActive_        ( false )
    ,   bShift_         ( false )
    ,   bCtrl_          ( false )
    ,   pAgent_         ( 0 )
    ,   vOldPos_        ( 0., 0.)
    ,   sFileName_      ( "MOS_RC.bin" )

{
    setSorting( -1 );
    addColumn( "Time" );
    addColumn( "Comptes Rendus" );
    connect( this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );
    connect( this, SIGNAL( pressed( QListViewItem * ) ), this, SLOT( SlotItemPressed( QListViewItem * ) ) );
    setRootIsDecorated( true );

}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_RCListView::~MOS_RCListView()
{
}



//-----------------------------------------------------------------------------
// Name: MOS_RCListView::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RCListView::Initialize()
{
    MOS_GLMouse_ABC::Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RCListView::Terminate()
{
    MOS_GLMouse_ABC::Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::CreateRC
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
QListViewItem* MOS_RCListView::CreateRC( MOS_RC& rc )
{
    if( nType_ == eListViewEditor )
    {
        MOS_RCListViewItem* pRC = 0;
        if( RCVector_.size() )
            pRC = new MOS_RCListViewItem( rc, *this, **(RCVector_.rbegin()), "" );
        else
            pRC = new MOS_RCListViewItem( rc, *this, "" );

        RegisterRC( *pRC );
        this->ensureItemVisible( this->lastItem() );
        return pRC;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::DeleteRC
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_RCListView::DeleteRC( QListViewItem& RC )
{
    UnregisterRC( RC );
    delete &RC;
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::RegisterRC
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_RCListView::RegisterRC( QListViewItem& RC )
{
    RCVector_.push_back( &RC );
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListView::UnregisterRC
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_RCListView::UnregisterRC( QListViewItem& RC )
{
    IT_RCPtrVector itRC = std::find( RCVector_.begin(), RCVector_.end(), &RC );
    assert( itRC != RCVector_.end() );
    RCVector_.erase( itRC );
}



//=============================================================================
// SLOTS
//=============================================================================
//-----------------------------------------------------------------------------
// Name: MOS_RCListView::SlotContextMenuRequested
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_RCListView::SlotContextMenuRequested( QListViewItem* pItem, const QPoint& point )
{
    if( nType_ == eListViewEditor )
    {
        // PopupMenu
        QPopupMenu* pMenu = new QPopupMenu( this );

        if( !pItem )
            return;
        else
        {
            switch( ((MOS_ListViewItem_ABC*)pItem)->GetItemType() )
            {
                case eItemTypeEltRC:
                {
                }
                break;

                case eItemTypeEltOrderConduite:
                {
                     pMenu->insertItem( QString( "Select ordre de conduite" ), 1 );
                }
                break;

            }
        }

        int nSelected = pMenu->exec( point ) ;
        if( nSelected == -1)
            return;

        if( nSelected == 1 )
        {
            MOS_RCSubItem* pSubItem = (MOS_RCSubItem*)pItem;
            MOS_RCListViewItem* pRCItem = (MOS_RCListViewItem*)pSubItem->parent();

            MOS_ASN_MsgOrderConduite asnMsg;
            asnMsg.GetAsnMsg().unit_id  = pRCItem->GetRC().GetAgent().GetAgentID();
            asnMsg.GetAsnMsg().order_id = 42;
            asnMsg.GetAsnMsg().order_conduite.t = pSubItem->GetValue() + 1; //$$$ DEGEU pour le moment
            asnMsg.Send( 65 );
        }

    }
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::LocOnRC
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_RCListView::LocOnRC( QListViewItem* pRC, bool bSelectList )
{
    if( pRC )
    {
        if( bSelectList )
            setSelected( pRC, true );

        if( nType_ == eListViewEditor )
        {
            MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( ((MOS_RCListViewItem*)pRC)->GetRC().GetPos() );
        }

    }
}
//-----------------------------------------------------------------------------
// Name: MOS_RCListView::SlotItemPressed
// Created: NLD 2002-07-18
//-----------------------------------------------------------------------------
void MOS_RCListView::SlotItemPressed( QListViewItem* pItem )
{
    if( !pItem )
        return;

    E_ItemType nType = eItemTypeNone;
    MOS_ListViewItem_ABC* pTmpItem = (MOS_ListViewItem_ABC*)pItem;
    nType = pTmpItem->GetItemType();

    // RC
    if( nType == eItemTypeEltRC )
    {
        pSelectedRC_ = pItem;
        LocOnRC( pSelectedRC_, true );
    }
    else
    if( nType == eItemTypeEltOrderMission )
    {
        pSelectedRC_ = pItem->parent();
        LocOnRC( pSelectedRC_, true );
    }
    else
    if( nType == eItemTypeEltOrderConduite )
    {
        pSelectedRC_ = pItem->parent();
        LocOnRC( pSelectedRC_, true );
    }

}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RCListView::Draw()
{
    if( bDrawAll_ )
    {
        for( CIT_RCPtrVector itRC = RCVector_.begin() ; itRC != RCVector_.end(); ++itRC )
        {
            MOS_RCListViewItem* pRC = (MOS_RCListViewItem*)( *itRC );
            pRC->Draw( (MOS_RCListViewItem*)pRC );
        }
    }
    else
    {
        for( CIT_RCPtrVector itRC = RCVector_.begin() ; itRC != RCVector_.end(); ++itRC )
        {
            MOS_RCListViewItem* pRC = (MOS_RCListViewItem*)( *itRC );
            pRC->Draw( (MOS_RCListViewItem*)pSelectedRC_ );
        }
    }

}
//-----------------------------------------------------------------------------
// Name: MOS_RCListView::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_RCListView::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_RCListView::RMBPressEvent( const MT_Vector2D& vPos )
{
    if( nType_ != eListViewEditor )
        return false;

    bool bReturn = false;

    for( CIT_RCPtrVector itRC = RCVector_.begin() ; itRC != RCVector_.end(); ++itRC )
    {
        if( ((MOS_RCListViewItem*)(*itRC))->IsInside( vPos ) )
        {
            LocOnRC( *itRC, true );
            bReturn = true;
        }
    }
    return bReturn;
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListView::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RCListView::MouseMoveEvent( const MT_Vector2D& /*vPos*/ )
{
    if( nType_ != eListViewEditor )
        return;

}


//-----------------------------------------------------------------------------
// Name: MOS_RCListView::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_RCListView::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListView::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_RCListView::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if( nType_ != eListViewEditor )
        return;
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::mousePressEvent
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_RCListView::mousePressEvent( QMouseEvent* /*pMouseEvent*/ )
{
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_RCListView::IsActive( bool bMove )
{
    if( !bMove && bDrawAll_ )
    {
        return true;
    }


    MOS_RCEditor* pParent = (MOS_RCEditor*)parent();
    return pParent->IsActive( bMove );
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_RCListView::keyPressEvent( QKeyEvent * e )
{
   if( e->key() == Qt::Key_Shift )
   {
        bShift_ = true;
   }
   else
   if( e->key() == Qt::Key_Control )
   {
        bCtrl_ = true;
   }
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_RCListView::keyReleaseEvent( QKeyEvent * e )
{
   if( e->key() == Qt::Key_Shift )
   {
        bShift_ = false;
   }
   else
   if( e->key() == Qt::Key_Control )
   {
        bCtrl_ = false;
   }
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListView::showEvent
// Created: APE
//-----------------------------------------------------------------------------
void MOS_RCListView::showEvent ( QShowEvent * )
{
    if( this->lastItem() )
        this->ensureItemVisible( this->lastItem() ); 
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListView::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_RCListView::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
    // Clear the list
    while( ! RCVector_.empty() )
        DeleteRC( *RCVector_.front() );

    clear();
    if( pAgent != 0 )
    {
        // Fill the list
        MOS_Agent::T_RCVector& rcVector = pAgent->GetRCVector();
        for( MOS_Agent::IT_RCVector it = rcVector.begin(); it != rcVector.end(); ++it )
            CreateRC( **it );

        if( MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().IsScrollTrace() )
            this->ensureItemVisible( this->lastItem() );
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_RCListView::GetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
MOS_Agent* MOS_RCListView::GetAgent()
{
    return pAgent_;
}

//-----------------------------------------------------------------------------
// Name: MOS_RCListView::SlotSetShowAll
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_RCListView::SlotSetShowAll( bool bOn )
{
    bDrawAll_ = bOn;
}


