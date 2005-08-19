
#include "MOS_Light_pch.h"
#include "MOS_App.h"

#include "MT_Tools/MT_Version.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT/MT_Tools/MT_Exception.h"

#include <qapplication.h>
#include <qpushbutton.h>
#include <qmessagebox.h>

LONG Nothing( EXCEPTION_POINTERS* )
{
    return EXCEPTION_CONTINUE_SEARCH;
}

int Main( int argc, char** argv )
{
    try
    {
        MOS_App::CreateApp( argc, argv );
        MOS_App::GetApp().Initialize();
        return MOS_App::GetApp().exec();
    }
    catch( MT_ScipioException& exception )
    {
        std::stringstream strMsg;
        strMsg << "Context : "     << exception.GetContext()     << std::endl
            << "File : "        << exception.GetFile()        << std::endl
            << "Line : "        << exception.GetLine()        << std::endl
            << "Message : "     << exception.GetMsg()         << std::endl
            << "Description : " << exception.GetDescription() << std::endl;
        MOS_App::GetApp().HideEverything();
        QMessageBox::critical( 0, "Exception caught", strMsg.str().c_str(), QMessageBox::Abort, QMessageBox::Abort, QMessageBox::Abort );
    }
    catch( MT_Exception& e )
    {
        MOS_App::GetApp().HideEverything();
        QMessageBox::critical( 0, "Exception caught", e.GetWholeMessage(), QMessageBox::Abort, QMessageBox::Abort, QMessageBox::Abort );
    }
    catch( std::exception& e )
    {
        MOS_App::GetApp().HideEverything();
        QMessageBox::critical( 0, "Exception caught", e.what(), QMessageBox::Abort, QMessageBox::Abort, QMessageBox::Abort );
    }
    catch( ... )
    {
        MOS_App::GetApp().HideEverything();
        QMessageBox::critical( 0, "Exception caught", "Unknown exception caught", QMessageBox::Abort, QMessageBox::Abort, QMessageBox::Abort );
    }
    return EXIT_FAILURE;
}

int main( int argc, char** argv )
{   
    SetConsoleTitle( "MOS Light - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );

     __try
    {
        return Main( argc, argv );
    }
    __except( Nothing( GetExceptionInformation() ) )
    {
    }
    return EXIT_FAILURE;
}

