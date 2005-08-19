//*****************************************************************************
// 
// $Created: FBD 02-12-17 $
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_TraceListView.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 13 $
// $Workfile: MOS_TraceListView.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_TraceListView.h"
#include "moc_MOS_TraceListView.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_TraceListView.inl"
#endif

#include "MOS_App.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_GLWidget.h"
#include "MOS_Mainwindow.h"

#include "MOS_ListViewItem_ABC.h"
#include "MOS_TraceListViewItem.h"
#include "MOS_TraceSubItem.h"

#include "MOS_LineManager.h"
#include "MOS_Trace.h"
#include "MOS_TraceEditor.h"
#include "MOS_GLMouse_ABC.h"
#include "MOS_AgentEditor.h"
#include "MOS_AgentListView.h"

#include "MOS_Agent.h"


using namespace DIN;


#include <QHeader.h>
#include <QSplitter.h>


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_TraceListView::MOS_TraceListView( QWidget* pParent, E_ListViewType nType )
    :   QListView       ( pParent )
    ,   MOS_GLMouse_ABC ()
    ,   pSelectedTrace_  ( 0 )
    ,   bDrawAll_       ( false )
    ,   nType_          ( nType )
    ,   bActive_        ( false )
    ,   bShift_         ( false )
    ,   bCtrl_          ( false )
    ,   pAgent_         ( 0 )
    ,   vOldPos_        ( 0., 0.)
    ,   sFileName_      ( "MOS_Trace.bin" )

{
    setSorting( -1 );
    addColumn( "Time" );
    addColumn( "Comptes Rendus" );
    connect( this, SIGNAL( pressed( QListViewItem * ) ), this, SLOT( SlotItemPressed( QListViewItem * ) ) );
    setRootIsDecorated( true );

}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView constructor
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
MOS_TraceListView::~MOS_TraceListView()
{
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::Initialize
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_TraceListView::Initialize()
{
    MOS_GLMouse_ABC::Initialize();
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::Terminate
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_TraceListView::Terminate()
{
    MOS_GLMouse_ABC::Terminate();
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::CreateTrace
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
QListViewItem* MOS_TraceListView::CreateTrace( MOS_Trace& Trace )
{
    if( nType_ == eListViewEditor )
    {
        MOS_TraceListViewItem* pTrace = 0;
        if( TraceVector_.size() )
            pTrace = new MOS_TraceListViewItem( eItemTypeEltTrace, Trace, *this, **(TraceVector_.rbegin()), "" );
        else
            pTrace = new MOS_TraceListViewItem( eItemTypeEltTrace, Trace, *this, "" );


        RegisterTrace( *pTrace );
        
        if( MOS_App::GetApp().GetMainWindow().GetAgentEditor().GetAgentListView().IsScrollTrace() )
            this->ensureItemVisible( this->lastItem() );
            
        return pTrace;
    }
    return 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::DeleteTrace
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_TraceListView::DeleteTrace( QListViewItem& Trace )
{
    UnregisterTrace( Trace );
    delete &Trace;
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::RegisterTrace
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_TraceListView::RegisterTrace( QListViewItem& Trace )
{
    TraceVector_.push_back( &Trace );
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::UnregisterTrace
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_TraceListView::UnregisterTrace( QListViewItem& Trace )
{
    IT_TracePtrVector itTrace = std::find( TraceVector_.begin(), TraceVector_.end(), &Trace );
    assert( itTrace != TraceVector_.end() );
    TraceVector_.erase( itTrace );
}


//=============================================================================
// SLOTS
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::LocOnTrace
// Created: FBD 02-12-19
//-----------------------------------------------------------------------------
void MOS_TraceListView::LocOnTrace( QListViewItem* pTrace, bool bSelectList )
{
    if( pTrace )
    {
        if( bSelectList )
            setSelected( pTrace, true );

        if( nType_ == eListViewEditor )
        {
            MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( ((MOS_TraceListViewItem*)pTrace)->GetTrace().GetPos() );
        }

    }
}
//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::SlotItemPressed
// Created: NLD 2002-07-18
//-----------------------------------------------------------------------------
void MOS_TraceListView::SlotItemPressed( QListViewItem* pItem )
{
    if( !pItem )
        return;

    E_ItemType nType = eItemTypeNone;
    MOS_ListViewItem_ABC* pTmpItem = (MOS_ListViewItem_ABC*)pItem;
    nType = pTmpItem->GetItemType();

    // Trace
    if( nType == eItemTypeEltTrace )
    {
        pSelectedTrace_ = pItem;
        LocOnTrace( pSelectedTrace_, true );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::Draw
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_TraceListView::Draw()
{
    if( bDrawAll_ )
    {
        for( CIT_TracePtrVector itTrace = TraceVector_.begin() ; itTrace != TraceVector_.end(); ++itTrace )
        {
            MOS_TraceListViewItem* pTrace = (MOS_TraceListViewItem*)( *itTrace );
            pTrace->Draw( (MOS_TraceListViewItem*)pTrace );
        }
    }
    else
    {
        for( CIT_TracePtrVector itTrace = TraceVector_.begin() ; itTrace != TraceVector_.end(); ++itTrace )
        {
            MOS_TraceListViewItem* pTrace = (MOS_TraceListViewItem*)( *itTrace );
            pTrace->Draw( (MOS_TraceListViewItem*)pSelectedTrace_ );
        }
    }
}
//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::LMBPressEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_TraceListView::LMBPressEvent( const MT_Vector2D& /*vPos*/ )
{
    return false;
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::RMBPressEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
bool MOS_TraceListView::RMBPressEvent( const MT_Vector2D& vPos )
{
    if( nType_ != eListViewEditor )
        return false;

    bool bReturn = false;

    for( CIT_TracePtrVector itTrace = TraceVector_.begin() ; itTrace != TraceVector_.end(); ++itTrace )
    {
        if( ((MOS_TraceListViewItem*)(*itTrace))->IsInside( vPos ) )
        {
            LocOnTrace( *itTrace, true );
            bReturn = true;
        }
    }
    return bReturn;
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::MouseMoveEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_TraceListView::MouseMoveEvent( const MT_Vector2D& /*vPos*/ )
{
    if( nType_ != eListViewEditor )
        return;

}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::LMBReleaseEvent
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
void MOS_TraceListView::LMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::RightMouseReleaseEvent
// Created: FBD 02-12-17
//-----------------------------------------------------------------------------
void MOS_TraceListView::RMBReleaseEvent( const MT_Vector2D& /*vPos*/ )
{
    if( nType_ != eListViewEditor )
        return;
}

//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::mousePressEvent
// Created: FBD 03-01-03
//-----------------------------------------------------------------------------
void MOS_TraceListView::mousePressEvent( QMouseEvent* /*pMouseEvent*/ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::showEvent
// Created: APE
//-----------------------------------------------------------------------------
void MOS_TraceListView::showEvent ( QShowEvent * )
{
    if( this->lastItem() )
        this->ensureItemVisible( this->lastItem() ); 
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::IsActive
// Created: FBD 03-01-10
//-----------------------------------------------------------------------------
bool MOS_TraceListView::IsActive( bool bMove )
{
    if( !bMove && bDrawAll_ )
    {
        return true;
    }

    MOS_TraceEditor* pParent = (MOS_TraceEditor*)parent();
    return pParent->IsActive( bMove );
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_TraceListView::keyPressEvent( QKeyEvent * e )
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
// Name: MOS_TraceListView::keyPressEvent
// Created: FBD 03-01-13
//-----------------------------------------------------------------------------
void MOS_TraceListView::keyReleaseEvent( QKeyEvent * e )
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
// Name: MOS_TraceListView::SetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
void MOS_TraceListView::SetAgent( MOS_Agent* pAgent )
{
    pAgent_ = pAgent;
    while( ! TraceVector_.empty() )
        DeleteTrace( *TraceVector_.front() );
    clear();
    if( pAgent_ != 0 )
    {
        // Fill the list
        MOS_Agent::T_TraceVector traveVect = pAgent->GetTraceVector();
        for( MOS_Agent::IT_TraceVector it = traveVect.begin(); it != traveVect.end(); ++it )
            CreateTrace( **it );
    }

    if( this->lastItem() )
        this->ensureItemVisible( this->lastItem() );
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::GetAgent
// Created: FBD 03-01-15
//-----------------------------------------------------------------------------
MOS_Agent* MOS_TraceListView::GetAgent()
{
    return pAgent_;
}


//-----------------------------------------------------------------------------
// Name: MOS_TraceListView::SlotSetShowAll
// Created: FBD 02-12-18
//-----------------------------------------------------------------------------
void MOS_TraceListView::SlotSetShowAll( bool bOn )
{
    bDrawAll_ = bOn;
}

