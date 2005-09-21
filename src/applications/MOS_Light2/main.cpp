
#include "MOS_Light2_pch.h"
#include "MOS_App.h"

#include "MT_Tools/MT_Version.h"

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
    SetConsoleTitle( "MOS Light - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );

    try
    {
        MOS_App a( argc, argv );
        a.processEvents();
        a.exec();
    }
    catch ( std::exception& e )
    {
        QApplication a( argc, argv );
        QMessageBox::critical( 0, "MOSLight 2", e.what() );
    }

    return 0;
}

