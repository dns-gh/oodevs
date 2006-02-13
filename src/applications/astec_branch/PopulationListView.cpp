// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "astec_pch.h"
#include "PopulationListView.h"

#include "App.h"
#include "MainWindow.h"
#include "moc_PopulationListView.cpp"
#include "Population.h"
#include "PopulationConcentration.h"
#include "PopulationFlow.h"
#include "Team.h"
#include "AgentManager.h"
#include "ActionContext.h"
#include "Options.h"

// -----------------------------------------------------------------------------
// Name: PopulationListView constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::PopulationListView( QWidget* pParent )
    : QListView     ( pParent )
    , pPopupMenu_   ( 0 )
{
    this->setMinimumSize( 1, 1 );
    this->addColumn( "Populations" );
    this->setRootIsDecorated( true );
    this->setResizeMode( QListView::LastColumn );
    this->setAcceptDrops( true );

    connect( &App::GetApp(), SIGNAL( PopulationCreated( Population& ) ), this, SLOT( AddPopulation( Population& ) ) );

	connect( &App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( clear() ) );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );

    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),                   &MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
    connect( this, SIGNAL( ElementSelected( SelectedElement& ) ),               &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ), &MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const ActionContext& ) ) );

    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this,   SLOT( SetSelectedElement( SelectedElement& ) ) );
    connect( &MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                           this,   SLOT( OnTeamChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView destructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::~PopulationListView()
{
}


// -----------------------------------------------------------------------------
// Name: PopulationListView::AddPopulation
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void PopulationListView::AddPopulation( Population& population )
{
    // Team
    Team& team = const_cast< Team& >( population.GetTeam() );
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

    QListViewItem* pNewItem = new MT_ValuedListViewItem< Population*, eAgent>( &population, pTeamItem, population.GetName().c_str() );
    pNewItem->setDropEnabled( ! App::GetApp().IsODBEdition() );
    pNewItem->setDragEnabled( ! App::GetApp().IsODBEdition() );
    pNewItem->setPixmap( 0, MAKE_PIXMAP( embraye ) );
    // When editing an ODB, we want to see groups as soon as they are created. 
    if( App::GetApp().IsODBEdition() )
        this->ensureItemVisible( pNewItem );


}

// -----------------------------------------------------------------------------
// Name: PopulationListView::SetSelectedElement
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void PopulationListView::SetSelectedElement( SelectedElement& selectedElement )
{
    const Population* pPopulation;
    if( selectedElement.pPopulation_ )
        pPopulation = selectedElement.pPopulation_;
    else if( selectedElement.pPopulationConcentration_ )
        pPopulation = &selectedElement.pPopulationConcentration_->GetPopulation();
    else if( selectedElement.pPopulationFlow_ )
        pPopulation = &selectedElement.pPopulationFlow_->GetPopulation();
    else
    {
        selectAll( false );
        return;
    }

    // Check if it's not already selected.
    if( ToPopulation( selectedItem() ) == pPopulation )
        return;

    QListViewItemIterator it( firstChild() );
    while( it.current() )
    {
        if( ToPopulation( it.current() ) == pPopulation )
        {
            disconnect( this, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
            setSelected( it.current(), true );
            connect( this, SIGNAL( selectionChanged( QListViewItem* ) ), this, SLOT( OnSelectionChange( QListViewItem* ) ) );
            if( MainWindow::GetMainWindow().GetOptions().bOpenTreeToItem_ )
                ensureItemVisible( it.current() );
            return;
        }
        ++it;
    }

    // Could not find the agent to select.
    assert( false );
}


// -----------------------------------------------------------------------------
// Name: PopulationListView::OnRequestPopup
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void PopulationListView::OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int /*nCol*/ )
{
	delete pPopupMenu_;
    pPopupMenu_ = new QPopupMenu( this );

    SelectedElement selectedElement;
    if( ToPopulation( pItem ) != 0 )
        selectedElement.pPopulation_ = ToPopulation( pItem );
    else if( ToTeam( pItem ) != 0 )
        selectedElement.pTeam_ = ToTeam( pItem );

    // Give a chance to the rest of the objets to fill up the popup menu if necessary.
    emit NewPopupMenu( *pPopupMenu_, ActionContext( selectedElement, 0 ) );

    // Only popup the menu if some items have been added to it.
    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OnRequestCenter
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void PopulationListView::OnRequestCenter()
{
	Population* pPopulation = this->ToPopulation( this->selectedItem() );
    if( pPopulation != 0 && !pPopulation->IsEmpty() )
        emit CenterOnPoint( pPopulation->GetPos() );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OnSelectionChange
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void PopulationListView::OnSelectionChange( QListViewItem* pItem )
{
	if( ! pItem )
        return;

    Population* pPopulation = ToPopulation( pItem );
    SelectedElement selectedElement;
    if( pPopulation != 0 )
        selectedElement = SelectedElement( *pPopulation );

    Team* pTeam = ToTeam( pItem );
    if( pTeam != 0 )
        selectedElement = SelectedElement( *pTeam );

    // Disconnect and connect to avoid having the signal come back to us.
    disconnect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this, SLOT( SetSelectedElement( SelectedElement& ) ) );
    emit ElementSelected( selectedElement );
    connect( &MainWindow::GetMainWindow(), SIGNAL( ElementSelected( SelectedElement& ) ), this, SLOT( SetSelectedElement( SelectedElement& ) ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::OnTeamChanged
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void PopulationListView::OnTeamChanged()
{
	this->clear();

    // In ODB mode, show all teams
    if( App::GetApp().IsODBEdition() )
    {
//        const AgentManager::T_TeamMap& teams = App::GetApp().GetAgentManager().GetTeams();
//        for( AgentManager::CIT_TeamMap it = teams.begin(); it != teams.end(); ++it )
//        {
//            Team& team = *((*it).second);
//            QListViewItem* pTeamItem = new MT_ValuedListViewItem< Team*, eTeam >( &team, this, team.GetName().c_str() );
//            pTeamItem->setDropEnabled( false );
//            pTeamItem->setDragEnabled( false );
//        }
    }

//    AgentManager::CT_PopulationMap popMap = App::GetApp().GetAgentManager().GetPopulationList();
//    for( AgentManager::CIT_PopulationMap it = popMap.begin(); it != popMap.end(); ++it )
//        AddPopulation( *(it->second) );
}


// -----------------------------------------------------------------------------
// Name: PopulationListView::keyPressEvent
// Created: HME 2006-01-03
// -----------------------------------------------------------------------------
void PopulationListView::keyPressEvent( QKeyEvent* pEvent )
{
    if( pEvent->key() == Qt::Key_Delete || pEvent->key() == Qt::Key_Backspace )
    {
        Population* pPopulation = this->ToPopulation( this->selectedItem() );
        assert( pPopulation );
        if( App::GetApp().IsODBEdition() )
        {
            App::GetApp().NotifyPopulationDeleted( *pPopulation );
            RemovePopulation( *pPopulation );
            App::GetApp().GetAgentManager().RemovePopulation( *pPopulation );
        }
            
        pEvent->accept();
        return;
    }

    QListView::keyPressEvent( pEvent );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::RemovePopulation
// Created: HME 2006-01-03
// -----------------------------------------------------------------------------
void PopulationListView::RemovePopulation( Population& population )
{
    QListViewItemIterator it( this );
    while( it.current() )
    {
        MT_ValuedListViewItem< Population*, eAgent>* pCastItem = (MT_ValuedListViewItem< Population*, eAgent>*)(it.current());
        if( pCastItem->GetValue() == &population )
        {
            delete it.current();
            return;
        }
        ++it;
    }
}