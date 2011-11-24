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
#include "ReconnectLoginDialog.h"
#include "gaming/Profile.h"
#include "gaming/UserProfile.h"
#include "gaming/Network.h"
#include "gaming/AgentServerMsgMgr.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/Controllers.h"

#pragma warning( disable : 4355 )

// -----------------------------------------------------------------------------
// Name: ProfilesPanel constructor
// Created: LGY 2011-11-15
// -----------------------------------------------------------------------------
ProfilesPanel::ProfilesPanel( QMainWindow* mainWindow, kernel::Controllers& controllers, Network& network )
    : QDockWidget( "Profiles", mainWindow )
    , controllers_ ( controllers )
    , network_     ( network )
    , star_        ( "resources/images/gaming/star.png" )
    , lock_        ( "resources/images/gaming/lock.png" )
    , profile_     ( "resources/images/gaming/current.png" )
    , red_         ( "resources/images/gaming/red.png" )
    , green_       ( "resources/images/gaming/green.png" )
{
    setObjectName( "ProfilesPanel" );
    QWidget* main = new QWidget( this );
    QVBoxLayout* mainLayout = new QVBoxLayout( main );
    setCaption( tools::translate( "Profiles", "Profiles" ) );
    dataModel_ = new QStandardItemModel();
    dataModel_->setColumnCount( 6 );

    proxyModel_ = new QSortFilterProxyModel();
    proxyModel_->setDynamicSortFilter( true );
    proxyModel_->setSortCaseSensitivity( Qt::CaseInsensitive );
    proxyModel_->setSourceModel( dataModel_ );

    tableView_ = new QTableView();
    tableView_->setModel( proxyModel_ );
    tableView_->setSortingEnabled( true );
    tableView_->setAlternatingRowColors( true );
    tableView_->setSelectionBehavior( QAbstractItemView::SelectRows );
    tableView_->setSelectionMode( QAbstractItemView::SingleSelection );
    tableView_->verticalHeader()->setVisible( false );
    tableView_->horizontalHeader()->setVisible( false );
    tableView_->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    tableView_->setColumnWidth( 0, 20 );
    tableView_->setColumnWidth( 2, 25 );
    tableView_->setColumnWidth( 3, 25 );
    tableView_->setColumnWidth( 4, 25 );
    tableView_->setColumnWidth( 5, 25 );
    mainLayout->addWidget( tableView_ );

    QHBoxLayout* buttonLayout = new QHBoxLayout();
    QPushButton* reconnect = new QPushButton( "Reconnect with..." );
    QPushButton* filter = new QPushButton( "Filter view" );
    buttonLayout->addWidget( reconnect );
    buttonLayout->addWidget( filter );
    connect( reconnect, SIGNAL( clicked() ), this, SLOT( Reconnect() ) );
    connect( filter, SIGNAL( clicked() ), this, SLOT( Filter() ) );
    mainLayout->addLayout( buttonLayout );
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

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::NotifyCreated
// Created: LGY 2011-11-15
// -----------------------------------------------------------------------------
void ProfilesPanel::NotifyCreated( const UserProfile& profile )
{
    profiles_.push_back( &profile );
    AddProfile( dataModel_->rowCount() );
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
        UpdateProfile( index, **it );
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
    {
        if( current_ == "" )
            Clean();
        current_ = profile.GetLogin();
    }
    else if( current_ != "" )
    {
        Clean();
        current_ = "";
    }
    for( int i = 0; i < dataModel_->rowCount(); ++i )
        if(  QStandardItem* item = dataModel_->item( i, 1 ) )
        {
            unsigned int count = profile.GetProfileCount( item->text().ascii() );
            if( QStandardItem* connected = dataModel_->item( i, 4 ) )
                connected->setIcon( count ? green_ : red_ );
            if( QStandardItem* number = dataModel_->item( i, 5 ) )
                number->setText( "(" + QString::number( count ) + ")" );
        }
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::NotifyUpdated
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ProfilesPanel::NotifyUpdated( const AvailableProfile& profile )
{
    const int index = dataModel_->rowCount();
    AddProfile( index );
    UpdateProfile( index, profile );
    proxyModel_->sort( 1 );
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::NotifyUpdated
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
void ProfilesPanel::NotifyUpdated( const Simulation& simulation )
{
    if( !simulation.IsConnected() )
        Clean();
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::Reconnect
// Created: LGY 2011-11-21
// -----------------------------------------------------------------------------
void ProfilesPanel::Reconnect()
{
    QModelIndex index = proxyModel_->mapToSource( tableView_->currentIndex() );
    if( index.row() != -1 )
        if( const UserProfile* profile = profiles_.at( index.row() ) )
        {
            const std::string& login = profile->GetLogin().ascii();
            if( profile->IsPasswordProtected() )
            {
                ReconnectLoginDialog* pLoginDialog = new ReconnectLoginDialog( this, *profile, network_ );
                if( pLoginDialog->exec() == QDialog::Accepted )
                    Clean();
            }
            else
            {
                network_.GetMessageMgr().Reconnect( login, "" );
                Clean();
            }
        }
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::Filter
// Created: LGY 2011-11-22
// -----------------------------------------------------------------------------
void ProfilesPanel::Filter()
{
    // NOTHING
}

namespace
{
    QStandardItem* CreateItem( const std::string& text )
    {
        QStandardItem* item = new QStandardItem( text.c_str() );
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        return item;
    }

    QStandardItem* CreateItem( const QIcon& icon )
    {
        QStandardItem* item = new QStandardItem();
        item->setIcon( icon );
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        return item;
    }
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::Profile
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ProfilesPanel::AddProfile( unsigned int index )
{
    dataModel_->setItem( index, 0, CreateItem( "" ) );
    dataModel_->setItem( index, 1, CreateItem( "" ) );
    dataModel_->setItem( index, 2, CreateItem( "" ) );
    dataModel_->setItem( index, 3, CreateItem( "" ) );
    dataModel_->setItem( index, 4, CreateItem( red_ ) );
    dataModel_->setItem( index, 5, CreateItem( "(0)" ) );
    proxyModel_->sort( 1 );
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::UpdateProfile
// Created: LGY 2011-11-24
// -----------------------------------------------------------------------------
template< typename T >
void ProfilesPanel::UpdateProfile( unsigned int index, const T& profile )
{
    if( QStandardItem* active = dataModel_->item( index, 0 ) )
        active->setIcon( profile.GetLogin() == current_ ? profile_ : QIcon( "" ) );
    QStandardItem* item = dataModel_->item( index, 1 );
    if( item && item->text() != profile.GetLogin() )
        item->setText( profile.GetLogin() );
    if( QStandardItem* admin = dataModel_->item( index, 2 ) )
        admin->setIcon( profile.IsSupervisor() ? star_ : QIcon( "" ) );
    if( QStandardItem* lock = dataModel_->item( index, 3 ) )
        lock->setIcon( profile.IsPasswordProtected() ? lock_ : QIcon( "" ) );
}

// -----------------------------------------------------------------------------
// Name: ProfilesPanel::Clean
// Created: LGY 2011-11-23
// -----------------------------------------------------------------------------
void ProfilesPanel::Clean()
{
    dataModel_->clear();
    dataModel_->setColumnCount( 6 );
    tableView_->horizontalHeader()->setResizeMode( 1, QHeaderView::Stretch );
    tableView_->setColumnWidth( 0, 20 );
    tableView_->setColumnWidth( 2, 25 );
    tableView_->setColumnWidth( 3, 25 );
    tableView_->setColumnWidth( 4, 25 );
    tableView_->setColumnWidth( 5, 25 );
}
