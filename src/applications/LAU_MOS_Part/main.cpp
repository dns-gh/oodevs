//*****************************************************************************
//
// $Created : AML 03-04-01 $
//  $Archive: /MVW_v10/Build/SDK/LAU_MOS_Part/src/main.cpp $
//   $Author: Aml $
//  $Modtime: 17/04/03 15:17 $
// $Revision: 4 $
// $Workfile: main.cpp $
//
//*****************************************************************************

#include "LMP_pch.h"

#include "LMP_App.h"

#include "MT/MT_Logger/MT_ConsoleLogger.h"

#include <windows.h>
#include <commctrl.h>

const char* szConsoleName = "LAU_MOS_Part";

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

//    nRet = SetConsoleWindowInfo( GetStdHandle( STD_OUTPUT_HANDLE ), TRUE, &rcPos ); // cette fonction ne deplace pas la fenetre!
    

    SetWindowPos( FindWindow( NULL, szConsoleName ), NULL, rcPos.Left, rcPos.Top, 0, 0, SWP_NOZORDER | SWP_NOOWNERZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_FRAMECHANGED );
    // reglage - fin
}

//
// le main (fonction principale) de l'appli (voila un commentaire bien utile!)
//

void main( uint nArgc, char** ppArgv )
{
    SetConsoleTitle( szConsoleName );
    SetConsolePos( 0, 320 );
    
    MT_ConsoleLogger		consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    
    try
    {
        bool32 bRestart = true;
        while( bRestart )
        {
            LMP_App app( nArgc, ppArgv );
            bRestart = app.Run();
        }
    }
    catch( MT_ScipioException& exception )
    {
        std::stringstream strMsg;
        strMsg << "Context : "     << exception.GetContext()     << std::endl
               << "File : "        << exception.GetFile()        << std::endl
               << "Line : "        << exception.GetLine()        << std::endl
               << "Message : "     << exception.GetMsg()         << std::endl
               << "Description : " << exception.GetDescription() << std::endl;
          
        MessageBox( 0, strMsg.str().c_str(), "Scipio - SimLauncher - Initialization error", MB_ICONERROR | MB_OK );
    }
    catch( MT_Exception& exception )
    {
        std::stringstream strMsg;
        strMsg << "Context : "     << exception.GetContext()     << std::endl
               << "Message : "     << exception.GetInfo()        << std::endl;
        
        MessageBox( 0, strMsg.str().c_str(), "Scipio - SimLauncher - MT_Exception", MB_ICONERROR | MB_OK );
    }

    MT_LOG_UNREGISTER_LOGGER( consoleLogger );

//    consoleLogger.SaveToFile( "LAU_log.txt" );
}
