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
#include "NewProfileDialog.h"
#include "ProfilesChecker_ABC.h"
#include "preparation/ProfilesModel.h"
#include "preparation/UserProfile.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Controller.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichWidget.h"

// -----------------------------------------------------------------------------
// Name: UserProfileList constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileList::UserProfileList( QWidget* parent, UserProfileWidget& pages, kernel::Controllers& controllers,
                                  ProfilesModel& model, ProfilesChecker_ABC& checker )
    : QWidget( parent )
    , controllers_      ( controllers )
    , pages_            ( pages )
    , model_            ( model )
    , checker_          ( checker )
    , pNewProfileDialog_( new NewProfileDialog( this, model, checker ) )
{
    gui::SubObjectName subObject( "UserProfileList" );
    QVBoxLayout* layout = new QVBoxLayout( this );

    dataModel_ = new QStandardItemModel();
    dataModel_->setColumnCount( 1 );
    proxyModel_ = new QSortFilterProxyModel();
    proxyModel_->setDynamicSortFilter( true );
    proxyModel_->setSortCaseSensitivity( Qt::CaseInsensitive );
    proxyModel_->setSourceModel( dataModel_ );
    list_ = new gui::RichWidget< QListView >( "list", this );
    list_->setViewMode( gui::RichWidget< QListView >::ListMode );
    list_->setSelectionMode( QAbstractItemView::SingleSelection );
    list_->setModel( proxyModel_ );

    QHBoxLayout* box = new QHBoxLayout();
    gui::RichPushButton* createBtn = new gui::RichPushButton( "create", tr( "Create" ) );
    gui::RichPushButton* deleteBtn = new gui::RichPushButton( "delete", tr( "Delete" ) );
    connect( createBtn, SIGNAL( clicked() ), SLOT( OnCreate() ) );
    connect( deleteBtn, SIGNAL( clicked() ), SLOT( OnDelete() ) );
    connect( list_->selectionModel(), SIGNAL( selectionChanged( const QItemSelection&, const QItemSelection& ) ), this, SLOT( OnSelectionChanged() ) );
    box->addWidget( createBtn );
    box->addWidget( deleteBtn );

    layout->addWidget( list_ );
    layout->addLayout( box );
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
    for( auto it = editors_.begin(); it != editors_.end(); ++it )
    {
        *const_cast< UserProfile* >( it->first ) = *it->second;
        controllers_.controller_.Update( it->first );
    }
    checker_.Clean();
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::Cancel
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileList::Cancel()
{
    for( auto it = editors_.begin(); it != editors_.end(); ++it )
        delete it->second;
    editors_.clear();
    checker_.Clean();
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnCurrentChanged
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnSelectionChanged()
{
    QModelIndexList indexes = list_->selectionModel()->selectedIndexes();
    if( isVisible() && !indexes.empty() )
    {
        if( const UserProfile* profile = profiles_.at( proxyModel_->mapToSource( indexes.front() ).row() ) )
        {
            UserProfile*& editor = editors_[ profile ];
            if( !editor )
                editor = new ProfileEditor( *profile, controllers_.controller_ );
            checker_.Display( editors_ );
            pages_.Display( *editor );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnCreate
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnCreate()
{
    pNewProfileDialog_->Exec();
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnDelete
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnDelete()
{
    QModelIndexList indexes = list_->selectionModel()->selectedIndexes();
    if( !indexes.empty() )
        if( const UserProfile* profile = profiles_.at( proxyModel_->mapToSource( indexes.front() ).row() ) )
            model_.DeleteProfile( *profile );
}

namespace
{
    QStandardItem* CreateItem( const QString& text )
    {
        QStandardItem* item = new QStandardItem( text );
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::NotifyCreated
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::NotifyCreated( const UserProfile& profile )
{
    profiles_.push_back( &profile );
    int count = dataModel_->rowCount();
    dataModel_->setItem( count, 0, CreateItem( profile.GetLogin() ) );
    proxyModel_->sort( 0 );
    if( isShown() )
    {
        list_->selectionModel()->clear();
        list_->selectionModel()->select( proxyModel_->mapFromSource( dataModel_->index( count, 0 ) ), QItemSelectionModel::Select );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::NotifyUpdated
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::NotifyUpdated( const UserProfile& profile )
{
    const UserProfile* updated = &profile;
    auto editorIt = editors_.find( &profile );
    if( editorIt != editors_.end() )
        updated = editorIt->second;

    auto it = std::find( profiles_.begin(), profiles_.end(), &profile );
    if( it != profiles_.end() )
    {
        const int index = static_cast< int >( std::distance( profiles_.begin(), it ) );
        QStandardItem* item = dataModel_->item( index );
        if( item && item->text() != updated->GetLogin() )
            item->setText( updated->GetLogin() );
    }
    proxyModel_->sort( 0 );
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
        dataModel_->takeRow( index );
        profiles_.erase( it );
        T_ProfileEditors::iterator editorIt = editors_.find( &profile );
        if( editorIt != editors_.end() )
        {
            delete editorIt->second;
            editors_.erase( editorIt );
        }
        checker_.Display( editors_ );
        pages_.setVisible( !profiles_.empty() );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::showEvent
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileList::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );
    if( dataModel_->rowCount() > 0 )
        pages_.setVisible( true );
    list_->selectionModel()->clear();
    list_->selectionModel()->select( proxyModel_->index( 0, 0 ), QItemSelectionModel::Select );
}
