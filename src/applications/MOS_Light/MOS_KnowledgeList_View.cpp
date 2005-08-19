//*****************************************************************************
// 
// $Created: AGN 03-05-22 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_KnowledgeList_View.cpp $
// $Author: Age $
// $Modtime: 23/11/04 10:49 $
// $Revision: 11 $
// $Workfile: MOS_KnowledgeList_View.cpp $
// 
//*****************************************************************************

#include "MOS_Light_pch.h"

#include "MOS_KnowledgeList_View.h"
#include "moc_MOS_KnowledgeList_View.cpp"

#ifndef MOS_USE_INLINE
#   include "MOS_KnowledgeList_View.inl"
#endif

#include <QHeader.h>

#include "MOS_AgentKnowledge.h"
#include "MOS_KnowledgeList_ViewItem.h"
#include "MOS_AgentKnowledge_Editor.h"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"
#include "MOS_AgentManager.h"
#include "MOS_Agent.h"
#include "MOS_Gtia.h"

#include <qwidgetstack.h>

//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_View constructor
// Created: AGN 03-05-22
//-----------------------------------------------------------------------------
MOS_KnowledgeList_View::MOS_KnowledgeList_View( QWidget* pParent, const std::string& sName )
: QListView( pParent, sName.c_str() )
, pGtia_( 0 )
{
    addColumn( "Unités connues" );
    setMargin( 5 );
    setLineWidth( 2 );
    setFrameStyle( QFrame::Sunken | QFrame::Box );

    connect( this, SIGNAL( clicked( QListViewItem* ) ), this, SLOT( OnClick( QListViewItem* ) ) );
//    connect( this, SIGNAL( contextMenuRequested( QListViewItem *, const QPoint &, int ) ), this, SLOT( SlotContextMenuRequested( QListViewItem*, const QPoint & ) ) );
    connect( this, SIGNAL( doubleClicked( QListViewItem * ) ), this, SLOT( SlotDoubleClicked( QListViewItem * ) ) );

}


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_View::Initialize
// Created: AGN 03-05-23
//-----------------------------------------------------------------------------
void MOS_KnowledgeList_View::Initialize()
{
    pEditor_ = new MOS_AgentKnowledge_Editor( MOS_App::GetApp().GetMainWindow().GetWidgetStackRight() );
    pEditor_->Initialize();
    pEditor_->hide();
}


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_View destructor
// Created: AGN 03-05-22
//-----------------------------------------------------------------------------
MOS_KnowledgeList_View::~MOS_KnowledgeList_View()
{

}


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_View::AddKnowledge
// Created: AGN 03-05-22
//-----------------------------------------------------------------------------
void MOS_KnowledgeList_View::AddKnowledge( MOS_AgentKnowledge& knowledge )
{
    new MOS_KnowledgeList_ViewItem( knowledge, this, knowledge.GetName() );
}


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_View::RemoveKnowledge
// Created: AGN 03-05-22
//-----------------------------------------------------------------------------
void MOS_KnowledgeList_View::RemoveKnowledge( MOS_AgentKnowledge& knowledge )
{
    QListViewItem* pItem = this->firstChild();
    while( pItem != 0 )
    {
        MOS_KnowledgeList_ViewItem* pKnow = static_cast< MOS_KnowledgeList_ViewItem* >( pItem );
        if( & pKnow->GetRepresented() == & knowledge )
        {
            this->takeItem( pItem );
            break;
        }
        pItem = pItem->nextSibling();
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_View::OnClick
// Created: AGN 03-05-22
//-----------------------------------------------------------------------------
// slots
void MOS_KnowledgeList_View::OnClick( QListViewItem* pItem )
{

    MOS_KnowledgeList_ViewItem* pCastedItem = static_cast< MOS_KnowledgeList_ViewItem* >( pItem );
    if( pCastedItem == 0 )
        return;

    MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget().SetWantedCenter( pCastedItem->GetRepresented().GetPosition() );
}


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_View::SlotDoubleClicked
// Created: AGN 03-05-22
//-----------------------------------------------------------------------------
// slots
void MOS_KnowledgeList_View::SlotDoubleClicked( QListViewItem* pItem )
{
    /*if( pItem == 0 )
    {
        QPopupMenu* pMenu = new QPopupMenu( this );
        pMenu->insertItem( "Créer connaissance", 0 );
        int nSelected = pMenu->exec( point );
        if( nSelected != 0 )
            return;

        // Create a new knowledge
        MOS_AgentKnowledge* pNew = new MOS_AgentKnowledge( pAgent_->GetAgentID() );
        pAgent_->SetWaitingKnowledge( pNew );
        pEditor_->SetEnemyInfo( pNew, false );
        MOS_App::GetApp().GetMainWindow().PushWidgetStackRight( pEditor_ );
    }*/
//    if( pItem != 0 )
//    {
//        QPopupMenu* pMenu = new QPopupMenu( this );
//        pMenu->insertItem( "Effacer connaissance", 0 );
//
        MOS_AgentKnowledge& knowledge = static_cast< MOS_KnowledgeList_ViewItem* >( pItem )->GetRepresented();
//        if( agent.GetRealId() == (MIL_AgentID)-1 )
//            pMenu->insertItem( "Modifier connaissance", 1 );
//        else
//            pMenu->insertItem( "Editer connaissance", 1 );


//        int nSelected = pMenu->exec( point );

//        if( nSelected == 0 )
//            agent.SendDestructionRequest();
//        else
//        if( nSelected == 1 )
//        {
            pEditor_->SetEnemyInfo( & knowledge );
            MOS_App::GetApp().GetMainWindow().PushWidgetStackRight( pEditor_ );
//        }
//    }
}


//-----------------------------------------------------------------------------
// Name: MOS_KnowledgeList_View::SetGtia
// Created: AGN 03-05-27
//-----------------------------------------------------------------------------
void MOS_KnowledgeList_View::SetGtia( MOS_Gtia* pGtia )
{
    if( pGtia_ )
        pGtia_->SetAgentKnowledgeListView( 0 );

    pGtia_ = pGtia;

    clear();

    if( pGtia_ )
        pGtia_->SetAgentKnowledgeListView( this );
}

