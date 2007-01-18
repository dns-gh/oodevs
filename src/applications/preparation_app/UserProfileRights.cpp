// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfileRights.h"
#include "moc_UserProfileRights.cpp"
#include "PreparationProfile.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserProfileRights constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileRights::UserProfileRights( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory, SymbolIcons& icons )
    : HierarchyListView< TacticalHierarchies >( parent, controllers, factory, PreparationProfile::GetProfile(), icons )
    , UserProfileRights_ABC( this )
{
    connect( this, SIGNAL( clicked( QListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( QListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileRights::~UserProfileRights()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights::viewportResizeEvent
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void UserProfileRights::viewportResizeEvent( QResizeEvent* e )
{
    QScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void UserProfileRights::setColumnWidth( int column, int w )
{
    QListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) - columnWidth( 2 ) : w );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights::showEvent
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights::showEvent( QShowEvent* event )
{
    OnShow();
    T_Parent::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights::OnItemClicked
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfileRights::OnItemClicked( QListViewItem* item, const QPoint& point, int column )
{
    UserProfileRights_ABC::OnItemClicked( item, point, column );
}
