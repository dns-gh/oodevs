// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ScoreProfilesPage.h"
#include "clients_kernel/Controllers.h"
#include "preparation/ProfileSelection.h"
#include "preparation/UserProfile.h"

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage constructor
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
ScoreProfilesPage::ScoreProfilesPage( QWidget* parent, kernel::Controllers& controllers )
    : Q3VBox( parent )
    , controllers_( controllers )
{
    setMargin( 5 );
    {
        Q3GroupBox* box = new Q3GroupBox( 1, Qt::Horizontal, tr( "Access rights" ), this );
        box->setMargin( 5 );
        new QLabel( tr( "Selected profiles will have access to the score during the exercise." ), box );
        list_ = new Q3ListView( box );
        list_->addColumn( tr( "Profile" ) );
    }
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage destructor
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
ScoreProfilesPage::~ScoreProfilesPage()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage::NotifyCreated
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void ScoreProfilesPage::NotifyCreated( const UserProfile& profile )
{
    Q3CheckListItem* item = new Q3CheckListItem( list_, profile.GetLogin(), Q3CheckListItem::CheckBox );
    item->setOn( true );
    list_->insertItem( item );
    profiles_[ &profile ] = item;
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage::NotifyUpdated
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void ScoreProfilesPage::NotifyUpdated( const UserProfile& profile )
{
    T_Profiles::iterator it = profiles_.find( &profile );
    if( it != profiles_.end() )
        if( it->second->text( 0 ) != profile.GetLogin() )
            it->second->setText( 0, profile.GetLogin() );
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage::NotifyDeleted
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void ScoreProfilesPage::NotifyDeleted( const UserProfile& profile )
{
    T_Profiles::iterator it = profiles_.find( &profile );
    if( it != profiles_.end() )
    {
        if( Q3ListViewItem* item = list_->findItem( profile.GetLogin(), 0 ) )
            list_->removeItem( item );
        profiles_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage::StartEdit
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void ScoreProfilesPage::StartEdit( const ProfileSelection& profiles )
{
    for( T_Profiles::iterator it = profiles_.begin(); it != profiles_.end(); ++it )
        it->second->setOn( profiles.Find( it->first->GetLogin() ) != 0 );
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage::CreateResult
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
std::auto_ptr< ProfileSelection > ScoreProfilesPage::CreateResult() const
{
    std::auto_ptr< ProfileSelection > selection( new ProfileSelection( controllers_ ) );
    for( T_Profiles::const_iterator it = profiles_.begin(); it != profiles_.end(); ++it )
        if( it->second->isOn() )
            selection->Register( it->first->GetLogin(), *it->first );
    return selection;
}
