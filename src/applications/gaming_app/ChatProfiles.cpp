// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChatProfiles.h"
#include "moc_ChatProfiles.cpp"
#include "clients_kernel/UserProfile_ABC.h"
#include "clients_kernel/Controllers.h"

Q_DECLARE_METATYPE( const kernel::UserProfile_ABC* );

// -----------------------------------------------------------------------------
// Name: ChatProfiles constructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
ChatProfiles::ChatProfiles( QWidget* parent, kernel::Controllers& controllers )
    : QListWidget( parent )
    , controllers_( controllers )
{
    setSortingEnabled( true );
    connect( this, SIGNAL( itemDoubleClicked( QListWidgetItem* ) ), SLOT( OnSelected( QListWidgetItem* ) ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles destructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
ChatProfiles::~ChatProfiles()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles::FindItem
// Created: JSR 2014-06-06
// -----------------------------------------------------------------------------
QListWidgetItem* ChatProfiles::FindItem( const kernel::UserProfile_ABC& profile ) const
{
    for( int i = 0; i < count(); ++i )
    {
        QListWidgetItem* listItem = item( i );
        if( &profile == listItem->data( Qt::UserRole ).value< const kernel::UserProfile_ABC* >() )
            return listItem;
    }
    return 0;
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles::NotifyUpdated
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatProfiles::NotifyUpdated( const kernel::UserProfile_ABC& profile )
{
    QListWidgetItem* item = FindItem( profile );
    if( !item )
    {
        item = new QListWidgetItem( this );
        item->setData( Qt::UserRole, QVariant::fromValue( &profile ) );
    }
    item->setText( profile.GetLogin() );
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles::NotifyDeleted
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatProfiles::NotifyDeleted( const kernel::UserProfile_ABC& profile )
{
    QListWidgetItem* item = FindItem( profile );
    if( item )
        setItemSelected( item, false );
    delete item;
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles::OnSelected
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatProfiles::OnSelected( QListWidgetItem* item )
{
    if( item )
        emit Selected( *item->data( Qt::UserRole ).value< const kernel::UserProfile_ABC* >() );
}
