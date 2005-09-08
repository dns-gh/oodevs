// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-15 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_AgentListView.cpp $
// $Author: Ape $
// $Modtime: 23/11/04 18:42 $
// $Revision: 14 $
// $Workfile: MOS_AgentListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_AgentListView.h"
#include "moc_MOS_AgentListView.cpp"

#include "MOS_Agent.h"
#include "MOS_AgentManager.h"
#include "MOS_Team.h"
#include "MOS_Gtia.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_Options.h"
#include "MOS_ActionContext.h"
#include "MOS_AgentKnowledge.h"
#include "MOS_ObjectKnowledge.h"
#include "MOS_ASN_Messages.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentListView.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView constructor
/** @param  pParent 
*/
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
MOS_AgentListView::MOS_AgentListView( QWidget* pParent )
    : QListView     ( pParent )
    , pPopupMenu_   ( 0 )
{
    this->setMinimumSize( 1, 1 );
    this->addColumn( "Unités" );
    this->setRootIsDecorated( true );
    this->setResizeMode( QListView::LastColumn );
    this->setAcceptDrops( true );

    connect( &MOS_App::GetApp(), SIGNAL( AgentCreated( MOS_Agent& ) ), this, SLOT( AddAgent( MOS_Agent& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( AgentUpdated( MOS_Agent& ) ), this, SLOT( OnAgentUpdated( MOS_Agent& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( AgentReparented( MOS_Agent& ) ), this, SLOT( OnAgentReparented( MOS_Agent& ) ) );
    connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( clear() ) );

    connect( &MOS_App::GetApp(), SIGNAL( GtiaCreated( MOS_Gtia& ) ), this, SLOT( AddGtia( MOS_Gtia& ) ) );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );

    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),                   &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ),               &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );

    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                           this,   SLOT( OnTeamChanged() ) );

    if( MOS_App::GetApp().IsODBEdition() )
    {
        // Refresh the display
        OnTeamChanged();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
MOS_AgentListView::~MOS_AgentListView()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::AddAgent
/** @param  agent 
*/
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
void MOS_AgentListView::AddAgent( MOS_Agent& agent )
{
    int nPlayedTeam = MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;
    if( nPlayedTeam != MOS_Options::eController && nPlayedTeam != (int)agent.GetTeam().GetIdx() )
        return;

    // Check if the agent is not already inserted.
#ifdef MOS_DEBUG
    QListViewItemIterator it( this->firstChild() );
    while( it.current() )
    {
        if( this->ToAgent( it.current() ) == &agent )
        {
            assert( false );
            return;
        }
        ++it;
    }
#endif // MOS_DEBUG

    // Team
    MOS_Team& team = agent.GetTeam();
    QListViewItem* pTeamItem = this->firstChild();
    while( pTeamItem != 0 )
    {
        MT_ValuedListViewItem< MOS_Team*, eTeam >* pCastItem = (MT_ValuedListViewItem< MOS_Team*, (int)eTeam >*)pTeamItem;
        if( pCastItem->GetValue() == &team )
            break;
        pTeamItem = pTeamItem->nextSibling();
    }

    if( pTeamItem == 0 )
    {
        pTeamItem = new MT_ValuedListViewItem< MOS_Team*, eTeam >( &team, this, team.GetName().c_str() );
        pTeamItem->setDropEnabled( false );
        pTeamItem->setDragEnabled( false );
    }

    // GTIA
    MOS_Gtia& gtia = agent.GetGtia();
    QListViewItem* pGtiaItem = pTeamItem->firstChild();
    while( pGtiaItem != 0 )
    {
        MT_ValuedListViewItem< MOS_Gtia*, eGtia >* pCastItem = (MT_ValuedListViewItem< MOS_Gtia*, (int)eGtia >*)pGtiaItem;
        if( pCastItem->GetValue() == &gtia )
            break;
        pGtiaItem = pGtiaItem->nextSibling();
    }

    if( pGtiaItem == 0 )
    {
        pGtiaItem = new MT_ValuedListViewItem< MOS_Gtia*, eGtia >( &gtia, pTeamItem, tr( "Gtia " ) + QString::number( gtia.GetID() ) );
        pGtiaItem->setDropEnabled( true );
        pGtiaItem->setDragEnabled( false );
    }
    // Group
    if( agent.GetParent() == 0 )
    {
        QListViewItem* pNewItem = new MT_ValuedListViewItem< MOS_Agent*, eAgent>( &agent, pGtiaItem, agent.GetName().c_str() );
        pNewItem->setDropEnabled( ! MOS_App::GetApp().IsODBEdition() );
        pNewItem->setDragEnabled( ! MOS_App::GetApp().IsODBEdition() );
        if( agent.IsEmbraye() )
            pNewItem->setPixmap( 0, MAKE_PIXMAP( embraye ) );
        else
            pNewItem->setPixmap( 0, MAKE_PIXMAP( debraye ) );

        // When editing an ODB, we want to see groups as soon as they are created. 
        if( MOS_App::GetApp().IsODBEdition() )
            this->ensureItemVisible( pNewItem );
    }
    else
    {
        QListViewItem* pParent = pGtiaItem->firstChild();
        while( pParent != 0 )
        {
            if( this->ToAgent( pParent ) == agent.GetParent() )
            {
                QListViewItem* pNewItem = new MT_ValuedListViewItem< MOS_Agent*, eAgent>( &agent, pParent, agent.GetName().c_str() );
                pNewItem->setDropEnabled( false );
                pNewItem->setDragEnabled( ! MOS_App::GetApp().IsODBEdition() );
                return;
            }
            pParent = pParent->nextSibling();
        }

        // Could not find the parent...
        assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::AddGtia
/** @param  gtia 
*/
// Created: APE 2004-08-25
// -----------------------------------------------------------------------------
void MOS_AgentListView::AddGtia( MOS_Gtia& gtia )
{
    int nPlayedTeam = MOS_MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;
    if( nPlayedTeam != MOS_Options::eController && nPlayedTeam != (int)gtia.GetTeam().GetIdx() )
        return;

    // Check if the gtia is not already inserted.
#ifdef MOS_DEBUG
    QListViewItemIterator it( this->firstChild() );
    while( it.current() )
    {
        if( this->ToGtia( it.current() ) == &gtia )
        {
            assert( false );
            return;
        }
        ++it;
    }
#endif // MOS_DEBUG

    // Team
    const MOS_Team& team = gtia.GetTeam();
    QListViewItem* pTeamItem = this->firstChild();
    while( pTeamItem != 0 )
    {
        MT_ValuedListViewItem< const MOS_Team*, eTeam >* pCastItem = (MT_ValuedListViewItem< const MOS_Team*, (int)eTeam >*)pTeamItem;
        if( pCastItem->GetValue() == &team )
            break;
        pTeamItem = pTeamItem->nextSibling();
    }

    if( pTeamItem == 0 )
    {
        pTeamItem = new MT_ValuedListViewItem< const MOS_Team*, eTeam >( &team, this, team.GetName().c_str() );
        pTeamItem->setDropEnabled( false );
        pTeamItem->setDragEnabled( false );
    }

    // GTIA
    QListViewItem* pGtiaItem = new MT_ValuedListViewItem< MOS_Gtia*, eGtia >( &gtia, pTeamItem, tr( "Gtia " ) + QString::number( gtia.GetID() ) );
    pGtiaItem->setDropEnabled( false );
    pGtiaItem->setDragEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::SetSelectedElement
/** @param  pAgent 
*/
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
void MOS_AgentListView::SetSelectedElement( MOS_SelectedElement& selectedElement )
{
    //$$$$ Pas terrible, à améliorer.
    if(    selectedElement.pAgent_ == 0 
        && selectedElement.pTeam_ == 0 
        && selectedElement.pGtia_ == 0
        && selectedElement.pAgentKnowledge_ == 0
        && selectedElement.pObjectKnowledge_ == 0 )
    {
        this->selectAll( false );
        return;
    }

    // Check if it's not already selected.
    if(    ( selectedElement.pAgent_ != 0               && this->ToAgent( this->selectedItem() ) == selectedElement.pAgent_ )
        || ( selectedElement.pGtia_ != 0                && this->ToGtia( this->selectedItem() ) == selectedElement.pGtia_ )
        || ( selectedElement.pTeam_ != 0                && this->ToTeam( this->selectedItem() ) == selectedElement.pTeam_ )
        || ( selectedElement.pAgentKnowledge_ != 0      && this->ToGtia( this->selectedItem() ) == &(selectedElement.pAgentKnowledge_->GetOwner()) )
        || ( selectedElement.pObjectKnowledge_ != 0 && this->ToTeam( this->selectedItem() ) == &(selectedElement.pObjectKnowledge_->GetOwner()) ) )
        return;

    QListViewItemIterator it( this->firstChild() );
    while( it.current() )
    {
        if(    ( selectedElement.pAgent_ != 0               && this->ToAgent( it.current() ) == selectedElement.pAgent_ )
            || ( selectedElement.pGtia_ != 0                && this->ToGtia( it.current() ) == selectedElement.pGtia_ )
            || ( selectedElement.pTeam_ != 0                && this->ToTeam( it.current() ) == selectedElement.pTeam_ )
            || ( selectedElement.pAgentKnowledge_ != 0      && this->ToGtia( it.current() ) == &(selectedElement.pAgentKnowledge_->GetOwner()) )
            || ( selectedElement.pObjectKnowledge_ != 0 && this->ToTeam( it.current() ) == &(selectedElement.pObjectKnowledge_->GetOwner()) ) )
        {
            disconnect( this, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
            this->setSelected( it.current(), true );
            connect( this, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
            if( MOS_MainWindow::GetMainWindow().GetOptions().bOpenTreeToItem_ )
                this->ensureItemVisible( it.current() );
            return;
        }
        ++it;
    }

    // Could not find the agent to select.
    assert( false );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::OnRequestPopup
/** @param  pItem 
    @param  pos 
    @param  nCol 
*/
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
void MOS_AgentListView::OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int /*nCol*/ )
{
    delete pPopupMenu_;
    pPopupMenu_ = new QPopupMenu( this );

    MOS_SelectedElement selectedElement;
    if( this->ToAgent( pItem ) != 0 )
        selectedElement.pAgent_ = this->ToAgent( pItem );
    if( this->ToGtia( pItem ) != 0 )
        selectedElement.pGtia_ = this->ToGtia( pItem );
    if( this->ToTeam( pItem ) != 0 )
        selectedElement.pTeam_ = this->ToTeam( pItem );

    // Give a chance to the rest of the objets to fill up the popup menu if necessary.
    emit NewPopupMenu( *pPopupMenu_, MOS_ActionContext( selectedElement, 0 ) );

    // Only popup the menu if some items have been added to it.
    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::OnRequestCenter
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
void MOS_AgentListView::OnRequestCenter()
{
    MOS_Agent* pAgent = this->ToAgent( this->selectedItem() );
    if( pAgent != 0 )
        emit CenterOnPoint( pAgent->GetPos() );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::OnSelectionChange
/** @param  pItem 
*/
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
void MOS_AgentListView::OnSelectionChange( QListViewItem* pItem )
{
    if( pItem == 0 )
        return;

    MOS_Agent* pAgent = this->ToAgent( pItem );
    MOS_SelectedElement selectedElement;
    if( pAgent != 0 )
        selectedElement = MOS_SelectedElement( *pAgent );

    MOS_Gtia* pGtia = this->ToGtia( pItem );
    if( pGtia != 0 )
        selectedElement = MOS_SelectedElement( *pGtia );

    MOS_Team* pTeam = this->ToTeam( pItem );
    if( pTeam != 0 )
        selectedElement = MOS_SelectedElement( *pTeam );

    // Disconnect and connect to avoid having the signal come back to us.
    disconnect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    emit ElementSelected( selectedElement );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::OnTeamChanged
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
void MOS_AgentListView::OnTeamChanged()
{
    this->clear();

    // In ODB mode, show all teams & gtias
    if( MOS_App::GetApp().IsODBEdition() )
    {
        const MOS_AgentManager::T_TeamMap& teams = MOS_App::GetApp().GetAgentManager().GetTeams();
        for( MOS_AgentManager::CIT_TeamMap it = teams.begin(); it != teams.end(); ++it )
        {
            MOS_Team& team = *((*it).second);
            QListViewItem* pTeamItem = new MT_ValuedListViewItem< MOS_Team*, eTeam >( &team, this, team.GetName().c_str() );
            pTeamItem->setDropEnabled( false );
            pTeamItem->setDragEnabled( false );

            const MOS_Team::T_GtiaMap& gtias = team.GetGtias();
            for( MOS_Team::CIT_GtiaMap it2 = gtias.begin(); it2 != gtias.end(); ++it2 )
                this->AddGtia( *((*it2).second) );
        }
    }

    MOS_AgentManager::CT_AgentMap agentMap = MOS_App::GetApp().GetAgentManager().GetAgentList();
    // Add the automates first.
    for( MOS_AgentManager::CIT_AgentMap it = agentMap.begin(); it != agentMap.end(); ++it )
        if( (it->second)->GetParent() == 0 )
            this->AddAgent( *(it->second) );
    // Then the units. This is done to avoid parenting problems in the listview.
    for( it = agentMap.begin(); it != agentMap.end(); ++it )
        if( (it->second)->GetParent() != 0 )
            this->AddAgent( *(it->second) );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::OnAgentUpdated
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
void MOS_AgentListView::OnAgentUpdated( MOS_Agent& agent )
{
    if( ! agent.IsAutomate() )
        return;

    QListViewItemIterator it( this->firstChild() );
    while( it.current() )
    {
        if( this->ToAgent( it.current() ) == &agent )
        {
            it.current()->setPixmap( 0, agent.IsEmbraye() ? MAKE_PIXMAP( embraye ) : MAKE_PIXMAP( debraye ) );
            break;
        }
        ++it;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::OnAgentReparented
/** @param  agent 
*/
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
void MOS_AgentListView::OnAgentReparented( MOS_Agent& agent )
{
    QListViewItemIterator it( this->firstChild() );
    while( it.current() )
    {
        if( this->ToAgent( it.current() ) == &agent )
        {
            delete it.current();
            this->AddAgent( agent );
            MOS_Agent::T_AgentVector children = agent.GetChildren();
            for( MOS_Agent::IT_AgentVector it = children.begin(); it != children.end(); ++it )
                this->AddAgent( **it );
            return;
        }
        ++it;
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::dragObject
/** @return 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
QDragObject* MOS_AgentListView::dragObject()
{
    QListViewItem* pItem = selectedItem();
    assert( pItem != 0 );

    QByteArray* pBytes = new QByteArray();
    pBytes->setRawData( (const char*)&pItem, sizeof( QListViewItem* ) );

    QStoredDrag* data = new QStoredDrag( "pointer/pion", this );
    data->setEncodedData( *pBytes );

    return data;
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::dropEvent
/** @param  pEvent 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
void MOS_AgentListView::dropEvent( QDropEvent* pEvent )
{
    if( !pEvent->provides( "pointer/pion" ) )
         return;

    QByteArray tmp = pEvent->encodedData( "pointer/pion" );

    QListViewItem* pItemToDrop;
    memcpy( &pItemToDrop, tmp.data(), sizeof( pItemToDrop ) );


    QListViewItem* pItemWhereToDrop = (QListViewItem*)itemAt( viewport()->mapFromParent( pEvent->pos() ) );

    if( !pItemToDrop || !pItemWhereToDrop || pItemToDrop->rtti() != eAgent )
    {
        pEvent->ignore();
        return;
    }

    if( pItemWhereToDrop->rtti() == eAgent )
    {
        MOS_Agent* pAgent = ((MT_ValuedListViewItem< MOS_Agent*, eAgent>*)pItemToDrop)->GetValue();
        MOS_Agent* pSupAgent = ((MT_ValuedListViewItem< MOS_Agent*, eAgent>*)pItemWhereToDrop)->GetValue();

        if( pAgent->GetParent() == 0 || pSupAgent->GetParent() != 0 )
        {
            pEvent->ignore();
            return;
        }

        // Envoi réseau
        MOS_ASN_MsgChangeAutomate asnMsg;
        asnMsg.GetAsnMsg().oid_pion     = pAgent->GetAgentID();
        asnMsg.GetAsnMsg().oid_automate  = pSupAgent->GetAgentID();
        asnMsg.Send( 351 );

        std::stringstream strMsg;
        strMsg << "Demande de passage de l'agent " << pAgent->GetName() << " sous les ordres de " << pSupAgent->GetName();
        MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );

        pEvent->accept();
    }

    if( pItemWhereToDrop->rtti() == eGtia )
    {
        MOS_Agent* pAgent = ((MT_ValuedListViewItem< MOS_Agent*, eAgent>*)pItemToDrop)->GetValue();
        MOS_Gtia* pGtia   = ((MT_ValuedListViewItem< MOS_Gtia*, eGtia>*)pItemWhereToDrop)->GetValue();

        if( pAgent->GetParent() != 0 )
        {
            pEvent->ignore();
            return;
        }

        // Envoi réseau
        MOS_ASN_MsgChangeGroupeConnaissance asnMsg;
        asnMsg.GetAsnMsg().oid_automate  = pAgent->GetAgentID();
        asnMsg.GetAsnMsg().oid_camp      = pGtia->GetTeam().GetID();
        asnMsg.GetAsnMsg().oid_groupe_connaissance = pGtia->GetID();
        asnMsg.Send( 351 );

        std::stringstream strMsg;
        strMsg << "Demande de passage de l'agent " << pAgent->GetName() << " dans le groupe de connaissance " << pGtia->GetID();
        MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );

        pEvent->accept();
    }
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::dragEnterEvent
/** @param  pEvent 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
void MOS_AgentListView::dragEnterEvent( QDragEnterEvent* pEvent )
{
    pEvent->accept( pEvent->provides( "pointer/pion" ) );
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::keyPressEvent
/** @param  pEvent 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void MOS_AgentListView::keyPressEvent( QKeyEvent* pEvent )
{
    if( ! MOS_App::GetApp().IsODBEdition() 
        || this->currentItem() == 0 
        || this->ToTeam( this->currentItem() ) != 0
        || ! (pEvent->key() == Qt::Key_Delete || pEvent->key() == Qt::Key_Backspace ) )
    {
        QListView::keyPressEvent( pEvent );
        return;
    }

    this->DeleteAssociatedElements( this->currentItem() );
    delete this->currentItem();

    // Disconnect and connect to avoid having the signal come back to us.
    disconnect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    MOS_SelectedElement selectedElement;
    emit ElementSelected( selectedElement );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );

    pEvent->accept();
}


// -----------------------------------------------------------------------------
// Name: MOS_AgentListView::DeleteAssociatedElements
/** @param  pItem 
*/
// Created: APE 2004-08-27
// -----------------------------------------------------------------------------
void MOS_AgentListView::DeleteAssociatedElements( QListViewItem* pItem )
{
    QListViewItem* pChild = pItem->firstChild();
    while( pChild != 0 )
    {
        this->DeleteAssociatedElements( pChild );
        pChild = pChild->nextSibling();
    }

    if( this->ToGtia( pItem ) != 0 )
    {
        MOS_Gtia* pGtia = this->ToGtia( pItem );
        pGtia->GetTeam().RemoveGtia( *pGtia );
        delete pGtia;
    }

    if( this->ToAgent( pItem ) != 0 )
    {
        MOS_Agent* pAgent = this->ToAgent( pItem );
        MOS_App::GetApp().GetAgentManager().RemoveAgent( *pAgent );
        delete pAgent;
    }
}
