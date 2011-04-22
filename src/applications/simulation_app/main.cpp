
#include "simulation_app_pch.h"
#include "SIM_App.h"
#include "SIM_NetworkLogger.h"
#include "MT_Tools/MT_ScipioException.h"
#include "tools/Version.h"
#include "MT_Tools/MT_CrashHandler.h"
#include "MT_Tools/MT_FormatString.h"
#include "MT_Tools/MT_FileLogger.h"
#include "MT_Tools/MT_Logger.h"
#include <tools/Win32/Win32Exception.h>
#include <tools/win32/FlexLm.h>
#include <masalloc/masalloc.h>
#pragma warning( push )
#pragma warning( disable : 4244 )
#include <google/protobuf/message.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <new.h>
#include <dbghelp.h>
#include <direct.h>

int __cdecl NoMoreMemoryHandler( std::size_t nSize )
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
    int maxConnections = 10;
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    std::auto_ptr< FlexLmLicense > license( FlexLmLicense::CheckLicense( "sword-runtime", 1.0f ) );
    try
    {
        FlexLmLicense license( "sword-dispatcher", 1.0f );
        maxConnections = license.GetAuthorisedUsers();
    }
    catch( FlexLmLicense::LicenseError& )
    {
        maxConnections = 10;
    }
#endif

    _mkdir( "./Debug" );
    MT_FileLogger           fileLogger     ( "./Debug/Sim.log" );
    const std::string filename = "./Debug/Crash Version " + std::string( tools::AppVersion() ) + ".log";
    MT_FileLogger           crashFileLogger( filename.c_str(), MT_Logger_ABC::eLogLevel_Error | MT_Logger_ABC::eLogLevel_FatalError );
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
        GOOGLE_PROTOBUF_VERIFY_VERSION;
        SIM_App app( hinstance, hPrevInstance, lpCmdLine, nCmdShow, maxConnections );
        MT_LOG_UNREGISTER_LOGGER( fileLogger );
        nResult = app.Execute();
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

    google::protobuf::ShutdownProtobufLibrary();

    MT_LOG_UNREGISTER_LOGGER( crashFileLogger );
    MT_LOG_UNREGISTER_LOGGER( fileLogger );

    return nResult;
}

int WINAPI WinMain( HINSTANCE hinstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow )
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
