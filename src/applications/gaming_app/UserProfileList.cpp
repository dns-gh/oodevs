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
    : QWidget( parent )
    , controllers_( controllers )
    , pages_      ( pages )
    , factory_    ( factory )
{
    QVBoxLayout* layout = new QVBoxLayout( this );

    dataModel_ = new QStandardItemModel();
    dataModel_->setColumnCount( 1 );
    proxyModel_ = new QSortFilterProxyModel();
    proxyModel_->setDynamicSortFilter( true );
    proxyModel_->setSortCaseSensitivity( Qt::CaseInsensitive );
    proxyModel_->setSourceModel( dataModel_ );
    list_ = new QListView( this );
    list_->setViewMode( QListView::ListMode );
    list_->setSelectionMode( QAbstractItemView::SingleSelection );
    list_->setModel( proxyModel_ );

    QHBoxLayout* box = new QHBoxLayout();
    QPushButton* createBtn = new QPushButton( tr( "Create" ) );
    QPushButton* deleteBtn = new QPushButton( tr( "Delete" ) );
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
// Name: UserProfileList::OnCurrentChanged
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnSelectionChanged()
{
    QModelIndexList indexes = list_->selectionModel()->selectedIndexes();
    if( !indexes.empty() )
    {
        if( const UserProfile* profile = userProfiles_.at( proxyModel_->mapToSource( indexes.front() ).row() ) )
            pages_.Display( *profile );
    }
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
    QModelIndexList indexes = list_->selectionModel()->selectedIndexes();
    if( !indexes.empty() )
        if( const UserProfile* profile = userProfiles_.at( proxyModel_->mapToSource( indexes.front() ).row() ) )
            const_cast< UserProfile* >( profile )->RequestDeletion();
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
    userProfiles_.push_back( &profile );
    dataModel_->setItem( dataModel_->rowCount(), 0, CreateItem( profile.GetLogin() ) );
    proxyModel_->sort( 0 );
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
        QStandardItem* item = dataModel_->item( index );
        if( item && item->text() != profile.GetLogin() )
            item->setText( profile.GetLogin() );
    }
    proxyModel_->sort( 0 );
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
        dataModel_->takeRow( index );
        userProfiles_.erase( it );
        OnSelectionChanged();
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::showEvent
// Created: LGY 2011-12-01
// -----------------------------------------------------------------------------
void UserProfileList::showEvent( QShowEvent* event )
{
    QWidget::showEvent( event );
    list_->selectionModel()->select( proxyModel_->index( 0, 0 ), QItemSelectionModel::Select );
}
