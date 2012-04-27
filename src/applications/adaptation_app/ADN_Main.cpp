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
#include "license_gui/LicenseDialog.h"
#include "MT_Tools/MT_ConsoleLogger.h"
#include "MT_Tools/MT_Version.h"
#include "tools/Version.h"
#include <windows.h>
#include <commctrl.h>

#pragma warning( push )
#pragma warning( disable: 4127 4512 4511 )
#include <boost/program_options.hpp>
#pragma warning( pop )

namespace po = boost::program_options;

static const std::string szADN_Version   = "ADN - " + std::string( tools::AppProjectVersion() ) + " - " MT_COMPILE_TYPE " - " __TIMESTAMP__;

void SetConsolePos( const int nPosX, const int nPosY )
{
    // reglage de la position de la console
    COORD coord;

    coord.X = 80;
    coord.Y = 25;

    SMALL_RECT rcPos;
    rcPos.Left = static_cast< SHORT >( nPosX );
    rcPos.Top = static_cast< SHORT >( nPosY );
    rcPos.Right = rcPos.Left + coord.X-1;
    rcPos.Bottom = rcPos.Top + coord.Y-1;

    SetConsoleScreenBufferSize( GetStdHandle( STD_OUTPUT_HANDLE ), coord );
    SetWindowPos( FindWindow( NULL, szADN_Version.c_str() ), NULL, rcPos.Left, rcPos.Top, 0, 0, SWP_NOZORDER | SWP_NOACTIVATE | SWP_NOSIZE | SWP_FRAMECHANGED );
}

int main( int nArgc, char** ppArgv )
{
    int nResult = EXIT_FAILURE;
#if !defined( _DEBUG ) && ! defined( NO_LICENSE_CHECK )
    try
    {
        license_gui::LicenseDialog::CheckLicense( "sword-authoring" );
    }
    catch( std::exception& /*e*/ )
    {
        return nResult;
    }
#endif
    // Console
    SetConsoleTitle( szADN_Version.c_str() );
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
        ( "nosymbols,n", "turn off unit symbols view" )
        ( "noreadonly", "disable read-only protection" )
    ;
    po::variables_map vm;
    po::store( po::parse_command_line( nArgc, ppArgv, desc ), vm );
    po::notify( vm );

    try
    {
        bool nosymbols = vm.count( "nosymbols" ) != 0;
        bool noreadonly = vm.count( "noreadonly" ) != 0;
        if( app.Initialize( inputFile, outputFile, nosymbols, noreadonly, nArgc, ppArgv ) )
            nResult = app.exec();
        else
            nResult = EXIT_SUCCESS;
    }
    catch( ADN_Exception_ABC& e )
    {
        QMessageBox::critical( 0, e.GetExceptionTitle().c_str(), e.GetExceptionMessage().c_str() );
    }
    catch( std::exception& exception )
    {
        if( !outputFile.empty() )
            MessageBox( 0, exception.what(), "Sword Adaptation Tool - Exception", MB_ICONERROR | MB_OK );
        else
        {
            MT_LOG_ERROR_MSG( exception.what() );
            QMessageBox::critical( 0, "Critical", exception.what() );
        }
    }
    catch( ... )
    {
        QMessageBox::critical( 0, "Critical", "Unknown error !" );
    }
    MT_LOG_UNREGISTER_LOGGER( consoleLogger );
    return nResult;
}
