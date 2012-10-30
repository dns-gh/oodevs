// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "ApplicationMonitor.h"
#include "clients_kernel/Tools.h"
#include "moc_ApplicationMonitor.cpp"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: ApplicationMonitor::ApplicationMonitor
// Created: BAX 2012-10-29
// -----------------------------------------------------------------------------
ApplicationMonitor::ApplicationMonitor( int& argc, char** argv )
    : QApplication( argc, argv )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ApplicationMonitor::~ApplicationMonitor
// Created: BAX 2012-10-29
// -----------------------------------------------------------------------------
ApplicationMonitor::~ApplicationMonitor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ApplicationMonitor::ApplicationMonitor
// Created: BAX 2012-10-29
// -----------------------------------------------------------------------------
bool ApplicationMonitor::notify( QObject* receiver, QEvent* event )
{
    try
    {
        return QApplication::notify( receiver, event );
    }
    catch( std::exception& e )
    {
        emit Error( QString( e.what() ) );
    }
    catch( ... )
    {
        emit Error( tools::translate( "ApplicationMonitor", "Unexpected exception" ) );
    }
    return true;
}
