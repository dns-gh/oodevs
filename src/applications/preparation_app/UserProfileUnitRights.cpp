// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfileUnitRights.h"
#include "moc_UserProfileUnitRights.cpp"
#include "PreparationProfile.h"
#include "clients_kernel/Entity_ABC.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileUnitRights::UserProfileUnitRights( QWidget* parent, Controllers& controllers, ItemFactory_ABC& factory, EntitySymbols& icons )
    : HierarchyListView< TacticalHierarchies >( parent, controllers, factory, PreparationProfile::GetProfile(), icons )
    , UserProfileRights_ABC( this )
{
    controllers_.Register( *this );
    connect( this, SIGNAL( clicked( QListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( QListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileUnitRights::~UserProfileUnitRights()
{
    controllers_.Unregister( *this );
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::viewportResizeEvent
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void UserProfileUnitRights::viewportResizeEvent( QResizeEvent* e )
{
    QScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void UserProfileUnitRights::setColumnWidth( int column, int w )
{
    QListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) - columnWidth( 2 ) : w );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::OnItemClicked
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfileUnitRights::OnItemClicked( QListViewItem* item, const QPoint& point, int column )
{
    UserProfileRights_ABC::OnItemClicked( item, point, column );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::NotifyUpdated
// Created: SBO 2008-08-26
// -----------------------------------------------------------------------------
void UserProfileUnitRights::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    if( gui::ValuedListItem* item = FindItem( &entity, firstChild() ) )
        item->SetNamed( entity );
}
