// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UserProfileUnitRights.h"
#include "moc_UserProfileUnitRights.cpp"
#include "LongNameHelper.h"
#include "Tools.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileUnitRights::UserProfileUnitRights( const QString& objectName, QWidget* parent, kernel::Controllers& controllers,
                                              const EntitySymbols& icons, const QString& name, const kernel::Profile_ABC& profile )
    : T_Parent( objectName, controllers, profile, observer_, icons, parent )
    , UserProfileRights_ABC( *this, dataModel_, name )
{
    connect( this, SIGNAL( clicked( const QModelIndex& ) ), SLOT( OnItemClicked( const QModelIndex& ) ) );
    SetLessThanEntityFunctor( &tools::LessThanByPC );
    controllers_.Update( *this );
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
// Name: UserProfileUnitRights::GetWidget
// Created: JSR 2014-05-27
// -----------------------------------------------------------------------------
QWidget* UserProfileUnitRights::GetWidget()
{
    return this;
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::Display
// Created: LGY 2011-12-09
// -----------------------------------------------------------------------------
void UserProfileUnitRights::Display( kernel::UserProfile_ABC& profile )
{
    selectionModel()->clearSelection();
    UserProfileRights_ABC::Display( profile );
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::AdditionalUpdateItem
// Created: JSR 2012-09-25
// -----------------------------------------------------------------------------
void UserProfileUnitRights::AdditionalUpdateItem( QStandardItem& entityItem, const kernel::Entity_ABC& entity )
{
    longname::SetItemLongName( entity, entityItem );
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
// Name: UserProfileUnitRights::OnSelect
// Created: NPT 2012-12-10
// -----------------------------------------------------------------------------
void UserProfileUnitRights::OnSelect( const QItemSelection& , const QItemSelection&  )
{
    //NOTHING
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
        longname::SetItemLongName( entity, *item);
        proxyModel_->invalidate();
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::contextMenuEvent
// Created: JSR 2012-09-25
// -----------------------------------------------------------------------------
void UserProfileUnitRights::contextMenuEvent( QContextMenuEvent* /*event*/ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileUnitRights::Commit
// Created: JSR 2014-06-03
// -----------------------------------------------------------------------------
void UserProfileUnitRights::Commit()
{
    UserProfileRights_ABC::Commit();
    emit NotifyRightsChanged();
}
