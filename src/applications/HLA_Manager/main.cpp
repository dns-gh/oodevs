
#include "HLA_Manager_pch.h"
#include "HLA_MainWindow.h"
#include <qapplication.h>

//-----------------------------------------------------------------------------
// Name: main
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
int main( int argc, char* argv[] )
{
    QApplication app( argc, argv );
    HLA_MainWindow mainWindow;
    app.setMainWidget( &mainWindow );
    mainWindow.show();
    return app.exec();
}

