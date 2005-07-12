//*****************************************************************************
//
// Name     : main.cpp
//
// $Created : AML 03-03-27 $
// $Archive: /MVW_v10/Build/SDK/LAU/src/main.cpp $
// $Author: Nld $
// $Modtime: 9/09/03 17:13 $
// $Revision: 17 $
// $Workfile: main.cpp $
//
//*****************************************************************************

#include "LAU_pch.h"

#include "LAU_App.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Version.h"

#include "MT/MT_Logger/MT_ConsoleLogger.h"

#include <windows.h>
#include <WinUser.h> // pour MB_SERVICE_NOTIFICATION (definir alors _WIN32_WINNT)
#include <io.h>
#include <iostream>
#include <commctrl.h>
#include <process.h> // pour spawn
#include "LAU_AdminService.h"

const char* szLAU_Version = "LAU - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__;

void SetConsolePos( const int nPosX, const int nPosY )
{
    // reglage de la position de la console
    COORD coord;

    coord.X = 80;
    coord.Y = 25;

    SMALL_RECT rcPos;
    rcPos.Left = (short)nPosX;
    rcPos.Top = (short)nPosY;
    rcPos.Right = (short)( rcPos.Left + coord.X-1 );
    rcPos.Bottom = (short)( rcPos.Top + coord.Y-1 );

    int nRet;
    nRet = SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
//    nRet = SetConsoleWindowInfo( GetStdHandle( STD_OUTPUT_HANDLE ), TRUE, &rcPos );
    // ne fait rien !!!

    SetWindowPos( FindWindow( NULL, szLAU_Version ), NULL, rcPos.Left, rcPos.Top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_FRAMECHANGED  );
    // reglage - fin

}

typedef enum E_RunningMode
{
    eInstall,
    eUninstall,
    eDebug,
    eService,
    eHelp,
    eStart,     // Start le service (idem démarrer dans le Services Manager)
    eStop,
} T_RunningMode;


T_RunningMode ParseStandardArgs( int nArgc, char* ppArgv[] )
{
    if ( nArgc <= 1 )
        return eService;

    if ( strcmp( ppArgv[1], "-?" ) == 0 )
        return eHelp;
    else if ( stricmp( ppArgv[1], "-i" ) == 0 )
        return eInstall;
    else if ( stricmp( ppArgv[1], "-u" ) == 0 )
        return eUninstall;
    else if ( stricmp( ppArgv[1], "-d" ) == 0 )
        return eDebug;
    else if ( stricmp( ppArgv[1], "-start" ) == 0 )
        return eStart;
    else if ( stricmp( ppArgv[1], "-stop" ) == 0 )
        return eStop;
    
    // Don't recognise the args
    return eService;
}

int Install( LAU_AdminService& service )
{
    if ( service.IsInstalled() )
        std::cout << service.GetName() << " is already installed\n";
    else if ( service.Install() )
        std::cout << service.GetName() << " installed\n";
    else 
        std::cout << service.GetName() << " failed to install. Error: " << GetLastError() << "\n";


    // verification qu'il a bien accés au desktop:

    HWINSTA hWinsta = GetProcessWindowStation();

    OutputLogDebug( MT_FormatString( "GetProcessWindowStation=>%d", hWinsta ).c_str(), __FILE__, __LINE__ );

    USEROBJECTFLAGS uoFlags;
    uint32 nLenReturned;
    GetUserObjectInformation( hWinsta, UOI_FLAGS, &uoFlags, sizeof( uoFlags ), &nLenReturned );

    OutputLogDebug( MT_FormatString( "uoFlags.dwFlags: %x", uoFlags.dwFlags ).c_str(), __FILE__, __LINE__ );
    if( ( uoFlags.dwFlags & WSF_VISIBLE ) != WSF_VISIBLE )
    {
        OutputLogDebug( "Install: InteractWithDesktop is not enabled", __FILE__, __LINE__ );

        // on a pas l'acces, on affiche un message (avec une mbox special pour que l'user la voit quand meme...)
        // pb ca gele des fois l'execution cette putain de mbox, et des fois ca marche, en fait ca gele quand on a pas l'acces (mais ca marche les autres fois)
        // c'est vraiement trop naze...

//        MessageBox( NULL, "WARNING: The service must have 'interaction to the desktop' rights (in the Service Manager, check ScipioLauncher\\Connection\\Interact With Desktop)", "Error", MB_SERVICE_NOTIFICATION );
    }

    return service.IsInstalled();
}

int Uninstall( LAU_AdminService& service )
{
    if ( ! service.IsInstalled() )
        std::cout << service.GetName() << " is not installed\n";
    else if ( service.Uninstall() )
        std::cout << service.GetName() << " removed.\n";
    else 
        std::cout << "Could not remove " << service.GetName() << ". Error " << GetLastError() << "\n";

    return ! service.IsInstalled();
}

int Usage( LAU_AdminService& service )
{
    std::cout << "The service is " << ( service.IsInstalled() ? "currently" : "not" ) << " installed.\n";
    std::cout << "use -i to install, -u to uninstall, -d to run like a normal application.\n";
    std::cout << "use -start to start the service, -stop to stop the service.\n";

    return 1;
}

int Start( LAU_AdminService& service )
{
    OutputLogDebug( "Start", __FILE__, __LINE__ );

    service.StartService();
    
    // When we get here, the service has been stopped


    // verification qu'il a bien accés au desktop:

    HWINSTA hWinsta = GetProcessWindowStation();

    OutputLogDebug( MT_FormatString( "GetProcessWindowStation=>%d", hWinsta ).c_str(), __FILE__, __LINE__ );

    USEROBJECTFLAGS uoFlags;
    uint32 nLenReturned;
    GetUserObjectInformation( hWinsta, UOI_FLAGS, &uoFlags, sizeof( uoFlags ), &nLenReturned );

    OutputLogDebug( MT_FormatString( "uoFlags.dwFlags: %x", uoFlags.dwFlags ).c_str(), __FILE__, __LINE__ );
    if( ( uoFlags.dwFlags & WSF_VISIBLE ) != WSF_VISIBLE )
    {
        OutputLogDebug( "Start: InteractWithDesktop is not enabled", __FILE__, __LINE__ );

        // on a pas l'acces, on affiche un message (avec une mbox special pour que l'user la voit quand meme...)
        // pb ca gele des fois l'execution cette putain de mbox, et des fois ca marche, en fait ca gele quand on a pas l'acces (mais ca marche les autres fois)
        // c'est vraiement trop naze...

//        MessageBox( NULL, "WARNING: The service must have 'interaction to the desktop' rights (in the Service Manager, check ScipioLauncher\\Connection\\Interact With Desktop)", "Error", MB_SERVICE_NOTIFICATION );
    }


    return service.GetExitCode();
}

int SystemStartService( LAU_AdminService& service )
{
    const char szActionName[] = " started\n";

    int nRet = spawnlp( P_WAIT, "net", "net", "start", service.GetName(), NULL );
    if( nRet == 0 )
    {
        std::cout << service.GetName() << szActionName;
    }
    else
    {
        std::cout << service.GetName() << " is not" << szActionName;
    }

    return nRet == -1;
}

int SystemStopService( LAU_AdminService& service )
{
    const char szActionName[] = " stopped\n";

    int nRet = spawnlp( P_WAIT, "net", "net", "stop", service.GetName(), NULL );
    if( nRet == 0 )
    {
        std::cout << service.GetName() << szActionName;
    }
    else
    {
        std::cout << service.GetName() << " is not" << szActionName;
    }

    return nRet == -1;
}


int MainDebug( uint nArgc, char** ppArgv )
{
    OutputLogDebug( "MainDebug", __FILE__, __LINE__ );

    SetConsoleTitle( szLAU_Version );
    SetConsolePos( 0, 0 );

    MT_ConsoleLogger		consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    
    try
    {
        bool32 bContinue = true;
        while( bContinue )
        {
            LAU_App app( nArgc, ppArgv );
            app.Initialize();
            bContinue = app.Run();
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

    return 1;
}

///////////////////////////////////////////////////////////////////////////////
//
// le main (fonction principale) de l'appli (voila un commentaire bien utile!)
//
///////////////////////////////////////////////////////////////////////////////

#ifndef LAU_IS_A_SERVICE
void main( uint nArgc, char** ppArgv )
{
    OutputLogDebug( "Main(NotAService) - Begin", __FILE__, __LINE__ );

    MainDebug( nArgc, ppArgv );
}

#else // LAU_IS_A_SERVICE

#include <ole2.h>

int main( int nArgc, char** ppArgv )
{
    // AML 03-04-24: CAVE: si une dll n'est pas trouvée DANS LE REPERTOIRE COURANT,
    // AML 03-04-24: sous NT4/2000 le message est: "dll not found"
    // AML 03-04-24: sous XP, le message est: Impossible de démarrer le service, Erreur 1053: le service n'a pas répondu assez vite à la demande de lancement ou de contrôle

    // mettre toutes les dll dans le meme rep que sim.exe (QT????.dll ...)
    // mettre le service avec l'option peut accéder au bureau...
    
    // le service ne s'exécute pas forcément dans le bon repertoire
    // (et donc ne trouve pas la licence ASN1...)
    SetCurrentDirectory( MT_GetApplicationPath() );

    MT_FileLogger		    fileLogger( "ScipioLauncherLog.log" );
    MT_LOG_REGISTER_LOGGER( fileLogger );


#ifdef _DEBUG
    OutputLogDebug( "(first log - DEBUG)" );
#else
    OutputLogDebug( "(first log - RELEASE)" );
#endif // _DEBUG

    OutputLogDebug( "Main(Service) - Begin", __FILE__, __LINE__ );



    HRESULT r;
    r = OleInitialize(0);

    OutputLogDebug( "Main(Service) - Apres OleInitialize", __FILE__, __LINE__ );

    LAU_AdminService service( "ScipioLauncher" );

    service.SetInteractWithDesktop();

    OutputLogDebug( "Main(Service) - Apres Create objet service", __FILE__, __LINE__ );

    int nValRet = -100;

    // Parse for standard arguments 
    // -i         install
    // -u         uninstall, does not stop running service but uninstalls it though
    // -d         debug, runs in console mode
    // -?         help, displays usage and whether service is installed or not
    switch ( ParseStandardArgs( nArgc, ppArgv ) )
    {
        case eInstall:
            nValRet = Install( service );
            break;
        case eUninstall:
            nValRet = Uninstall( service );
            break;
        case eDebug:
            nValRet = MainDebug( nArgc, ppArgv );
            break;
        case eService:
            nValRet = Start( service );
            break;
        case eStart:
            nValRet = SystemStartService( service );
            break;
        case eStop:
            nValRet = SystemStopService( service );
            break;
        default :
            nValRet = Usage( service );
            break;
    }

    OleUninitialize();

    char buffer[512];
    sprintf( buffer, "Main(Service) - Quitting ret code: %d", nValRet );
    OutputLogDebug( buffer, __FILE__, __LINE__ );

    MT_LOG_UNREGISTER_LOGGER( fileLogger );

    return nValRet;
}
#endif // LAU_IS_A_SERVICE