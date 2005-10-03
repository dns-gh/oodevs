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

#include "MOS_Light2_pch.h"
#include "MOS_PopulationListView.h"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "moc_MOS_PopulationListView.cpp"
#include "MOS_Population.h";
#include "MOS_Team.h"
#include "MOS_AgentManager.h"
#include "MOS_ActionContext.h"

// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView constructor
/** @param  pParent 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
MOS_PopulationListView::MOS_PopulationListView( QWidget* pParent )
    : QListView     ( pParent )
    , pPopupMenu_   ( 0 )
{
    this->setMinimumSize( 1, 1 );
    this->addColumn( "Populations" );
    this->setRootIsDecorated( true );
    this->setResizeMode( QListView::LastColumn );
    this->setAcceptDrops( true );

    connect( &MOS_App::GetApp(), SIGNAL( PopulationCreated( MOS_Population& ) ), this, SLOT( AddPopulation( MOS_Population& ) ) );

	connect( &MOS_App::GetApp(), SIGNAL( ConnexionStatusChanged( bool ) ), this, SLOT( clear() ) );

    connect( this, SIGNAL( contextMenuRequested( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestPopup( QListViewItem*, const QPoint&, int ) ) );
    connect( this, SIGNAL( doubleClicked       ( QListViewItem*, const QPoint&, int ) ), this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( spacePressed        ( QListViewItem* ) ),                     this, SLOT( OnRequestCenter() ) );
    connect( this, SIGNAL( selectionChanged    ( QListViewItem* ) ),                     this, SLOT( OnSelectionChange( QListViewItem* ) ) );

    connect( this, SIGNAL( CenterOnPoint( const MT_Vector2D& ) ),                   &MOS_MainWindow::GetMainWindow(), SIGNAL( CenterOnPoint( const MT_Vector2D& ) ) );
    connect( this, SIGNAL( ElementSelected( MOS_SelectedElement& ) ),               &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ) );
    connect( this, SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ), &MOS_MainWindow::GetMainWindow(), SIGNAL( NewPopupMenu( QPopupMenu&, const MOS_ActionContext& ) ) );

    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( TeamChanged() ),                           this,   SLOT( OnTeamChanged() ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView destructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
MOS_PopulationListView::~MOS_PopulationListView()
{
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView::AddPopulation
/** @param  population 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationListView::AddPopulation( MOS_Population& population )
{
    // Team
    MOS_Team& team = const_cast< MOS_Team& >( population.GetTeam() );
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

    QListViewItem* pNewItem = new MT_ValuedListViewItem< MOS_Population*, eAgent>( &population, pTeamItem, population.GetName().c_str() );
    pNewItem->setDropEnabled( ! MOS_App::GetApp().IsODBEdition() );
    pNewItem->setDragEnabled( ! MOS_App::GetApp().IsODBEdition() );
    pNewItem->setPixmap( 0, MAKE_PIXMAP( embraye ) );
    // When editing an ODB, we want to see groups as soon as they are created. 
    if( MOS_App::GetApp().IsODBEdition() )
        this->ensureItemVisible( pNewItem );


}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView::SetSelectedElement
/** @param  selectedElement 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationListView::SetSelectedElement( MOS_SelectedElement& selectedElement )
{
}


// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView::OnRequestPopup
/** @param  pItem 
    @param  pos 
    @param  nCol 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationListView::OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int nCol )
{
	delete pPopupMenu_;
    pPopupMenu_ = new QPopupMenu( this );

    MOS_SelectedElement selectedElement;
    if( this->ToPopulation( pItem ) != 0 )
        selectedElement.pPopulation_ = this->ToPopulation( pItem );
    if( this->ToTeam( pItem ) != 0 )
        selectedElement.pTeam_ = this->ToTeam( pItem );

    // Give a chance to the rest of the objets to fill up the popup menu if necessary.
    emit NewPopupMenu( *pPopupMenu_, MOS_ActionContext( selectedElement, 0 ) );

    // Only popup the menu if some items have been added to it.
    if( pPopupMenu_->count() > 0 )
        pPopupMenu_->popup( pos );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView::OnRequestCenter
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationListView::OnRequestCenter()
{
	MOS_Population* pPopulation = this->ToPopulation( this->selectedItem() );
    if( pPopulation != 0 )
        emit CenterOnPoint( pPopulation->GetPos() );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView::OnSelectionChange
/** @param  pItem 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationListView::OnSelectionChange( QListViewItem* pItem )
{
	if( pItem == 0 )
    return;

    MOS_Population* pPopulation = this->ToPopulation( pItem );
    MOS_SelectedElement selectedElement;
    if( pPopulation != 0 )
        selectedElement = MOS_SelectedElement( *pPopulation );

    MOS_Team* pTeam = this->ToTeam( pItem );
    if( pTeam != 0 )
        selectedElement = MOS_SelectedElement( *pTeam );

    // Disconnect and connect to avoid having the signal come back to us.
    disconnect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
    emit ElementSelected( selectedElement );
    connect( &MOS_MainWindow::GetMainWindow(), SIGNAL( ElementSelected( MOS_SelectedElement& ) ), this,   SLOT( SetSelectedElement( MOS_SelectedElement& ) ) );
}

// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView::OnTeamChanged
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationListView::OnTeamChanged()
{
	this->clear();

    // In ODB mode, show all teams
    if( MOS_App::GetApp().IsODBEdition() )
    {
        const MOS_AgentManager::T_TeamMap& teams = MOS_App::GetApp().GetAgentManager().GetTeams();
        for( MOS_AgentManager::CIT_TeamMap it = teams.begin(); it != teams.end(); ++it )
        {
            MOS_Team& team = *((*it).second);
            QListViewItem* pTeamItem = new MT_ValuedListViewItem< MOS_Team*, eTeam >( &team, this, team.GetName().c_str() );
            pTeamItem->setDropEnabled( false );
            pTeamItem->setDragEnabled( false );
        }
    }

    MOS_AgentManager::CT_PopulationMap popMap = MOS_App::GetApp().GetAgentManager().GetPopulationList();
    for( MOS_AgentManager::CIT_PopulationMap it = popMap.begin(); it != popMap.end(); ++it )
		this->AddPopulation( *(it->second) );
}
