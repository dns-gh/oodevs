// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#pragma warning(push, 0)
#include <QtGui/qapplication.h>
#pragma warning(pop)
#include "MainWindow.h"

int main( int argc, char**argv )
{
    QApplication myApp( argc, argv );
    MainWindow mainWindow( argc < 2 ? "" : argv[1] );
    mainWindow.show();
    return myApp.exec();
}
