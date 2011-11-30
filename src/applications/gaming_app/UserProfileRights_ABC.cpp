// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "UserProfileRights_ABC.h"
#include "icons.h"
#include "clients_kernel/Tools.h"
#include "gaming/UserProfile.h"
#include "clients_gui/ValuedListItem.h"

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC constructor
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
UserProfileRights_ABC::UserProfileRights_ABC( Q3ListView* listView )
    : listView_       ( listView )
    , selectedProfile_( 0 )
    , needsSaving_    ( false )
    , check_          ( MAKE_PIXMAP( check ) )
    , check_grey_     ( MAKE_PIXMAP( check_grey ) )
{
    listView_->header()->show();
    listView_->addColumn( tools::translate( "UserProfileRights", "Read" ) , 40 );
    listView_->addColumn( tools::translate( "UserProfileRights", "Write" ), 40 );
    listView_->addColumn( "hidden", 0 );
    listView_->header()->setResizeEnabled( false );
    listView_->hideColumn( 3 );
    listView_->setDisabled( true );
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
// Name: UserProfileRights_ABC::Commit
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::Commit( bool savedStatus )
{
    needsSaving_ = savedStatus;
    if( !selectedProfile_ )
        return;
    Q3ListViewItemIterator it( listView_ );
    while( Q3ListViewItem* item = it.current() )
    {
        if( const ValuedListItem* value = static_cast< const ValuedListItem* >( item ) )
        {
            const Entity_ABC* entity = value->GetValue< const Entity_ABC >();
            const Status status = Status( item->text( 3 ).toInt() );
            const bool isWriteable = status == eWrite;
            const bool isReadable  = status == eReadOnly;
            selectedProfile_->SetReadable ( *entity, isReadable && !isWriteable );
            selectedProfile_->SetWriteable( *entity, isWriteable );
        }
        ++it;
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::Reset
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::Reset()
{
    // $$$$ SBO 2007-01-17: TODO: somehow undo changes
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::Display
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::Display( UserProfile& profile )
{
    CloseAll();
    listView_->setDisabled( false );
    needsSaving_ = false;
    selectedProfile_ = &profile;
    ValuedListItem* value = static_cast< ValuedListItem* >( listView_->firstChild() );
    while( value )
    {
        SetStatus( value, false, false );
        value = static_cast< ValuedListItem* >( value->nextSibling() );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::OnItemClicked
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::OnItemClicked( Q3ListViewItem* item, const QPoint&, int column )
{
    if( column == 0 || !item )
        return;

    const Status status = Status( item->text( 3 ).toInt() );

    if( status == eWriteInherited || ( column == 1 && status == eReadInherited ) )
        return;
    bool write = status == eWrite;
    if( column == 2 )
        write = !write;
    bool read = status == eWrite || status == eReadOnly;
    if( column == 1 )
        read = !read;
    SetStatus( static_cast< ValuedListItem* >( item ), read, write, false, false );
    needsSaving_ = true;
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::OnShow
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::OnShow()
{
    if( selectedProfile_ )
        Display( *selectedProfile_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::OnHide
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::OnHide()
{
    Commit( needsSaving_ );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::CloseAll
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::CloseAll()
{
    for( Q3ListViewItemIterator it( listView_ ); it.current(); ++it )
        listView_->setOpen( *it, false );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::SetStatus
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::SetStatus( Q3ListViewItem* item, Status status )
{
    item->setText( 3, QString::number( status ) );
    if( status == eNothing )
    {
        item->setPixmap( 1, QPixmap() );
        item->setPixmap( 2, QPixmap() );
    }
    else if( status == eReadOnly )
    {
        item->setPixmap( 1, check_ );
        item->setPixmap( 2, QPixmap() );
    }
    else if( status == eReadInherited )
    {
        item->setPixmap( 1, check_grey_ );
        item->setPixmap( 2, QPixmap() );
    }
    else if( status == eWrite )
    {
        item->setPixmap( 1, check_ );
        item->setPixmap( 2, check_ );
    }
    else if( status == eWriteInherited )
    {
        item->setPixmap( 1, check_grey_ );
        item->setPixmap( 2, check_grey_ );
    }
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::SetStatus
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::SetStatus( gui::ValuedListItem* item, bool inheritsReadable, bool inheritsWriteable )
{
    const Entity_ABC* entity = item->GetValue< const Entity_ABC >();
    if( !entity )
        return;
    const bool isWriteable = selectedProfile_->IsWriteable( *entity );
    const bool isReadable  = isWriteable || selectedProfile_->IsReadable( *entity );

    SetStatus( item, isReadable, isWriteable, inheritsReadable, inheritsWriteable );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::SetStatus
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::SetStatus( gui::ValuedListItem* item, bool isReadable, bool isWriteable, bool inheritsReadable, bool inheritsWriteable )
{
    SetStatus( item, MakeStatus( isReadable, isWriteable, inheritsReadable, inheritsWriteable ) );
    if( isReadable || isWriteable )
        listView_->ensureItemVisible( item );

    ValuedListItem* value = static_cast< ValuedListItem* >( item->firstChild() );
    while( value )
    {
        SetStatus( value, isReadable || inheritsReadable, isWriteable || inheritsWriteable );
        value = static_cast< ValuedListItem* >( value->nextSibling() );
    }
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

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::SetInheritedStatus
// Created: SBO 2007-01-18
// -----------------------------------------------------------------------------
void UserProfileRights_ABC::SetInheritedStatus( Q3ListViewItem* item, Status status )
{
    if( !item )
        return;
    for( Q3ListViewItem* child = item->firstChild(); child; child = child->nextSibling() )
        SetStatus( child, status );
}

// -----------------------------------------------------------------------------
// Name: UserProfileRights_ABC::NeedsSaving
// Created: SBO 2007-03-29
// -----------------------------------------------------------------------------
bool UserProfileRights_ABC::NeedsSaving() const
{
    return needsSaving_;
}

