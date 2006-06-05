ri
#include "astec_pch.h"
#include "App.h"

#include "MT_Tools/MT_Version.h"
#include "paranoia/ParanoiaFacade.h"

#include <qapplication.h>
#include <qmessagebox.h>

static void PureHandler()
{
    assert( false && "Pure Call" );
    throw std::exception( "Pure virtual call" );
}

int main( int argc, char** argv )
{   
    _set_purecall_handler( PureHandler );
    SetConsoleTitle( "ASTEC - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );

#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    ParanoiaFacade::CheckLicense( "astec" );
#endif

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

    return 0;
}

