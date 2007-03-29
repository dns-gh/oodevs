// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UserProfilePopulationRights.h"
#include "moc_UserProfilePopulationRights.cpp"

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights constructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfilePopulationRights::UserProfilePopulationRights( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile )
    : gui::PopulationListView( pParent, controllers, factory, profile )
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
// Name: UserProfilePopulationRights::showEvent
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::showEvent( QShowEvent* event )
{
    OnShow();
    gui::PopulationListView::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::hideEvent
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::hideEvent( QHideEvent* event )
{
    OnHide();
    gui::PopulationListView::hideEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::OnItemClicked
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::OnItemClicked( QListViewItem* item, const QPoint& point, int column )
{
    UserProfileRights_ABC::OnItemClicked( item, point, column );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::NeedsCommit
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
bool UserProfilePopulationRights::NeedsCommit() const
{
    return isVisible();
}
