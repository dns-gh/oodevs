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

// -----------------------------------------------------------------------------
// Name: FileToolbar constructor
// Created: SBO 2006-12-13
// -----------------------------------------------------------------------------
FileToolbar::FileToolbar( QMainWindow* pParent )
    : QToolBar      ( pParent, "File Toolbar" )
    , saveButton_   ( 0 )
{
    setLabel( tr( "Standard" ) );
    new QToolButton( MAKE_ICON( new )   , tr( "New"  )  , tr( "File" ), pParent, SLOT( New() )   , this );
    new QToolButton( MAKE_ICON( open )  , tr( "Open" )  , tr( "File" ), pParent, SLOT( Open() )  , this );
    saveButton_ = new QToolButton( MAKE_ICON( save )  , tr( "Save" )  , tr( "File" ), pParent, SLOT( Save() )  , this );
    saveAsButton_ = new QToolButton( MAKE_ICON( saveas )  , tr( "Save As" )  , tr( "File" ), pParent, SLOT( SaveAs() )  , this );
}

// -----------------------------------------------------------------------------
// Name: FileToolbar destructor
// Created: SBO 2006-12-13
// -----------------------------------------------------------------------------
FileToolbar::~FileToolbar()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: FileToolbar destructor
// Created: SBO 2006-12-13
// -----------------------------------------------------------------------------
void FileToolbar::EnableSaveItem( bool status )
{
    saveButton_->setEnabled( status );
    saveAsButton_->setEnabled( status );
}
