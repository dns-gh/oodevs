// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "Application.h"

#include "tools/Win32/StackWalkerProxy.h"
#include <qapplication.h>
#include <qmessagebox.h>
#include <fstream>

namespace
{
    ////
    static const QString locale = "_fr";
    ////

    void AddTranslator( const char* t )
    {
        QTranslator* trans = new QTranslator( qApp );
        trans->load( t + locale, "." );
        qApp->installTranslator( trans );
    }


    void PureHandler()
    {
        throw std::exception( "Pure virtual call" );
    }

    void Run( QApplication& app )
    {
        try
        {
            app.exec();
        }
        catch( std::exception& e )
        {
            QMessageBox::critical( 0, APP_NAME, e.what() );
        };
    }

    QApplication& InstanciateApp( int argc, char** argv )
    {
        try
        {
            QApplication& app = *new Application( argc, argv );
            AddTranslator( "qt_" );
            AddTranslator( "ENT" );
            AddTranslator( "clients_kernel" );
            AddTranslator( "clients_gui" );
            AddTranslator( "gaming" );
            AddTranslator( "gaming_app" );
            return app;
        }
        catch( ... )
        {
            std::exit( EXIT_FAILURE );
        }
    }

    std::ostream& GetLog()
    {
        static std::ofstream log( "crash.log" );
        return log;
    }
}

int main( int argc, char** argv )
{   
    _set_purecall_handler( PureHandler );
    SetConsoleTitle( APP_NAME " - " APP_VERSION " - " __TIMESTAMP__ );

    __try
    {
        Run( InstanciateApp( argc, argv ) );
    }
    __except( StackWalkerProxy::ContinueSearch( GetExceptionInformation(), GetLog() ) )
    {
    }
    return 0;
}