// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "SaveModelChecker.h"
#include "preparation/Tools.h"

// -----------------------------------------------------------------------------
// Name: SaveModelChecker constructor
// Created: JSR 2011-07-27
// -----------------------------------------------------------------------------
SaveModelChecker::SaveModelChecker( QMainWindow* window )
    : window_( window )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SaveModelChecker destructor
// Created: JSR 2011-07-27
// -----------------------------------------------------------------------------
SaveModelChecker::~SaveModelChecker()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SaveModelChecker::Validate
// Created: JSR 2011-07-27
// -----------------------------------------------------------------------------
bool SaveModelChecker::Validate()
{
    return true;
}

// -----------------------------------------------------------------------------
// Name: SaveModelChecker::Reject
// Created: JSR 2011-07-27
// -----------------------------------------------------------------------------
bool SaveModelChecker::Reject( const QString& reason )
{
    QMessageBox::critical( window_, tools::translate( "Application", "SWORD" ), reason );
    return false;
}

// -----------------------------------------------------------------------------
// Name: SaveModelChecker::Prompt
// Created: JSR 2011-07-27
// -----------------------------------------------------------------------------
bool SaveModelChecker::Prompt( const QString& question )
{
    if( QMessageBox::question( window_, tools::translate( "Application", "SWORD" )
                             , question + tools::translate( "MainWindow", "\nDo you want to save anyway?" ), QMessageBox::Yes, QMessageBox::No ) == QMessageBox::Yes )
        return true;
    return false;
}
