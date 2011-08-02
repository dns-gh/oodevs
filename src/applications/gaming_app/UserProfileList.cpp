// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UserProfileList.h"
#include "moc_UserProfileList.cpp"
#include "UserProfileWidget.h"
#include "gaming/UserProfile.h"
#include "gaming/UserProfileFactory_ABC.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: UserProfileList constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileList::UserProfileList( QWidget* parent, UserProfileWidget& pages, kernel::Controllers& controllers, UserProfileFactory_ABC& factory )
    : Q3VBox( parent, "UserProfileList" )
    , controllers_( controllers )
    , pages_( pages )
    , factory_( factory )
{
    list_ = new Q3ListBox( this );
    Q3HBox* box = new Q3HBox( this );
    QPushButton* createBtn = new QPushButton( tr( "Create" ), box );
    QPushButton* deleteBtn = new QPushButton( tr( "Delete" ), box );
    connect( createBtn, SIGNAL( clicked() ), SLOT( OnCreate() ) );
    connect( deleteBtn, SIGNAL( clicked() ), SLOT( OnDelete() ) );
    connect( list_, SIGNAL( selectionChanged() ), SLOT( OnSelectionChanged() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfileList destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileList::~UserProfileList()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnCurrentChanged
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnSelectionChanged()
{
    if( list_->currentItem() != -1 )
        pages_.Display( *userProfiles_.at( list_->currentItem() ) );
    else
        pages_.Clean();
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnCreate
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnCreate()
{
    factory_.Create();
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnDelete
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnDelete()
{
    const int index = list_->currentItem();
    if( index != -1 )
        const_cast< UserProfile* >( userProfiles_.at( index ) )->RequestDeletion();
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::NotifyCreated
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::NotifyCreated( const UserProfile& profile )
{
    userProfiles_.push_back( &profile );
    list_->insertItem( profile.GetLogin() );
    if( list_->currentItem() == -1 )
        list_->setSelected( 0, true );
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::NotifyUpdated
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::NotifyUpdated( const UserProfile& profile )
{
    T_UserProfiles::iterator it = std::find( userProfiles_.begin(), userProfiles_.end(), &profile );
    if( it != userProfiles_.end() )
    {
        const int index = static_cast< int >( std::distance( userProfiles_.begin(), it ) );
        if( list_->text( index ) != profile.GetLogin() )
            list_->changeItem( profile.GetLogin(), index );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::NotifyDeleted
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::NotifyDeleted( const UserProfile& profile )
{
    T_UserProfiles::iterator it = std::find( userProfiles_.begin(), userProfiles_.end(), &profile );
    if( it != userProfiles_.end() )
    {
        const int index = static_cast< int >( std::distance( userProfiles_.begin(), it ) );
        list_->removeItem( index );
        userProfiles_.erase( it );
        OnSelectionChanged();
    }
}
