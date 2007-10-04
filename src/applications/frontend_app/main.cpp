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

int main( int argc, char* argv[] )
{
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
    return result;
}
