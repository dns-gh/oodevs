// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfilePopulationRights.h"
#include "moc_UserProfilePopulationRights.cpp"
#include "PreparationProfile.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights constructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfilePopulationRights::UserProfilePopulationRights( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory )
    : gui::PopulationListView( pParent, controllers, factory, PreparationProfile::GetProfile() )
    , UserProfileRights_ABC( this )
{
    connect( this, SIGNAL( clicked( QListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( QListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights destructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfilePopulationRights::~UserProfilePopulationRights()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::viewportResizeEvent
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::viewportResizeEvent( QResizeEvent* e )
{
    QScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::setColumnWidth
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::setColumnWidth( int column, int w )
{
    QListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) - columnWidth( 2 ) : w );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::OnItemClicked
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::OnItemClicked( QListViewItem* item, const QPoint& point, int column )
{
    UserProfileRights_ABC::OnItemClicked( item, point, column );
}
