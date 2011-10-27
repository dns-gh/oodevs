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
#include "ProfileEditor.h"
#include "ControlsChecker_ABC.h"
#include "preparation/ModelConsistencyChecker.h"
#include "preparation/ProfilesModel.h"
#include "preparation/UserProfile.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"

// -----------------------------------------------------------------------------
// Name: UserProfileList constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileList::UserProfileList( QWidget* parent, UserProfileWidget& pages, kernel::Controllers& controllers,
                                  ProfilesModel& model, ControlsChecker_ABC& checker )
    : Q3VBox( parent, "UserProfileList" )
    , controllers_( controllers )
    , pages_      ( pages )
    , model_      ( model )
    , checker_    ( checker )
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
// Name: UserProfileList::Save
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileList::Save()
{
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
    {
        *const_cast< UserProfile* >( it->first ) = *it->second;
        controllers_.controller_.Update( it->first );
    }
    emit( DoConsistencyCheck() );
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::Cancel
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileList::Cancel()
{
    for( CIT_ProfileEditors it = editors_.begin(); it != editors_.end(); ++it )
        delete it->second;
    editors_.clear();
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnCurrentChanged
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnSelectionChanged()
{
    if( isVisible() && list_->currentItem() != -1 )
    {
        const UserProfile* profile = profiles_.at( list_->currentItem() );
        UserProfile*& editor = editors_[profile];
        if( !editor )
            editor = new ProfileEditor( *profile, controllers_.controller_ );
        checker_.Display( editors_ );
        pages_.Display( *editor );
    }
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
        model_.DeleteProfile( *profiles_.at( list_->currentItem() ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::NotifyCreated
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::NotifyCreated( const UserProfile& profile )
{
    profiles_.push_back( &profile );
    const bool itemSelected = list_->selectedItem() != 0;
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
    const UserProfile* updated = &profile;
    CIT_ProfileEditors editorIt = editors_.find( &profile );
    if( editorIt != editors_.end() )
        updated = editorIt->second;

    T_Profiles::iterator it = std::find( profiles_.begin(), profiles_.end(), &profile );
    if( it != profiles_.end() )
    {
        const int index = static_cast< int >( std::distance( profiles_.begin(), it ) );
        if( list_->text( index ) != updated->GetLogin() )
            list_->changeItem( updated->GetLogin(), index );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::NotifyDeleted
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::NotifyDeleted( const UserProfile& profile )
{
    T_Profiles::iterator it = std::find( profiles_.begin(), profiles_.end(), &profile );
    if( it != profiles_.end() )
    {
        const int index = static_cast< int >( std::distance( profiles_.begin(), it ) );
        const bool selected = list_->isSelected( index );
        list_->removeItem( index );
        profiles_.erase( it );
        T_ProfileEditors::iterator editorIt = editors_.find( &profile );
        if( editorIt != editors_.end() )
        {
            delete editorIt->second;
            editors_.erase( editorIt );
        }
        if( selected && list_->count() )
            list_->setSelected( list_->item( index ) ? index : index - 1, true );
        pages_.SetEnabled( !profiles_.empty() );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::showEvent
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileList::showEvent( QShowEvent* event )
{
    Q3VBox::showEvent( event );
    OnSelectionChanged();
}
