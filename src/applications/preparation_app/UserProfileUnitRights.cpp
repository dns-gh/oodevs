// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfileUnitRights.h"
#include "moc_UserProfileUnitRights.cpp"
#include "PreparationProfile.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"

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
    connect( this, SIGNAL( clicked( Q3ListViewItem*, const QPoint&, int ) ), SLOT( OnItemClicked( Q3ListViewItem*, const QPoint&, int ) ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileUnitRights::~UserProfileUnitRights()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::Display
// Created: JSR 2011-09-15
// -----------------------------------------------------------------------------
void UserProfileUnitRights::Display( const kernel::Entity_ABC& entity, gui::ValuedListItem* item )
{
    T_Parent::Display( entity, item);
    LongNameHelper::SetItemLongName( entity, *item );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::viewportResizeEvent
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void UserProfileUnitRights::viewportResizeEvent( QResizeEvent* e )
{
    Q3ScrollView::viewportResizeEvent( e );
    setColumnWidth( 0, -1 );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::setColumnWidth
// Created: SBO 2006-11-30
// -----------------------------------------------------------------------------
void UserProfileUnitRights::setColumnWidth( int column, int w )
{
    Q3ListView::setColumnWidth( column, column == 0 ? visibleWidth() - columnWidth( 1 ) - columnWidth( 2 ) : w );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::OnItemClicked
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfileUnitRights::OnItemClicked( Q3ListViewItem* item, const QPoint& point, int column )
{
    UserProfileRights_ABC::OnItemClicked( item, point, column );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::NotifyUpdated
// Created: SBO 2008-08-26
// -----------------------------------------------------------------------------
void UserProfileUnitRights::NotifyUpdated( const Entity_ABC& entity )
{
    if( ValuedListItem* item = FindItem( &entity, firstChild() ) )
    {
        item->SetNamed( entity );
        LongNameHelper::SetItemLongName( entity, *item );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::ValueChanged
// Created: LGY 2011-09-13
// -----------------------------------------------------------------------------
void UserProfileUnitRights::ValueChanged( const Entity_ABC* entity, bool isWriteable )
{
    if( entity->GetTypeName() == "automat" || entity->GetTypeName() == "party" )
        emit ProfiledChanged( entity, isWriteable );
    else
    {
        tools::Iterator< const Entity_ABC& > children = entity->Get< TacticalHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            const Entity_ABC& child = children.NextElement();
            ValueChanged( &child, isWriteable );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::OnProfiledChanged
// Created: LGY 2011-09-14
// -----------------------------------------------------------------------------
void UserProfileUnitRights::OnProfiledChanged( const Entity_ABC* entity, bool isReadable, bool isWriteable )
{
    if( entity )
    {
        ValuedListItem* item = FindItem( entity, firstChild() );
        if( item )
            SetStatus( item, isReadable, isWriteable, false, false );
    }
}
