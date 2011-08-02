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
#include "gaming/UserProfile.h"
#include "clients_kernel/Controllers.h"

namespace
{
    class ProfileItem : public Q3ListBoxText
    {
    public:
        ProfileItem( Q3ListBox* parent, const UserProfile& profile )
            : Q3ListBoxText( parent, profile.GetLogin() )
            , profile_( &profile )
        {}
        const UserProfile* profile_;
    };
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles constructor
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
ChatProfiles::ChatProfiles( QWidget* parent, kernel::Controllers& controllers )
    : Q3ListBox( parent )
    , controllers_( controllers )
{
    connect( this, SIGNAL( selected( Q3ListBoxItem* ) ), SLOT( OnSelected( Q3ListBoxItem* ) ) );
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
    Q3ListBoxItem* item = findItem( profile.GetLogin() );
    if( !item )
        new ProfileItem( this, profile );
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles::NotifyDeleted
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatProfiles::NotifyDeleted( const UserProfile& profile )
{
    Q3ListBoxItem* item = findItem( profile.GetLogin() );
    if( isSelected( item ) )
        setSelected( item, false );
    delete item;
}

// -----------------------------------------------------------------------------
// Name: ChatProfiles::OnSelected
// Created: SBO 2008-06-11
// -----------------------------------------------------------------------------
void ChatProfiles::OnSelected( Q3ListBoxItem* item )
{
    ProfileItem* profileItem = dynamic_cast< ProfileItem* >( item );
    if( profileItem )
        emit Selected( *profileItem->profile_ );
}
