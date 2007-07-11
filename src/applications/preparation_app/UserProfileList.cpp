// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "UserProfileList.h"
#include "moc_UserProfileList.cpp"
#include "UserProfileWidget.h"
#include "preparation/UserProfile.h"
#include "preparation/ProfilesModel.h"
#include "clients_kernel/Controllers.h"
#include <qlistbox.h>

// -----------------------------------------------------------------------------
// Name: UserProfileList constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileList::UserProfileList( QWidget* parent, UserProfileWidget& pages, kernel::Controllers& controllers, ProfilesModel& model )
    : QVBox( parent )
    , controllers_( controllers )
    , pages_( pages )
    , model_( model )
{
    list_ = new QListBox( this );
    QHBox* box = new QHBox( this );
    QButton* createBtn = new QPushButton( tr( "Create" ), box );
    QButton* deleteBtn = new QPushButton( tr( "Delete" ), box );
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
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnCreate
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnCreate()
{
    model_.CreateProfile();
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnDelete
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnDelete()
{
    if( list_->currentItem() != -1 )
        model_.DeleteProfile( *userProfiles_.at( list_->currentItem() ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::NotifyCreated
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::NotifyCreated( const UserProfile& profile )
{
    userProfiles_.push_back( &profile );
    const bool itemSelected = list_->selectedItem();
    list_->insertItem( profile.GetLogin() );
    if( !itemSelected )
    {
        list_->setSelected( 0, true );
        pages_.SetEnabled( true );
    }
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
        const int index = std::distance( userProfiles_.begin(), it );
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
        const int index = std::distance( userProfiles_.begin(), it );
        const bool selected = list_->isSelected( index );
        list_->removeItem( index );
        userProfiles_.erase( it );
        if( selected && list_->count() )
            list_->setSelected( list_->item( index ) ? index : index - 1, true );
        pages_.SetEnabled( !userProfiles_.empty() );
    }
}
