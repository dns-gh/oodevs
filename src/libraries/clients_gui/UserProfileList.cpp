// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UserProfileList.h"
#include "moc_UserProfileList.cpp"
#include "UserProfileWidget.h"
#include "NewProfileDialog.h"
#include "clients_kernel/ProfilesModel_ABC.h"
#include "clients_gui/SubObjectName.h"
#include "clients_kernel/ProfileEditor.h"
#include "clients_kernel/UserProfile_ABC.h"
#include "RichPushButton.h"
#include "RichWidget.h"

using namespace gui;

Q_DECLARE_METATYPE( kernel::ProfileEditor* )

// -----------------------------------------------------------------------------
// Name: UserProfileList constructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileList::UserProfileList( QWidget* parent, UserProfileWidget& pages, kernel::Controller& controller, kernel::ProfilesModel_ABC& model )
    : QWidget( parent )
    , controller_( controller )
    , pages_( pages )
    , model_( model )
    , pNewProfileDialog_( new NewProfileDialog( this, *this ) )
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

    pages_.SetChecker( this );
}

// -----------------------------------------------------------------------------
// Name: UserProfileList destructor
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
UserProfileList::~UserProfileList()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::Commit
// Created: JSR 2014-06-03
// -----------------------------------------------------------------------------
void UserProfileList::Commit()
{
    for( auto it = localProfiles_.begin(); it != localProfiles_.end(); ++it )
        model_.CommitFromEditor( **it);
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::Exists
// Created: JSR 2014-06-02
// -----------------------------------------------------------------------------
bool UserProfileList::Exists( const QString& oldLogin, const QString& newLogin ) const
{
    for( auto it = localProfiles_.begin(); it != localProfiles_.end(); ++it )
    {
        if( !( *it )->IsDeleted() )
        {
            const QString& login = ( *it )->GetProfile().GetLogin();
            if( login != oldLogin && login == newLogin )
                return true;
        }
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::Exists
// Created: JSR 2014-06-02
// -----------------------------------------------------------------------------
bool UserProfileList::Exists( const QString& login ) const
{
    for( auto it = localProfiles_.begin(); it != localProfiles_.end(); ++it )
        if( !( *it )->IsDeleted() && ( *it )->GetProfile().GetLogin() == login )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnSelectionChanged
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnSelectionChanged()
{
    QModelIndexList indexes = list_->selectionModel()->selectedIndexes();
    if( isVisible() && !indexes.empty() )
    {
        const int row = proxyModel_->mapToSource( indexes.front() ).row();
        if( kernel::ProfileEditor* profile = dataModel_->item( row )->data().value< kernel::ProfileEditor* >() )
            pages_.Display( profile->GetProfile() );
    }
}

namespace
{
    QStandardItem* CreateItem( kernel::ProfileEditor& profile )
    {
        QStandardItem* item = new QStandardItem( profile.GetProfile().GetLogin() );
        item->setFlags( Qt::ItemIsSelectable | Qt::ItemIsEnabled );
        item->setData( QVariant::fromValue( &profile ) );
        return item;
    }

    int GetIndex( const QStandardItemModel* model, const kernel::UserProfile_ABC* profile )
    {
        for( int i = 0; i < model->rowCount(); ++i )
            if( &model->item( i )->data().value< kernel::ProfileEditor* >()->GetProfile() == profile )
                return i;
        return -1;
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnCreate
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnCreate()
{
    const QString& result = pNewProfileDialog_->Exec();
    if( result.isEmpty() )
        return;
    kernel::ProfileEditor* profileEditor = model_.CreateProfileEditor();
    profileEditor->GetProfile().SetLogin( result );
    localProfiles_.emplace_back( profileEditor );
    dataModel_->setItem( dataModel_->rowCount(), 0, CreateItem( *profileEditor ) );
    pages_.setVisible( true );
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::OnDelete
// Created: SBO 2007-01-16
// -----------------------------------------------------------------------------
void UserProfileList::OnDelete()
{
    QModelIndexList indexes = list_->selectionModel()->selectedIndexes();
    if( !indexes.empty() )
    {
        const int index = proxyModel_->mapToSource( indexes.front() ).row();
        dataModel_->item( index )->data().value< kernel::ProfileEditor* >()->Delete();
        dataModel_->removeRow( index );
        pages_.setVisible( dataModel_->rowCount() > 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::NotifyNameChanged
// Created: JSR 2014-06-03
// -----------------------------------------------------------------------------
void UserProfileList::NotifyNameChanged( const kernel::UserProfile_ABC* profile ) const
{
    int index = GetIndex( dataModel_, profile );
    if( index != -1 )
    {
        QStandardItem* item = dataModel_->item( index );
        item->setText( profile->GetLogin() );
        proxyModel_->sort( 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::showEvent
// Created: SBO 2007-11-08
// -----------------------------------------------------------------------------
void UserProfileList::showEvent( QShowEvent* event )
{
    model_.Apply( [&]( kernel::UserProfile_ABC& profile )
        {
            kernel::ProfileEditor* profileEditor = model_.CreateProfileEditor( profile );
            localProfiles_.emplace_back( profileEditor );
            dataModel_->setItem( dataModel_->rowCount(), 0, CreateItem( *profileEditor ) );
        });
    proxyModel_->sort( 0 );
    QWidget::showEvent( event );
    pages_.setVisible( dataModel_->rowCount() > 0 );
    list_->selectionModel()->clear();
    list_->selectionModel()->select( proxyModel_->index( 0, 0 ), QItemSelectionModel::Select );
}

// -----------------------------------------------------------------------------
// Name: UserProfileList::hideEvent
// Created: JSR 2014-06-02
// -----------------------------------------------------------------------------
void UserProfileList::hideEvent( QHideEvent* event )
{
    dataModel_->clear();
    localProfiles_.clear();
    QWidget::hideEvent( event );
}
