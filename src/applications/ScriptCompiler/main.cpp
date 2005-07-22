//*****************************************************************************
//
// $Created: AGN 03-06-16 $
// $Archive: /MVW_v10/Build/SDK/SSC/src/main.cpp $
// $Author: Mco $
// $Modtime: 22/02/05 18:04 $
// $Revision: 11 $
// $Workfile: main.cpp $
//
//*****************************************************************************

#include "SSC_Workspace.h"

#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Version.h"

#include <windows.h>
#include <ostream>

bool IsInteractive( int argc, char** argv )
{
    // -s for 'silent'....
    return( argc < 2 || strcmp( argv[1], "-s" ) );
}

std::string Wrap( const std::string& content, const std::string& prefix )
{
    std::string result;
    std::stringstream input( content );
    std::string line;
    bool bFirst = true;
    while( std::getline( input, line ) )
    {
        if( ! bFirst )
            result += '\n';
        else
            bFirst = false;
        result += prefix + line;
    }
    return result;
}

int main( int argc, char** argv )
{
    bool bIsInteractive = IsInteractive( argc, argv );
    if( bIsInteractive )
        SetConsoleTitle( "Script Compiler - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__ );
    try
    {
        SSC_Workspace workspace( argc, argv );
    }
    catch( MT_ScipioException& exception )
     {
        static const std::string prefix( "ERROR: " );
        if( bIsInteractive )
        {
            std::stringstream strMsg;
            strMsg << Wrap( exception.GetMsg(), prefix ) << std::endl
                   << Wrap( exception.GetDescription(), prefix ) << std::endl;
            MessageBox( 0, strMsg.str().c_str(), "Scipio - Initialization error", MB_ICONERROR | MB_OK );
        }
        else
            std::cerr << Wrap( exception.GetMsg(), prefix ) << std::endl
                    << Wrap( exception.GetDescription(), prefix ) << std::endl;

        DIA_Workspace::TerminateSDK();
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
