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
// $Archive: /MVW_v10/Build/SDK/Light2/src/AgentListView.cpp $
// $Author: Ape $
// $Modtime: 23/11/04 18:42 $
// $Revision: 14 $
// $Workfile: AgentListView.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "AgentListView.h"
#include "moc_AgentListView.cpp"

#include "Agent.h"
#include "AgentManager.h"
#include "Team.h"
#include "Gtia.h"
#include "App.h"
#include "MainWindow.h"
#include "Options.h"
#include "ActionContext.h"
#include "AgentKnowledge.h"
#include "ObjectKnowledge.h"
#include "ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: AgentListView constructor
/** @param  pParent 
*/
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
AgentListView::AgentListView( QWidget* pParent )
    : QListView     ( pParent )
    , pPopupMenu_   ( 0 )
{
    this->setMinimumSize( 1, 1 );
    this->addColumn( "Unités" );
    this->setRootIsDecorated( true );
    this->setResizeMode( QListView::LastColumn );
    this->setAcceptDrops( true );

    connect( &App::GetApp(), SIGNAL( AgentCreated( Agent& ) ), this, SLOT( AddAgent( Agent& ) ) );
    connect( &App::GetApp(), SIGNAL( AgentUpdated( Agent& ) ), this, SLOT( OnAgentUpdated( Agent& ) ) );
    connect( &App::GetApp(), SIGNAL( AgentReparented( Agent& ) ), this, SLOT( OnAgentReparented( Agent& ) ) );
    connect( &App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( clear() ) );

    connect( &App::GetApp(), SIGNAL( GtiaCreated( Gtia& ) ), this, SLOT( AddGtia( Gtia& ) ) );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );

    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),                   &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
    connect( this, SIGNAL( ElementSelected( SelectedElement& ) ),               &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ) );

    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
    connect( &MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                           this,   SLOT( OnTeamChanged() ) );

    if( App::GetApp().IsODBEdition() )
    {
        // Refresh the display
        OnTeamChanged();
    }
}


// -----------------------------------------------------------------------------
// Name: AgentListView destructor
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
AgentListView::~AgentListView()
{
}


// -----------------------------------------------------------------------------
// Name: AgentListView::AddAgent
/** @param  agent 
*/
// Created: APE 2004-03-18
// -----------------------------------------------------------------------------
void AgentListView::AddAgent( Agent& agent )
{
    int nPlayedTeam = MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;
    if( nPlayedTeam != Options::eController && nPlayedTeam != (int)agent.GetTeam().GetIdx() )
        return;

    // Check if the agent is not already inserted.
#ifdef _DEBUG
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
#endif // DEBUG

    // Team
    Team& team = agent.GetTeam();
    QListViewItem* pTeamItem = this->firstChild();
    while( pTeamItem != 0 )
    {
        MT_ValuedListViewItem< Team*, eTeam >* pCastItem = (MT_ValuedListViewItem< Team*, (int)eTeam >*)pTeamItem;
        if( pCastItem->GetValue() == &team )
            break;
        pTeamItem = pTeamItem->nextSibling();
    }

    if( pTeamItem == 0 )
    {
        pTeamItem = new MT_ValuedListViewItem< Team*, eTeam >( &team, this, team.GetName().c_str() );
        pTeamItem->setDropEnabled( false );
        pTeamItem->setDragEnabled( false );
    }

    // GTIA
    Gtia& gtia = agent.GetGtia();
    QListViewItem* pGtiaItem = pTeamItem->firstChild();
    while( pGtiaItem != 0 )
    {
        MT_ValuedListViewItem< Gtia*, eGtia >* pCastItem = (MT_ValuedListViewItem< Gtia*, (int)eGtia >*)pGtiaItem;
        if( pCastItem->GetValue() == &gtia )
            break;
        pGtiaItem = pGtiaItem->nextSibling();
    }

    if( pGtiaItem == 0 )
    {
        pGtiaItem = new MT_ValuedListViewItem< Gtia*, eGtia >( &gtia, pTeamItem, tr( "Gtia " ) + QString::number( gtia.GetID() ) );
        pGtiaItem->setDropEnabled( true );
        pGtiaItem->setDragEnabled( false );
    }
    // Group
    if( agent.GetParent() == 0 )
    {
        QListViewItem* pNewItem = new MT_ValuedListViewItem< Agent*, eAgent>( &agent, pGtiaItem, agent.GetName().c_str() );
        pNewItem->setDropEnabled( ! App::GetApp().IsODBEdition() );
        pNewItem->setDragEnabled( ! App::GetApp().IsODBEdition() );
        if( agent.IsEmbraye() )
            pNewItem->setPixmap( 0, MAKE_PIXMAP( embraye ) );
        else
            pNewItem->setPixmap( 0, MAKE_PIXMAP( debraye ) );

        // When editing an ODB, we want to see groups as soon as they are created. 
        if( App::GetApp().IsODBEdition() )
            this->ensureItemVisible( pNewItem );
    }
    else
    {
        QListViewItem* pParent = pGtiaItem->firstChild();
        while( pParent != 0 )
        {
            if( this->ToAgent( pParent ) == agent.GetParent() )
            {
                QListViewItem* pNewItem = new MT_ValuedListViewItem< Agent*, eAgent>( &agent, pParent, agent.GetName().c_str() );
                pNewItem->setDropEnabled( false );
                pNewItem->setDragEnabled( ! App::GetApp().IsODBEdition() );
                return;
            }
            pParent = pParent->nextSibling();
        }

        // Could not find the parent...
        assert( false );
    }
}


// -----------------------------------------------------------------------------
// Name: AgentListView::AddGtia
/** @param  gtia 
*/
// Created: APE 2004-08-25
// -----------------------------------------------------------------------------
void AgentListView::AddGtia( Gtia& gtia )
{
    int nPlayedTeam = MainWindow::GetMainWindow().GetOptions().nPlayedTeam_;
    if( nPlayedTeam != Options::eController && nPlayedTeam != (int)gtia.GetTeam().GetIdx() )
        return;

    // Check if the gtia is not already inserted.
#ifdef _DEBUG
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
#endif // DEBUG

    // Team
    const Team& team = gtia.GetTeam();
    QListViewItem* pTeamItem = this->firstChild();
    while( pTeamItem != 0 )
    {
        MT_ValuedListViewItem< const Team*, eTeam >* pCastItem = (MT_ValuedListViewItem< const Team*, (int)eTeam >*)pTeamItem;
        if( pCastItem->GetValue() == &team )
            break;
        pTeamItem = pTeamItem->nextSibling();
    }

    if( pTeamItem == 0 )
    {
        pTeamItem = new MT_ValuedListViewItem< const Team*, eTeam >( &team, this, team.GetName().c_str() );
        pTeamItem->setDropEnabled( false );
        pTeamItem->setDragEnabled( false );
    }

    // GTIA
    QListViewItem* pGtiaItem = new MT_ValuedListViewItem< Gtia*, eGtia >( &gtia, pTeamItem, tr( "Gtia " ) + QString::number( gtia.GetID() ) );
    pGtiaItem->setDropEnabled( false );
    pGtiaItem->setDragEnabled( false );
}


// -----------------------------------------------------------------------------
// Name: AgentListView::SetSelectedElement
/** @param  pAgent 
*/
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
void AgentListView::SetSelectedElement( SelectedElement& selectedElement )
{
    //$$$$ Pas terrible, à améliorer.
    if(    selectedElement.pObject_ != 0 
        || selectedElement.pPopulation_ != 0 
		|| ( selectedElement.pTeam_ == 0 && selectedElement.pAgent_ == 0 && selectedElement.pGtia_ == 0 ))
    {
        this->selectAll( false );
        return;
    }

    // Check if it's not already selected.
    if(    ( selectedElement.pAgent_           != 0 && this->ToAgent( this->selectedItem() ) == selectedElement.pAgent_ )
        || ( selectedElement.pGtia_            != 0 && this->ToGtia( this->selectedItem() ) == selectedElement.pGtia_ )
        || ( selectedElement.pTeam_            != 0 && this->ToTeam( this->selectedItem() ) == selectedElement.pTeam_ )
        || ( selectedElement.pAgentKnowledge_  != 0 && this->ToGtia( this->selectedItem() ) == &(selectedElement.pAgentKnowledge_->GetOwner()) )
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
            if( MainWindow::GetMainWindow().GetOptions().bOpenTreeToItem_ )
                this->ensureItemVisible( it.current() );
            return;
        }
        ++it;
    }

    // Could not find the agent to select.
    assert( false );
}


// -----------------------------------------------------------------------------
// Name: AgentListView::OnRequestPopup
/** @param  pItem 
    @param  pos 
    @param  nCol 
*/
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
void AgentListView::OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int /*nCol*/ )
{
    delete pPopupMenu_;
    pPopupMenu_ = new QPopupMenu( this );

    SelectedElement selectedElement;
    if( this->ToAgent( pItem ) != 0 )
        selectedElement.pAgent_ = this->ToAgent( pItem );
    if( this->ToGtia( pItem ) != 0 )
        selectedElement.pGtia_ = this->ToGtia( pItem );
    if( this->ToTeam( pItem ) != 0 )
        selectedElement.pTeam_ = this->ToTeam( pItem );

    // Give a chance to the rest of the objets to fill up the popup menu if necessary.
    emit NewPopupMenu( *pPopupMenu_, ActionContext( selectedElement, 0 ) );

    // Only popup the menu if some items have been added to it.
    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}


// -----------------------------------------------------------------------------
// Name: AgentListView::OnRequestCenter
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
void AgentListView::OnRequestCenter()
{
    Agent* pAgent = this->ToAgent( this->selectedItem() );
    if( pAgent != 0 )
        emit CenterOnPoint( pAgent->GetPos() );
}


// -----------------------------------------------------------------------------
// Name: AgentListView::OnSelectionChange
/** @param  pItem 
*/
// Created: APE 2004-03-19
// -----------------------------------------------------------------------------
void AgentListView::OnSelectionChange( QListViewItem* pItem )
{
    if( pItem == 0 )
        return;

    Agent* pAgent = this->ToAgent( pItem );
    SelectedElement selectedElement;
    if( pAgent != 0 )
        selectedElement = SelectedElement( *pAgent );

    Gtia* pGtia = this->ToGtia( pItem );
    if( pGtia != 0 )
        selectedElement = SelectedElement( *pGtia );

    Team* pTeam = this->ToTeam( pItem );
    if( pTeam != 0 )
        selectedElement = SelectedElement( *pTeam );

    // Disconnect and connect to avoid having the signal come back to us.
    disconnect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
    emit ElementSelected( selectedElement );
    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
}


// -----------------------------------------------------------------------------
// Name: AgentListView::OnTeamChanged
// Created: APE 2004-05-26
// -----------------------------------------------------------------------------
void AgentListView::OnTeamChanged()
{
    this->clear();

    // In ODB mode, show all teams & gtias
    if( App::GetApp().IsODBEdition() )
    {
        const AgentManager::T_TeamMap& teams = App::GetApp().GetAgentManager().GetTeams();
        for( AgentManager::CIT_TeamMap it = teams.begin(); it != teams.end(); ++it )
        {
            Team& team = *((*it).second);
            QListViewItem* pTeamItem = new MT_ValuedListViewItem< Team*, eTeam >( &team, this, team.GetName().c_str() );
            pTeamItem->setDropEnabled( false );
            pTeamItem->setDragEnabled( false );

            const Team::T_GtiaMap& gtias = team.GetGtias();
            for( Team::CIT_GtiaMap it2 = gtias.begin(); it2 != gtias.end(); ++it2 )
                this->AddGtia( *((*it2).second) );
        }
    }

    AgentManager::CT_AgentMap agentMap = App::GetApp().GetAgentManager().GetAgentList();
    // Add the automates first.
    for( AgentManager::CIT_AgentMap it = agentMap.begin(); it != agentMap.end(); ++it )
        if( (it->second)->GetParent() == 0 )
            this->AddAgent( *(it->second) );
    // Then the units. This is done to avoid parenting problems in the listview.
    for( it = agentMap.begin(); it != agentMap.end(); ++it )
        if( (it->second)->GetParent() != 0 )
            this->AddAgent( *(it->second) );
}


// -----------------------------------------------------------------------------
// Name: AgentListView::OnAgentUpdated
// Created: APE 2004-06-14
// -----------------------------------------------------------------------------
void AgentListView::OnAgentUpdated( Agent& agent )
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
// Name: AgentListView::OnAgentReparented
/** @param  agent 
*/
// Created: APE 2004-07-29
// -----------------------------------------------------------------------------
void AgentListView::OnAgentReparented( Agent& agent )
{
    QListViewItemIterator it( this->firstChild() );
    while( it.current() )
    {
        if( this->ToAgent( it.current() ) == &agent )
        {
            delete it.current();
            this->AddAgent( agent );
            Agent::T_AgentVector children = agent.GetChildren();
            for( Agent::IT_AgentVector it = children.begin(); it != children.end(); ++it )
                this->AddAgent( **it );
            return;
        }
        ++it;
    }
}


// -----------------------------------------------------------------------------
// Name: AgentListView::dragObject
/** @return 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
QDragObject* AgentListView::dragObject()
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
// Name: AgentListView::dropEvent
/** @param  pEvent 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
void AgentListView::dropEvent( QDropEvent* pEvent )
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
        Agent* pAgent = ((MT_ValuedListViewItem< Agent*, eAgent>*)pItemToDrop)->GetValue();
        Agent* pSupAgent = ((MT_ValuedListViewItem< Agent*, eAgent>*)pItemWhereToDrop)->GetValue();

        if( pAgent->GetParent() == 0 || pSupAgent->GetParent() != 0 )
        {
            pEvent->ignore();
            return;
        }

        // Envoi réseau
        ASN_MsgChangeAutomate asnMsg;
        asnMsg.GetAsnMsg().oid_pion     = pAgent->GetID();
        asnMsg.GetAsnMsg().oid_automate  = pSupAgent->GetID();
        asnMsg.Send( 351 );

        std::stringstream strMsg;
        strMsg << "Demande de passage de l'agent " << pAgent->GetName() << " sous les ordres de " << pSupAgent->GetName();
        MT_LOG_INFO( strMsg.str().c_str(), eSent, 0 );

        pEvent->accept();
    }

    if( pItemWhereToDrop->rtti() == eGtia )
    {
        Agent* pAgent = ((MT_ValuedListViewItem< Agent*, eAgent>*)pItemToDrop)->GetValue();
        Gtia* pGtia   = ((MT_ValuedListViewItem< Gtia*, eGtia>*)pItemWhereToDrop)->GetValue();

        if( pAgent->GetParent() != 0 )
        {
            pEvent->ignore();
            return;
        }

        // Envoi réseau
        ASN_MsgChangeGroupeConnaissance asnMsg;
        asnMsg.GetAsnMsg().oid_automate  = pAgent->GetID();
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
// Name: AgentListView::dragEnterEvent
/** @param  pEvent 
*/
// Created: APE 2004-07-28
// -----------------------------------------------------------------------------
void AgentListView::dragEnterEvent( QDragEnterEvent* pEvent )
{
    pEvent->accept( pEvent->provides( "pointer/pion" ) );
}


// -----------------------------------------------------------------------------
// Name: AgentListView::keyPressEvent
/** @param  pEvent 
*/
// Created: APE 2004-08-05
// -----------------------------------------------------------------------------
void AgentListView::keyPressEvent( QKeyEvent* pEvent )
{
    if( ! App::GetApp().IsODBEdition() 
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
    disconnect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
    SelectedElement selectedElement;
    emit ElementSelected( selectedElement );
    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );

    pEvent->accept();
}


// -----------------------------------------------------------------------------
// Name: AgentListView::DeleteAssociatedElements
/** @param  pItem 
*/
// Created: APE 2004-08-27
// -----------------------------------------------------------------------------
void AgentListView::DeleteAssociatedElements( QListViewItem* pItem )
{
    QListViewItem* pChild = pItem->firstChild();
    while( pChild != 0 )
    {
        this->DeleteAssociatedElements( pChild );
        pChild = pChild->nextSibling();
    }

    if( this->ToGtia( pItem ) != 0 )
    {
        Gtia* pGtia = this->ToGtia( pItem );
        pGtia->GetTeam().RemoveGtia( *pGtia );
        delete pGtia;
    }

    if( this->ToAgent( pItem ) != 0 )
    {
        Agent* pAgent = this->ToAgent( pItem );
        App::GetApp().GetAgentManager().RemoveAgent( *pAgent );
        delete pAgent;
    }
}
