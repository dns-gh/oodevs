// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SuccessFactorProfileList.h"
#include "clients_kernel/Controllers.h"
#include "preparation/ProfileSelection.h"
#include "preparation/UserProfile.h"

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfileList constructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorProfileList::SuccessFactorProfileList( QWidget* parent, kernel::Controllers& controllers )
    : QListBox( parent )
    , controllers_( controllers )
{
    setSelectionMode( QListBox::Multi );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfileList destructor
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
SuccessFactorProfileList::~SuccessFactorProfileList()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfileList::StartEdit
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorProfileList::StartEdit( const ProfileSelection& profiles )
{
    clearSelection();
    tools::Iterator< const UserProfile& > it( profiles.CreateIterator() );
    while( it.HasMoreElements() )
        Select( it.NextElement() );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfileList::CommitTo
// Created: SBO 2009-06-17
// -----------------------------------------------------------------------------
void SuccessFactorProfileList::CommitTo( ProfileSelection& profiles ) const
{
    profiles.Clear();
    for( unsigned int i = 0; i < count() && i < profiles_.size(); ++i )
        if( isSelected( i ) )
            if( const UserProfile* profile = profiles_.at( i ) )
                profiles.Register( profile->GetLogin(), *profile );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfileList::Select
// Created: SBO 2009-06-16
// -----------------------------------------------------------------------------
void SuccessFactorProfileList::Select( const UserProfile& profile )
{
    T_Profiles::iterator it = std::find( profiles_.begin(), profiles_.end(), &profile );
    if( it != profiles_.end() )
        setSelected( static_cast< int >( std::distance( profiles_.begin(), it ) ), true );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfileList::NotifyCreated
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorProfileList::NotifyCreated( const UserProfile& profile )
{
    profiles_.push_back( &profile );
    insertItem( profile.GetLogin() );
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfileList::NotifyUpdated
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorProfileList::NotifyUpdated( const UserProfile& profile )
{
    T_Profiles::iterator it = std::find( profiles_.begin(), profiles_.end(), &profile );
    if( it != profiles_.end() )
    {
        const int index = static_cast< int >( std::distance( profiles_.begin(), it ) );
        if( text( index ) != profile.GetLogin() )
            changeItem( profile.GetLogin(), index );
    }
}

// -----------------------------------------------------------------------------
// Name: SuccessFactorProfileList::NotifyDeleted
// Created: SBO 2009-06-15
// -----------------------------------------------------------------------------
void SuccessFactorProfileList::NotifyDeleted( const UserProfile& profile )
{
    T_Profiles::iterator it = std::find( profiles_.begin(), profiles_.end(), &profile );
    if( it != profiles_.end() )
    {
        removeItem( static_cast< int >( std::distance( profiles_.begin(), it ) ) );
        profiles_.erase( it );
    }
}
