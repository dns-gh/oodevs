// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "FileToolbar.h"
#include "moc_FileToolbar.cpp"
#include "icons.h"
#include "clients_kernel/Controllers.h"

// -----------------------------------------------------------------------------
// Name: FileToolbar constructor
// Created: SBO 2006-12-13
// -----------------------------------------------------------------------------
FileToolbar::FileToolbar( QMainWindow* parent, kernel::Controllers& controllers )
    : QToolBar   ( parent, "File Toolbar" )
    , saveButton_( 0 )
    , controllers_( controllers )
{
    setLabel( tr( "Standard" ) );
    new QToolButton( MAKE_ICON( new ), tr( "New"  ), tr( "File" ), parent, SLOT( New() ), this );
    new QToolButton( MAKE_ICON( open ), tr( "Open" ), tr( "File" ), parent, SLOT( Open() ), this );
    saveButton_ = new QToolButton( MAKE_ICON( save ), tr( "Save" ), tr( "File" ), parent, SLOT( Save() ), this );
    saveAsButton_ = new QToolButton( MAKE_ICON( saveas ), tr( "Save As" ), tr( "File" ), parent, SLOT( SaveAs() ), this );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: FileToolbar destructor
// Created: SBO 2006-12-13
// -----------------------------------------------------------------------------
FileToolbar::~FileToolbar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: FileToolbar::EnableSaveItem
// Created: SBO 2006-12-13
// -----------------------------------------------------------------------------
void FileToolbar::EnableSaveItem( bool status )
{
    saveButton_->setEnabled( status );
}

// -----------------------------------------------------------------------------
// Name: FileToolbar::NotifyUpdated
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void FileToolbar::NotifyUpdated( const kernel::ModelLoaded& )
{
    saveAsButton_->setEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: FileToolbar::NotifyUpdated
// Created: SBO 2011-05-16
// -----------------------------------------------------------------------------
void FileToolbar::NotifyUpdated( const kernel::ModelUnLoaded& )
{
    saveAsButton_->setEnabled( false );
}
