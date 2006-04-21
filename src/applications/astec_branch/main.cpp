
#include "astec_pch.h"
#include "App.h"

#include "MT_Tools/MT_Version.h"
#include "tools/Win32/StackWalkerProxy.h"

#include <qapplication.h>
#include <qmessagebox.h>
#include <fstream>

namespace
{
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
            return *new App( argc, argv );
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
    SetConsoleTitle( "ASTEC - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );

    __try
    {
        Run( InstanciateApp( argc, argv ) );
    }
    __except( StackWalkerProxy::ContinueSearch( GetExceptionInformation(), GetLog() ) )
    {
    }
    return 0;
}