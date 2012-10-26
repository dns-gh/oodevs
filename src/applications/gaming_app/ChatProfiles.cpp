// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ChatProfiles.h"
#include "moc_ChatProfiles.cpp"
#include "gaming/UserProfile.h"
#include "clients_kernel/Controllers.h"

namespace
{
    class ProfileItem : public QListWidgetItem
    {
    public:
        ProfileItem( QListWidget* parent, const UserProfile& profile )
            : QListWidgetItem( profile.GetLogin() )
            , profile_( &profile )
        {
            parent->addItem( this );
        }
        const UserProfile* profile_;
    };
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles constructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
ChatProfiles::ChatProfiles( QWidget* parent, kernel::Controllers& controllers )
    : QListWidget( parent )
    , controllers_( controllers )
{
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
// Name: ChatProfiles::NotifyUpdated
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatProfiles::NotifyUpdated( const UserProfile& profile )
{
    if( findItems( profile.GetLogin(), Qt::MatchFixedString ).isEmpty() )
        new ProfileItem( this, profile );
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles::NotifyDeleted
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatProfiles::NotifyDeleted( const UserProfile& profile )
{
    if( !findItems( profile.GetLogin(), Qt::MatchFixedString ).isEmpty() )
        if( QListWidgetItem* item = findItems( profile.GetLogin(), Qt::MatchFixedString ).at( 0 ) )
        {
            setItemSelected( item, false );
            delete item;
        }
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles::OnSelected
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatProfiles::OnSelected( QListWidgetItem* item )
{
    ProfileItem* profileItem = dynamic_cast< ProfileItem* >( item );
    if( profileItem )
        emit Selected( *profileItem->profile_ );
}
