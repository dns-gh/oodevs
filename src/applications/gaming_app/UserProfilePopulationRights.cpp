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

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights constructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfilePopulationRights::UserProfilePopulationRights( QWidget* pParent, kernel::Controllers& controllers, gui::ItemFactory_ABC& factory, const kernel::Profile_ABC& profile )
    : gui::PopulationListView( pParent, controllers, factory, profile )
    , UserProfileRights_ABC( this )
{
    connect( this, SIGNAL( clicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( Q3ListViewItem*, const QPoint&, int ) ) );
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
    Q3ScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: UserProfilePopulationRights::setColumnWidth
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfilePopulationRights::setColumnWidth( int column, int w )
{
    Q3ListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) - columnWidth( 2 ) : w );
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
void UserProfilePopulationRights::OnItemClicked( Q3ListViewItem* item, const QPoint& point, int column )
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
