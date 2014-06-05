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
#include "clients_kernel/Tools.h"
#include "clients_kernel/UserProfile_ABC.h"
#include "preparation/ProfileSelection.h"
#include "clients_gui/RichGroupBox.h"
#include "clients_gui/RichTreeView.h"

Q_DECLARE_METATYPE( const kernel::UserProfile_ABC* )

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage constructor
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
ScoreProfilesPage::ScoreProfilesPage( QWidget* parent, kernel::Controllers& controllers )
    : Q3VBox( parent )
    , controllers_( controllers )
{
    gui::SubObjectName subObject( "ScoreProfilesPage" );
    setMargin( 5 );
    {
        //label
        QLabel* scoreProfilesLabel = new QLabel( tools::translate( "ScoreProfilesPage", "Selected profiles will have access to the score during the exercise." ) );
        //tree view
        gui::RichTreeView* scoreProfilesView = new gui::RichTreeView( "scoreProfilesView" );
        scoreProfilesView->setRootIsDecorated( false );
        scoreProfilesView->setEditTriggers( 0 );
        model_ = new QStandardItemModel( this );
        scoreProfilesView->setModel( model_ );
        model_->setHorizontalHeaderLabels( QStringList( tools::translate( "ScoreProfilesPage", "Profile" ) ) );

        //group box parent
        gui::RichGroupBox* scoreProfilesBox = new gui::RichGroupBox( "scoreProfilesBox", tools::translate( "ScoreProfilesPage", "Access rights" ), this );
        QVBoxLayout* scoreProfileLayout = new QVBoxLayout( scoreProfilesBox );
        scoreProfileLayout->addWidget( scoreProfilesLabel );
        scoreProfileLayout->addWidget( scoreProfilesView );
        scoreProfileLayout->setMargin( 5 );
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
void ScoreProfilesPage::NotifyCreated( const kernel::UserProfile_ABC& profile )
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
void ScoreProfilesPage::NotifyUpdated( const kernel::UserProfile_ABC& profile )
{
    for( int i = 0; i < model_->rowCount(); ++i )
    {
        QStandardItem* item = model_->item( i );
        if( item->data().value< const kernel::UserProfile_ABC* >() == &profile )
            item->setText( profile.GetLogin() );
    }
}

// -----------------------------------------------------------------------------
// Name: ScoreProfilesPage::NotifyDeleted
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void ScoreProfilesPage::NotifyDeleted( const kernel::UserProfile_ABC& profile )
{
    for( int i = 0; i < model_->rowCount(); ++i )
    {
        QStandardItem* item = model_->item( i );
        if( item->data().value< const kernel::UserProfile_ABC* >() == &profile )
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
std::unique_ptr< ProfileSelection > ScoreProfilesPage::CreateResult() const
{
    std::unique_ptr< ProfileSelection > selection( new ProfileSelection( controllers_ ) );
    for( int i = 0; i < model_->rowCount(); ++i )
    {
        QStandardItem* item = model_->item( i );
        if( item->data( Qt::CheckStateRole ).toBool() )
        {
            const kernel::UserProfile_ABC* profile = item->data().value< const kernel::UserProfile_ABC* >();
            selection->Register( profile->GetLogin(), *profile );
        }
    }
    return selection;
}
