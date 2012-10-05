// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UserProfileUnitRights.h"
#include "moc_UserProfileUnitRights.cpp"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/Entity_ABC.h"

#pragma warning( disable : 4355 ) // $$$$ SBO 2008-05-14: 'this' : used in base member initializer list

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileUnitRights::UserProfileUnitRights( QWidget* parent, kernel::Controllers& controllers, const gui::EntitySymbols& icons, const QString& name, const kernel::Profile_ABC& profile )
    : HierarchyTreeView< kernel::TacticalHierarchies >( controllers, profile, observer_, icons, parent )
    , UserProfileRights_ABC( *this, dataModel_, name )
{
    controllers_.Update( *this );
    connect( this, SIGNAL( clicked( const QModelIndex& ) ), SLOT( OnItemClicked( const QModelIndex& ) ) );
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
// Name: UserProfileUnitRights::showEvent
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileUnitRights::showEvent( QShowEvent* event )
{
    OnShow();
    T_Parent::showEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::hideEvent
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
void UserProfileUnitRights::hideEvent( QHideEvent* event )
{
    OnHide();
    T_Parent::hideEvent( event );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::OnItemClicked
// Created: SBO 2007-01-17
// -----------------------------------------------------------------------------
void UserProfileUnitRights::OnItemClicked( const QModelIndex& index )
{
    UserProfileRights_ABC::OnItemClicked( index );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::NeedsCommit
// Created: SBO 2007-03-26
// -----------------------------------------------------------------------------
bool UserProfileUnitRights::NeedsCommit() const
{
    return isVisible();
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::AdditionalUpdateItem
// Created: JSR 2012-10-05
// -----------------------------------------------------------------------------
void UserProfileUnitRights::AdditionalUpdateItem( QStandardItem& entityItem, const kernel::Entity_ABC& entity )
{
    gui::longname::SetItemLongName( entity, entityItem );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::NotifyUpdated
// Created: SBO 2008-08-26
// -----------------------------------------------------------------------------
void UserProfileUnitRights::NotifyUpdated( const kernel::Entity_ABC& entity )
{
    QStandardItem* item = dataModel_.FindDataItem( entity );
    if( item )
    {
        gui::longname::SetItemLongName( entity, *item);
        proxyModel_->invalidate();
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::contextMenuEvent
// Created: JSR 2012-06-22
// -----------------------------------------------------------------------------
void UserProfileUnitRights::contextMenuEvent( QContextMenuEvent* /*event*/ )
{
    // NOTHING
}
