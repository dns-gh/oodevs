// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "PopulationListView.h"
#include "Population.h"
#include "Team.h"
#include "Controller.h"
#include "ValuedListItem.h"

// -----------------------------------------------------------------------------
// Name: PopulationListView constructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::PopulationListView( QWidget* pParent, Controller& controller )
    : QListView     ( pParent )
    , pPopupMenu_   ( 0 )
{
    this->setMinimumSize( 1, 1 );
    this->addColumn( "Populations" );
    this->setRootIsDecorated( true );
    this->setResizeMode( QListView::LastColumn );
    this->setAcceptDrops( true );

    controller.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: PopulationListView destructor
// Created: HME 2005-10-03
// -----------------------------------------------------------------------------
PopulationListView::~PopulationListView()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyCreated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void PopulationListView::NotifyCreated( const Population& popu )
{
    const Team& team = popu.GetTeam();
    ValuedListItem* teamItem = FindSibling( &team, firstChild() );
    if( ! teamItem )
        teamItem = new ValuedListItem( &team, this, team.GetName().c_str() );
    new ValuedListItem( &popu, teamItem, popu.GetName().c_str() );
}
    
// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyUpdated
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void PopulationListView::NotifyUpdated( const Population& )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationListView::NotifyDeleted
// Created: AGE 2006-02-16
// -----------------------------------------------------------------------------
void PopulationListView::NotifyDeleted( const Population& popu )
{
    delete FindItem( &popu, firstChild() );
}
