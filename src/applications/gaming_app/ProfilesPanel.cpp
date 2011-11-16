// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "ProfilesPanel.h"
#include "moc_ProfilesPanel.cpp"
#include "gaming/Profile.h"
#include "gaming/UserProfile.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: ProfilesPanel constructor
// Created: LGY 2011-11-15
// -----------------------------------------------------------------------------
ProfilesPanel::ProfilesPanel( QMainWindow* mainWindow, kernel::Controllers& controllers )
    : QDockWidget( "Profiles", mainWindow )
    , controllers_( controllers )
    , star_       ( "resources/images/gaming/star.png")
    , profile_    ( "resources/images/gaming/current.png")
{
    setObjectName( "ProfilesPanel" );
    QWidget* main = new QWidget( this );
    QVBoxLayout* mainLayout = new QVBoxLayout( main );
    setCaption( tools::translate( "Profiles", "Profiles" ) );
    dataModel_ = new QStandardItemModel();
    dataModel_->setColumnCount( 3 );

    proxyModel_ = new QSortFilterProxyModel();
    proxyModel_->setDynamicSortFilter( true );
    proxyModel_->setSortCaseSensitivity( Qt::CaseInsensitive );
    proxyModel_->setSourceModel( dataModel_ );

    tableView_ = new QTableView();
    tableView_->setModel( proxyModel_ );
    tableView_->setSortingEnabled( true );
    tableView_->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView_->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView_->verticalHeader()->setVisible( false );
    tableView_->horizontalHeader()->setVisible( false );
    tableView_->setAlternatingRowColors( true );
    tableView_->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    tableView_->setColumnWidth( 0, 20 );
    tableView_->setColumnWidth( 2, 25 );
    mainLayout->addWidget( tableView_ );
    setWidget( main );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel destructor
// Created: LGY 2011-11-15
// -----------------------------------------------------------------------------
ProfilesPanel::~ProfilesPanel()
{
    controllers_.Unregister( *this );
}

namespace
{
    QStandardItem* CreateItem()
    {
        QStandardItem* item = new QStandardItem();
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::NotifyCreated
// Created: LGY 2011-11-15
// -----------------------------------------------------------------------------
void ProfilesPanel::NotifyCreated( const UserProfile& profile )
{
    profiles_.push_back( &profile );
    const int rows = dataModel_->rowCount();
    dataModel_->setItem( rows, 0, CreateItem() );
    dataModel_->setItem( rows, 1, CreateItem() );
    dataModel_->setItem( rows, 2, CreateItem() );
    proxyModel_->sort( 1 );
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::NotifyUpdated
// Created: LGY 2011-11-15
// -----------------------------------------------------------------------------
void ProfilesPanel::NotifyUpdated( const UserProfile& profile )
{
    T_UserProfiles::iterator it = std::find( profiles_.begin(), profiles_.end(), &profile );
    if( it != profiles_.end() )
    {
        const int index = static_cast< int >( std::distance( profiles_.begin(), it ) );
        QStandardItem* active = dataModel_->item( index, 0 );
        if( active )
            active->setIcon( profile.GetLogin() == current_ ? profile_ : QIcon( "" ) );
        QStandardItem* item = dataModel_->item( index, 1 );
        if( item && item->text() != profile.GetLogin() )
            item->setText( profile.GetLogin() );
        QStandardItem* admin = dataModel_->item( index, 2 );
        if( admin )
            admin->setIcon( profile.IsSupervisor() ? star_ : QIcon( "" ) );
        proxyModel_->sort( 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::NotifyDeleted
// Created: LGY 2011-11-15
// -----------------------------------------------------------------------------
void ProfilesPanel::NotifyDeleted( const UserProfile& profile )
{
    T_UserProfiles::iterator it = std::find( profiles_.begin(), profiles_.end(), &profile );
    if( it != profiles_.end() )
    {
        const int index = static_cast< int >( std::distance( profiles_.begin(), it ) );
        dataModel_->takeRow( index );
        profiles_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::NotifyUpdated
// Created: LGY 2011-11-16
// -----------------------------------------------------------------------------
void ProfilesPanel::NotifyUpdated( const Profile& profile )
{
    if( profile.IsLoggedIn() )
        current_ = profile.GetLogin();
}
