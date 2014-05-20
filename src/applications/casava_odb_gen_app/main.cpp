// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "Dialog.h"
#include "clients_gui/ApplicationMonitor.h"
#include "tools/Exception.h"
#include "tools/WinArguments.h"
#include <windows.h>
#include <QtGui/QtGui>

int main()
{
    tools::WinArguments winArgs( GetCommandLineW() );
    int argc = winArgs.Argc();
    auto argv = winArgs.Argv();
    gui::ApplicationMonitor monitor( argc, argv );
    try
    {
        ( new Dialog() )->exec();
    }
    catch( const std::exception& e )
    {
       QMessageBox::critical( 0, "Error", tools::GetExceptionMsg( e ).c_str() );
    }
    return EXIT_SUCCESS;
}
