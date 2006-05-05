
#include "SIM_pch.h"
#include "tools/masalloc/masalloc.h"

#include "SIM_App.h"

#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Version.h"
#include "MT_Tools/MT_CrashHandler.h"
    
#include "MT/MT_Logger/MT_ConsoleLogger.h"
#include "MT/MT_Logger/MT_FileLogger.h"

#include <windows.h>
#include <commctrl.h>
#include <new.h>
#include <dbghelp.h>
#include <direct.h>

#include "tools/Win32/Win32Exception.h"


///$$$$ TEST A GICLER
/*#include <qdatetime.h>
#include "Network/NET_ASN_Tools.h"

void Test()
{
    ASN1T_GDH asnGDH;
    NET_ASN_Tools::BuildAsnGDH( asnGDH );

    QDateTime initDateTime ( QDate( 1901, 1, 1 ) );
    QDateTime currentTmp = QDateTime::currentDateTime();

    uint nQTSec = initDateTime.secsTo( currentTmp );
    assert( nQTSec == asnGDH.datation );
} */
///$$$$ TEST A GICLER

int __cdecl NoMoreMemoryHandler( unsigned int nSize )
{
    int nResult = MessageBox( 0, MT_FormatString( "No more memory (%d bytes requested) - Retry ?", nSize ).c_str(), "Scipio - Memory error", MB_ICONERROR | MB_RETRYCANCEL | MB_TOPMOST );
    
    switch( nResult )
    {
        case IDRETRY: 
            return 1;             
        case IDCANCEL:
        default:
            throw std::bad_alloc( );
            return 0;
    }
}

//-----------------------------------------------------------------------------
// Name: WriteMiniDump()
// Created: NLD 2004-02-04
//-----------------------------------------------------------------------------
LONG WriteMiniDump( EXCEPTION_POINTERS* pExp )
{
    assert( pExp );

    if( !IsDebuggerPresent() && SIM_App::CrashWithCoreDump() )
    {
        _mkdir( "./Debug" );
        std::stringstream strFileName;
        strFileName << "./Debug/SIM " << VERSION << ".dmp";

        MT_LOG_INFO_MSG( MT_FormatString( "The SIM has crashed - Writing core dump to file %s", strFileName.str().c_str() ) );

        HANDLE hDumpFile = CreateFile( strFileName.str().c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL );

        MINIDUMP_EXCEPTION_INFORMATION eInfo;
        eInfo.ThreadId          = GetCurrentThreadId();
        eInfo.ExceptionPointers = pExp;
        eInfo.ClientPointers    = FALSE;

        MiniDumpWriteDump( GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpWithFullMemory, &eInfo, NULL, 0 );
 
        CloseHandle( hDumpFile );

        MT_LOG_INFO_MSG( "Core dump written" );
    }
    else
    {
        MT_LOG_INFO_MSG( "The SIM has crashed - no core dump generated" );
    }

    return EXCEPTION_CONTINUE_SEARCH;
}                          
        

//-----------------------------------------------------------------------------
// Name: GetConsoleHwnd()
// Created: NLD 2003-10-13
//-----------------------------------------------------------------------------
HWND GetConsoleHwnd()
{
#define MY_BUFSIZE 1024 // Buffer size for console window titles.

   HWND hwndFound;         // This is what is returned to the caller.
   char pszNewWindowTitle[MY_BUFSIZE]; // Contains fabricated
                                       // WindowTitle.
   char pszOldWindowTitle[MY_BUFSIZE]; // Contains original
                                       // WindowTitle.

   // Fetch current window title.
   GetConsoleTitle(pszOldWindowTitle, MY_BUFSIZE);

   // Format a "unique" NewWindowTitle.
   wsprintf( pszNewWindowTitle,"%d/%d", GetTickCount(), GetCurrentProcessId() );

   // Change current window title.
   SetConsoleTitle(pszNewWindowTitle);

   // Ensure window title has been updated.
   Sleep(40);

   // Look for NewWindowTitle.
   hwndFound = FindWindow(NULL, pszNewWindowTitle);

   // Restore original window title.
   SetConsoleTitle(pszOldWindowTitle);
   return hwndFound;
}

//-----------------------------------------------------------------------------
// Name: InitConsole()
// Created: NLD 2003-10-13
//-----------------------------------------------------------------------------
void InitConsole()
{   
    HANDLE hStdHandle = GetStdHandle( STD_OUTPUT_HANDLE );;
    CONSOLE_SCREEN_BUFFER_INFO screenBufferInfo; 
    GetConsoleScreenBufferInfo( hStdHandle, &screenBufferInfo );

    COORD coord = GetLargestConsoleWindowSize( hStdHandle );

    coord.X = coord.X * 3 / 4;
    coord.Y = 9999;

    SetConsoleScreenBufferSize( hStdHandle, coord );

    HWND hCurWin = GetConsoleHwnd();
    
    SetConsoleTitle( "SIM - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );

    ShowWindow( hCurWin, SW_MAXIMIZE );
}


/*
bool IsExecutionAllowed()
{
#define MAX_SIZE 1000

    HANDLE tokenHandle;
    if( !OpenProcessToken( GetCurrentProcess(), TOKEN_ALL_ACCESS, &tokenHandle ) )
        return false;

    DWORD        nReturnLength;
	TOKEN_OWNER* pTokenOwner = (TOKEN_OWNER *)malloc( MAX_SIZE );
	if( !GetTokenInformation( tokenHandle, TokenOwner, pTokenOwner, MAX_SIZE, &nReturnLength ) )
    {
        free( pTokenOwner );    
        return false;
    }

    SID_NAME_USE sidNameUse;
    char    pSzName      [MAX_SIZE];
    char    pSzDomainName[MAX_SIZE];
    DWORD   nNameLength   = MAX_SIZE; 
    DWORD   nDomainLength = MAX_SIZE;
    
    if( !LookupAccountSid( 0, pTokenOwner->Owner, pSzName, &nNameLength, pSzDomainName, &nDomainLength, &sidNameUse ) )
    {
        free( pTokenOwner );
        return false;
    }
    free( pTokenOwner );
    return !::stricmp( pSzDomainName, "MASA" );
}
*/

//-----------------------------------------------------------------------------
// Name: SetLowFragmentationHeapAlgorithm
// Created: NLD 2006-01-20
// -----------------------------------------------------------------------------
void SetLowFragmentationHeapAlgorithm()
{
    ULONG ulHeapCompatibilityInformation = 2;
    if( HeapSetInformation( GetProcessHeap(), HeapCompatibilityInformation, &ulHeapCompatibilityInformation, sizeof(ulHeapCompatibilityInformation) ) )
        MT_LOG_INFO_MSG( "Low fragmentation heap algorithm enabled" );
}

//-----------------------------------------------------------------------------
// Name: Run()
// Created: NLD 2004-02-04
//-----------------------------------------------------------------------------
int Run( uint nArgc, char* pArgv[] )
{
    _mkdir( "./Debug" );
    MT_ConsoleLogger        consoleLogger;
    MT_FileLogger           fileLogger( "./Debug/SIM " VERSION ".log", MT_Logger_ABC::eLogLevel_All, MT_Logger_ABC::eLogLayer_All, true ); // 'true' is for 'clear previous log'
    MT_LOG_REGISTER_LOGGER( consoleLogger );
    MT_LOG_REGISTER_LOGGER( fileLogger );

    SetLowFragmentationHeapAlgorithm();

    // Float exceptions
#ifdef _DEBUG
    _controlfp( _EM_OVERFLOW | _EM_UNDERFLOW | _EM_INEXACT, _MCW_EM ); // Exception raised for _EM_DENORMAL and _EM_ZERODIVIDE and _EM_INVALID
#else
    _controlfp( _EM_OVERFLOW | _EM_UNDERFLOW | _EM_INEXACT | _EM_INVALID, _MCW_EM ); // Exception raised for _EM_DENORMAL and _EM_ZERODIVIDE and _EM_INVALID
#endif

    // Memory handlers
    _set_new_mode   ( 1 );
    _set_new_handler( NoMoreMemoryHandler );
  
    // Init the console window size and appearance
    InitConsole();   

    SIM_App app( nArgc, pArgv );
    int nResult = app.Execute();

    MT_LOG_UNREGISTER_LOGGER( fileLogger );
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );

    return nResult;
}

//-----------------------------------------------------------------------------
// Name: main constructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
int main( uint nArgc, char* pArgv[] )
{
     __try
    {
        return Run( nArgc, pArgv );
    }
//    __except( WriteMiniDump( GetExceptionInformation() ) )
    __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
    {
    }
}

