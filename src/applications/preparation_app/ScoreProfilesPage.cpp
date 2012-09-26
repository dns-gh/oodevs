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

Q_DECLARE_METATYPE( const UserProfile* )

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
        QTreeView* view = new QTreeView( box );
        view->setRootIsDecorated( false );
        view->setEditTriggers( 0 );
        model_ = new QStandardItemModel( this );
        view->setModel( model_ );
        model_->setHorizontalHeaderLabels( QStringList( tr( "Profile" ) ) );
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
    QStandardItem* item = new QStandardItem( profile.GetLogin() );
    item->setData( true, Qt::CheckStateRole );
    item->setData( QVariant::fromValue( &profile ) );
    item->setCheckable( true );
    model_->appendRow( item );
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage::NotifyUpdated
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void ScoreProfilesPage::NotifyUpdated( const UserProfile& profile )
{
    for( int i = 0; i < model_->rowCount(); ++i )
    {
        QStandardItem* item = model_->item( i );
        if( item->data().value< const UserProfile* >() == &profile )
            item->setText( profile.GetLogin() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage::NotifyDeleted
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void ScoreProfilesPage::NotifyDeleted( const UserProfile& profile )
{
    for( int i = 0; i < model_->rowCount(); ++i )
    {
        QStandardItem* item = model_->item( i );
        if( item->data().value< const UserProfile* >() == &profile )
        {
            model_->removeRow( i );
            return;;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage::StartEdit
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void ScoreProfilesPage::StartEdit( const ProfileSelection& profiles )
{
    for( int i = 0; i < model_->rowCount(); ++i )
    {
        QStandardItem* item = model_->item( i );
        item->setData( profiles.Find( item->text() ) != 0, Qt::CheckStateRole );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage::CreateResult
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
std::auto_ptr< ProfileSelection > ScoreProfilesPage::CreateResult() const
{
    std::auto_ptr< ProfileSelection > selection( new ProfileSelection( controllers_ ) );
    for( int i = 0; i < model_->rowCount(); ++i )
    {
        QStandardItem* item = model_->item( i );
        if( item->data( Qt::CheckStateRole ).toBool() )
        {
            const UserProfile* profile = item->data().value< const UserProfile* >();
            selection->Register( profile->GetLogin(), *profile );
        }
    }
    return selection;
}
