
#include "astec_pch.h"
#include "App.h"

#include "MT_Tools/MT_Version.h"

#include <qapplication.h>
#include <qmessagebox.h>

static void PureHandler()
{
    throw std::exception( "Pure virtual call" );
}

int main( int argc, char** argv )
{   
    _set_purecall_handler( PureHandler );
    SetConsoleTitle( "ASTEC - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );

    try
    {
        App a( argc, argv );
        a.processEvents();
        a.exec();
    }
    catch ( std::exception& e )
    {
        QApplication a( argc, argv );
        QMessageBox::critical( 0, APP_NAME, e.what() );
    }
    catch( ... )
    {
        // NOTHING
    }
    return 0;
}