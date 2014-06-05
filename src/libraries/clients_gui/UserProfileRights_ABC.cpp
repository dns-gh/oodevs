// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "UserProfileRights_ABC.h"
#include "resources.h"
#include "RichTreeView.h"
#include "StandardModel.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/UserProfile_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC constructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfileRights_ABC::UserProfileRights_ABC( RichTreeView& listView, StandardModel& model, const QString& name )
    : listView_( listView )
    , model_( model )
    , profile_( 0 )
    , check_( MAKE_PIXMAP( check ) )
    , check_grey_( MAKE_PIXMAP( check_grey ) )
{
    model_.setColumnCount( 3 );
    listView_.setHeaderHidden( false );
    listView_.setEditTriggers( 0 );
    listView_.EnableDragAndDrop( false );

    QStringList headers;
    headers << name << tools::translate( "UserProfileRights", "Read" ) << tools::translate( "UserProfileRights", "Write" );
    model_.setHorizontalHeaderLabels( headers );
    listView_.header()->setMovable( false );
    listView_.header()->setStretchLastSection( false );
    listView_.header()->setResizeMode( 0, QHeaderView::Stretch );
    listView_.header()->setResizeMode( 1, QHeaderView::Fixed );
    listView_.header()->setResizeMode( 2, QHeaderView::Fixed );
    listView_.header()->resizeSection( 1, 48 );
    listView_.header()->resizeSection( 2, 48 );
    listView_.setDisabled( true );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC destructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfileRights_ABC::~UserProfileRights_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::Visit
// Created: JSR 2012-09-25
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::Visit( QStandardItem& item )
{
    kernel::Entity_ABC* entity = model_.GetDataFromItem< kernel::Entity_ABC >( item );
    if( entity )
    {
        const Status status = Status( item.data( Roles::OtherRole ).toInt() );
        const bool isWriteable = status == eWrite;
        const bool isReadable  = status == eReadOnly;
        profile_->SetReadable ( *entity, isReadable && !isWriteable );
        profile_->SetWriteable( *entity, isWriteable );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::Commit
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::Commit()
{
    if( !profile_ )
        return;
    model_.Accept( *this );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::Display
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::Display( kernel::UserProfile_ABC& profile )
{
    Clear();
    listView_.setDisabled( false );
    profile_ = &profile;
    for( int i = 0; i < model_.rowCount(); ++i )
    {
        QStandardItem* item = model_.item( i );
        SetStatus( item, false, false );
        EnsureVisible( item );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::OnItemClicked
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::OnItemClicked( const QModelIndex& index )
{
    if( !index.isValid() || index.column() == 0 )
        return;

    QStandardItem* item = model_.GetItemFromIndex( model_.GetMainModelIndex( index ) );
    const Status status = Status( item->data( Roles::OtherRole ).toInt() );
    if( status == eWriteInherited || ( index.column() == 1 && status == eReadInherited ) )
        return;
    bool write = status == eWrite;
    if( index.column() == 2 )
        write = !write;
    bool read = status == eWrite || status == eReadOnly;
    if( index.column() == 1 )
        read = !read;
    SetStatus( item, read, write, false, false );
    Commit(); // $$$$ SBO 2007-11-08: should not be done after every change...
}

namespace
{
    class ClearVisitor : public StandardModelVisitor_ABC
                       , private boost::noncopyable
    {
    public:
        ClearVisitor() {}
        ~ClearVisitor() {}
        virtual void Visit( QStandardItem& item )
        {
            const QStandardItemModel& model = static_cast< const QStandardItemModel& >( *item.model() );
            const QModelIndex index = model.indexFromItem( &item );
            const QModelIndex index1 = model.index( index.row(), 1, index.parent() );
            const QModelIndex index2 = model.index( index.row(), 2, index.parent() );
            model.itemFromIndex( index1 )->setIcon( QPixmap() );
            model.itemFromIndex( index2 )->setIcon( QPixmap() );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::Clear
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::Clear()
{
    ClearVisitor visitor;
    model_.Accept( visitor );
    listView_.collapseAll();
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::SetStatus
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::SetStatus( QStandardItem* item, Status status )
{
    assert( item->column() == 0 );
    item->setData( static_cast< int >( status ), Roles::OtherRole );
    const QModelIndex index = model_.indexFromItem( item );
    const QModelIndex index1 = model_.index( index.row(), 1, index.parent() );
    const QModelIndex index2 = model_.index( index.row(), 2, index.parent() );
    QStandardItem* item1 = model_.itemFromIndex( index1 );
    QStandardItem* item2 = model_.itemFromIndex( index2 );
    if( status == eNothing )
    {
        item1->setIcon( QPixmap() );
        item2->setIcon( QPixmap() );
    }
    else if( status == eReadOnly )
    {
        item1->setIcon( check_ );
        item2->setIcon( QPixmap() );
    }
    else if( status == eReadInherited )
    {
        item1->setIcon( check_grey_ );
        item2->setIcon( QPixmap() );
    }
    else if( status == eWrite )
    {
        item1->setIcon( check_ );
        item2->setIcon( check_ );
    }
    else if( status == eWriteInherited )
    {
        item1->setIcon( check_grey_ );
        item2->setIcon( check_grey_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::SetStatus
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::SetStatus( QStandardItem* item, bool inheritsReadable, bool inheritsWriteable )
{
    assert( item->column() == 0 );
    kernel::Entity_ABC* entity = model_.GetDataFromItem< kernel::Entity_ABC >( *item );
    if( !entity )
        return;
    const bool isWriteable = profile_->IsWriteable( *entity );
    const bool isReadable  = isWriteable || profile_->IsReadable( *entity );
    SetStatus( item, isReadable, isWriteable, inheritsReadable, inheritsWriteable );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::SetStatus
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::SetStatus( QStandardItem* item, bool isReadable, bool isWriteable, bool inheritsReadable, bool inheritsWriteable )
{
    assert( item->column() == 0 );
    SetStatus( item, MakeStatus( isReadable, isWriteable, inheritsReadable, inheritsWriteable ) );
    for( int i = 0; i < item->rowCount(); ++i )
    {
        QStandardItem* value = item->child( i );
        SetStatus( value, isReadable || inheritsReadable, isWriteable || inheritsWriteable );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::EnsureVisible
// Created: LDC 2012-04-30
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::EnsureVisible( QStandardItem* item )
{
    assert( item->column() == 0 );
    kernel::Entity_ABC* entity = model_.GetDataFromItem< kernel::Entity_ABC >( *item );
    if( !entity )
        return;
    const bool isWriteable = profile_->IsWriteable( *entity );
    const bool isReadable  = isWriteable || profile_->IsReadable( *entity );
    if( isReadable || isWriteable )
        listView_.expand( model_.indexFromItem( item ) );
    for( int i = 0; i < item->rowCount(); ++i )
        EnsureVisible( item->child( i ) );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::MakeStatus
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfileRights_ABC::Status UserProfileRights_ABC::MakeStatus( bool read, bool write, bool inheritedRead, bool inheritedWrite )
{
    Status status = eNothing;
    if( write )
        status = eWrite;
    else if( read )
        status = eReadOnly;
    else if( inheritedWrite )
        status = eWriteInherited;
    else if( inheritedRead )
        status = eReadInherited;
    return status;
}
