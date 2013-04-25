// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include <timeline_ui/api.h>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <QtGui>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

int main( int argc, char* argv[] )
{
    QT_REQUIRE_VERSION( argc, argv, "4.7.0" );
    QApplication app( argc, argv );
    try
    {
        if( argc != 2 )
            throw std::exception( "usage: timeline_app.exe <url>" );

        QMainWindow main;
        main.resize( 800, 600 );
        auto central = new QWidget( &main );
        main.setCentralWidget( central );

        timeline::Configuration cfg;
        cfg.widget = central;
        cfg.target = argv[1];
        auto context = timeline::MakeContext( cfg );

        main.show();
        return app.exec();
    }
    catch( const std::exception& err )
    {
        QMessageBox::critical( NULL, "Critical failure", err.what() );
        return -1;
    }
    catch( ... )
    {
        QMessageBox::critical( NULL, "Critical failure", "Unexpected error" );
        return -1;
    }
}
