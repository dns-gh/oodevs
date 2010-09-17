// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "adaptation_app_pch.h"
#include "ADN_App.h"
#include "ADN_Exception_ABC.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include <windows.h>
#include <commctrl.h>
#include <qtranslator.h>
#include <qtextcodec.h>

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

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

int main( uint nArgc, char** ppArgv )
{
    // Console
    SetConsoleTitle( szADN_Version );
    SetConsolePos( 0, 0 );
    MT_ConsoleLogger        consoleLogger;
    MT_LOG_REGISTER_LOGGER( consoleLogger );

    ADN_App app( nArgc, ppArgv );

    // Command line options
    std::string inputFile, outputFile;
    po::options_description desc( "Allowed options" );
    desc.add_options()
        ( "input,i" , po::value< std::string >( &inputFile  )->default_value( "" ), "specify root input file (physical.xml)" )
        ( "output,o", po::value< std::string >( &outputFile )->default_value( "" ), "specify output file (physical.xml) (open/save-mode: input must be specified)" )
    ;
    po::variables_map vm;
    po::store( po::parse_command_line( nArgc, ppArgv, desc ), vm );
    po::notify( vm );

    try
    {
        if( app.Initialize( inputFile, outputFile ) )
            app.exec();
    }
    catch( ADN_Exception_ABC& e )
    {
        QMessageBox::critical( 0, e.GetExceptionTitle().c_str(), e.GetExceptionMessage().c_str() );
        return EXIT_FAILURE;
    }
    catch( std::exception& exception )
    {
        if( !outputFile.empty() )
            MessageBox( 0, exception.what(), "Sword Adaptation Tool - Exception", MB_ICONERROR | MB_OK );
        else
            MT_LOG_ERROR_MSG( exception.what() );
        app.quit();
        MT_LOG_UNREGISTER_LOGGER( consoleLogger );
        return EXIT_FAILURE;
    }
    app.quit();
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return EXIT_SUCCESS;
}
