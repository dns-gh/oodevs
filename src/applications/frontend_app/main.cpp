// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_app_pch.h"
#include "Application.h"
#include "MT/MT_Logger/MT_Logger_Lib.h"

int main( int argc, char* argv[] )
{
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    int result = EXIT_FAILURE;
    try
    {
        Application app( argc, argv );
        result = app.exec();
    }
    catch ( std::exception& e )
    {
        QApplication no_app( argc, argv );
        QMessageBox::critical( 0, "frontend_app exception", e.what() );
    }
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return result;
}
