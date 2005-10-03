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
#include "moc_MOS_PopulationListView.cpp"

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_Population.h";
#include "MOS_Team.h"

// -----------------------------------------------------------------------------
// Name: MOS_PopulationListView constructor
/** @param  pParent 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
MOS_PopulationListView::MOS_PopulationListView( QWidget* pParent )
{
    this->setMinimumSize( 1, 1 );
    this->addColumn( "Populations" );
    this->setRootIsDecorated( true );
    this->setResizeMode( QListView::LastColumn );
    this->setAcceptDrops( true );

    connect( &MOS_App::GetApp(), SIGNAL( PopulationCreated( MOS_Population& ) ), this, SLOT( AddPopulation( MOS_Population& ) ) );
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
// Name: MOS_PopulationListView::OnAgentUpdated
/** @param  population 
*/
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
void MOS_PopulationListView::OnAgentUpdated( MOS_Population& population ){
}