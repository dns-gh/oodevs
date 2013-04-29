// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include <timeline/api.h>

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
    QObject::connect( &app, SIGNAL( lastWindowClosed() ), &app, SLOT( quit() ) );
    try
    {
        if( argc != 3 )
            throw std::exception( "usage: timeline_app.exe <exe> <url>" );

        QMainWindow main;
        main.resize( 800, 600 );
        auto central = new QWidget( &main );
        main.setCentralWidget( central );

        QPalette palette;
        palette.setColor( QPalette::Background, Qt::red );
        central->setAutoFillBackground( true );
        central->setPalette( palette );

        timeline::Configuration cfg;
        cfg.rundir = ".";
        cfg.binary = argv[1];
        if( !cfg.binary.IsRegularFile() )
            throw std::runtime_error( QString( "invalid file %1" ).arg( argv[1] ).toStdString() );

        cfg.widget = central;
        cfg.target = argv[2];
        auto context = timeline::MakeServer( cfg );

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
