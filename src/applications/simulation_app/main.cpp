
#include "simulation_app_pch.h"
#include "masalloc/masalloc.h"

#include "SIM_App.h"
#include "SIM_NetWorkLogger.h"

#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Version.h"
#include "MT_Tools/MT_CrashHandler.h"

#include "MT/MT_Logger/MT_FileLogger.h"
#include "tools/win32/FlexLm.h"
#include <xeumeuleu/xml.h>

#include <commctrl.h>
#include <new.h>
#include <dbghelp.h>
#include <direct.h>

#include "tools/Win32/Win32Exception.h"

int __cdecl NoMoreMemoryHandler( unsigned int nSize )
{
    int nResult = MessageBox( 0, MT_FormatString( "No more memory (%d bytes requested) - Retry ?", nSize ).c_str(), "SWORD - Memory error", MB_ICONERROR | MB_RETRYCANCEL | MB_TOPMOST );

    switch( nResult )
    {
        case IDRETRY:
            return 1;
        case IDCANCEL:
        default:
            throw std::bad_alloc();
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
// Name: SetLowFragmentationHeapAlgorithm
// Created: NLD 2006-01-20
// -----------------------------------------------------------------------------
void SetLowFragmentationHeapAlgorithm()
{
    ULONG ulHeapCompatibilityInformation = 2;
    HeapSetInformation( GetProcessHeap(), HeapCompatibilityInformation, &ulHeapCompatibilityInformation, sizeof(ulHeapCompatibilityInformation) );
}

//-----------------------------------------------------------------------------
// Name: Run()
// Created: NLD 2004-02-04
//-----------------------------------------------------------------------------
int Run( HINSTANCE hinstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( "sword", 1.0f ) );
#endif
    _mkdir( "./Debug" );
    MT_FileLogger           fileLogger     ( "./Debug/Sim.log" );
    MT_FileLogger           crashFileLogger( "./Debug/Crash " VERSION ".log", MT_Logger_ABC::eLogLevel_Error | MT_Logger_ABC::eLogLevel_FatalError, MT_Logger_ABC::eLogLayer_All );
    MT_LOG_REGISTER_LOGGER( fileLogger );
    MT_LOG_REGISTER_LOGGER( crashFileLogger );

    SetLowFragmentationHeapAlgorithm();

    // Float exceptions
    unsigned int control_word;
    _controlfp_s( &control_word, _EM_OVERFLOW | _EM_UNDERFLOW | _EM_INEXACT | _EM_INVALID, _MCW_EM ); // Exception raised for _EM_DENORMAL and _EM_ZERODIVIDE

    // Memory handlers
    _set_new_mode   ( 1 );
    _set_new_handler( NoMoreMemoryHandler );

    // Init the console window size and appearance
    // InitConsole();

    int nResult = EXIT_FAILURE;
    try
    {
        SIM_App app( hinstance, hPrevInstance, lpCmdLine, nCmdShow );
        MT_LOG_UNREGISTER_LOGGER( fileLogger );
        nResult = app.Execute();
    }
    catch( SIM_App::QuitException& e ) 
    {
    } 
    catch( MT_ScipioException& exception )
    {
        std::stringstream strMsg;
        strMsg << "Context : "     << exception.GetContext()     << std::endl
               << "File : "        << exception.GetFile()        << std::endl
               << "Line : "        << exception.GetLine()        << std::endl
               << "Message : "     << exception.GetMsg()         << std::endl
               << "Description : " << exception.GetDescription() << std::endl;
        MT_LOG_ERROR_MSG( strMsg.str() );
        MessageBox( 0, strMsg.str().c_str(), "SWORD - Invalid input data - Please check ODB data and launch the SIM again", MB_ICONEXCLAMATION | MB_OK | MB_TOPMOST );
    }
    catch( MT_Exception& exception )
    {
        std::stringstream strMsg;
        strMsg << "Context : " << exception.GetContext() << std::endl
               << "Code :"     << exception.GetCode()    << std::endl
               << "Message : " << exception.GetInfo()    << std::endl;
        MT_LOG_ERROR_MSG( strMsg.str() );
        MessageBox( 0, strMsg.str().c_str(), "SWORD - Invalid input data - Please check ODB data and launch the SIM again", MB_ICONEXCLAMATION | MB_OK | MB_TOPMOST );
    }
    catch( xml::exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        MessageBox ( 0, exception.what(), "SWORD - Invalid input data - Please check ODB data and launch the SIM again", MB_ICONEXCLAMATION | MB_OK | MB_TOPMOST );
    }
    catch( std::bad_alloc& /*exception*/ )
    {
        MT_LOG_ERROR_MSG( "Bad alloc" );
        MessageBox( 0, "Allocation error : not enough memory", "Simulation - Memory error", MB_ICONERROR | MB_OK | MB_TOPMOST );
    }
    catch( std::exception& exception )
    {
        MT_LOG_ERROR_MSG( exception.what() );
        MessageBox( 0, exception.what(), "SWORD - Exception standard", MB_ICONERROR | MB_OK | MB_TOPMOST );
    }

    MT_LOG_UNREGISTER_LOGGER( crashFileLogger );
    MT_LOG_UNREGISTER_LOGGER( fileLogger );

    return nResult;
}

int WINAPI WinMain( HINSTANCE hinstance, HINSTANCE hPrevInstance,LPSTR lpCmdLine, int nCmdShow )
{
    __try
    {
        return Run( hinstance, hPrevInstance, lpCmdLine, nCmdShow );
    }
    __except( MT_CrashHandler::ContinueSearch( GetExceptionInformation() ) )
    {
    }
    return 0;
}
 /*

#ifdef _MSC_VER
INT WINAPI WinMain( HINSTANCE /current/, HINSTANCE /previous/, LPSTR cmdline, int /showcmd/ )
{
bpo::command_line_parser parser( bpo::split_winmain( cmdline ) );
return Main( parser );
}
#endif

*/
