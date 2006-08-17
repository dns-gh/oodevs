//*****************************************************************************
//
// $Created: JDY 03-06-19 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_Main.cpp $
// $Author: Ape $
// $Modtime: 14/04/05 15:56 $
// $Revision: 4 $
// $Workfile: ADN_Main.cpp $
//
//*****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_App.h"
#include "MT_Tools/MT_ScipioException.h"
#include <windows.h>
#include <commctrl.h>
#include <qtranslator.h>
#include <qtextcodec.h>

extern const char* szADN_Version;

/*
#pragma init_seg(lib)

class ADN_MessageOutputIntercepter
{
public:

    ADN_MessageOutputIntercepter()
    {
        qInstallMsgHandler( ADN_MessageOutputIntercepter::intercept );
    }

    static void intercept( QtMsgType type, const char *msg )
    {
        switch ( type ) 
        {
            case QtDebugMsg:
                fprintf( stderr, "Debug: %s\n", msg );
                break;
            case QtWarningMsg:
                fprintf( stderr, "Warning: %s\n", msg );
                break;
            case QtFatalMsg:
            {
                _asm {int 3}   
                fprintf( stderr, "Fatal: %s\n", msg );
            }
        }
    }
};

static ADN_MessageOutputIntercepter msgIntercept;
*/

void SetConsolePos( const int nPosX, const int nPosY )
{
    // reglage de la position de la console
    COORD coord;

    coord.X = 80;
    coord.Y = 25;

    SMALL_RECT rcPos;
    rcPos.Left = nPosX;
    rcPos.Top = nPosY;
    rcPos.Right = rcPos.Left + coord.X-1;
    rcPos.Bottom = rcPos.Top + coord.Y-1;

    int nRet;
    nRet = SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
    SetWindowPos( FindWindow( NULL, szADN_Version ), NULL, rcPos.Left, rcPos.Top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_FRAMECHANGED  );
}


void main( uint nArgc, char** ppArgv )
{
    // Console
    SetConsoleTitle( szADN_Version );    
    SetConsolePos( 0, 0 );
    MT_ConsoleLogger		consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    ADN_App app( nArgc, ppArgv );

    try
    {
        app.Initialize( nArgc, ppArgv );
        app.exec();
    }
    catch( MT_Exception& exception )
    {
        std::stringstream strMsg;
        strMsg << "Context : "     << exception.GetContext()     << std::endl
               << "Message : "     << exception.GetInfo()        << std::endl;
        
        MessageBox( 0, strMsg.str().c_str(), "Scipio Adaptation Tool - MT_Exception", MB_ICONERROR | MB_OK );
    }
    
    app.quit();

    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
}