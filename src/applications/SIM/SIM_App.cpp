//*****************************************************************************
//
// $Created: NLD 2002-08-07 $
// $Archive: /MVW_v10/Build/SDK/SIM/src/SIM_App.cpp $
// $Author: Nld $
// $Modtime: 18/04/05 17:21 $
// $Revision: 62 $
// $Workfile: SIM_App.cpp $
//
//*****************************************************************************

#include "SIM_pch.h"

#include "SIM_App.h"
#include <ctime>
#include <signal.h>
#include "SIM_NetworkLogger.h"

#include "MIL/MIL_AgentServer.h"
#include "Missions/Missions.h"
#include "MT_Tools/MT_Version.h"
#include "MT_Tools/MT_ScipioException.h"
#include "MT_Tools/MT_Profiler.h"
#include "MT/MT_XmlTools/MT_XXmlInputArchive.h"

bool SIM_App::bCrashWithCoreDump_ = false;
bool SIM_App::bUserInterrupt_     = false;

//-----------------------------------------------------------------------------
// Name: SIM_App constructor
// Created: NLD 2002-08-07
// Last modified: JVT 03-07-24
//-----------------------------------------------------------------------------
SIM_App::SIM_App( int nArgc, char* pArgv[] )
    : startupConfig_ ()
    , pNetworkLogger_( 0 )
    , bTestMode_     ( false )
{
    std::string strMsg = "Scipio SIM - " VERSION " - " MT_COMPILE_TYPE " - " __TIMESTAMP__;
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );
    MT_LOG_STARTUP_MESSAGE( strMsg.c_str() );
    MT_LOG_STARTUP_MESSAGE( "----------------------------------------------------------------" );

    ParseCmdArgs( nArgc, pArgv, startupConfig_ );

    try
    {
        pNetworkLogger_ = new SIM_NetworkLogger( 20000 + startupConfig_.GetExerciceID() ); //$$$ 20000 en dur
        MT_LOG_REGISTER_LOGGER( *pNetworkLogger_ );
    }
    catch( MT_Exception& exception )
    {
        MT_LOG_WARNING_MSG( MT_FormatString( "Network logger (telnet) not registered - Reason : '%s'", exception.GetInfo().c_str() ) );
        pNetworkLogger_ = 0;
    }
}


//-----------------------------------------------------------------------------
// Name: SIM_App destructor
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
SIM_App::~SIM_App()
{
    if( pNetworkLogger_ )
    {
        MT_LOG_UNREGISTER_LOGGER( *pNetworkLogger_ );
        delete pNetworkLogger_;
    }
}

// -----------------------------------------------------------------------------
// Name: SIM_App::UserInterruptHandler
// Created: SBO 2005-11-25
// -----------------------------------------------------------------------------
void SIM_App::UserInterruptHandler( int /*nContext*/ )
{
    bUserInterrupt_ = true;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Initialize
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Initialize()
{
    MT_Profiler::Initialize();
    MT_MakeDir( "CheckPoints" );

    missions::RegisterMissions();

    MIL_AgentServer::CreateWorkspace( startupConfig_ );
    
    signal( SIGINT, &UserInterruptHandler );
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Cleanup
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Cleanup()
{
    MIL_AgentServer::DestroyWorkspace();
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Run
// Created: NLD 2004-01-27
// -----------------------------------------------------------------------------
void SIM_App::Run()
{
    bool bRun = true;
    while( bRun && !bUserInterrupt_ )
    {            
        MIL_AgentServer::E_SimState nSimState = MIL_AgentServer::GetWorkspace().Update(); 
        bRun = ( nSimState != MIL_AgentServer::eSimStopped );

        if( pNetworkLogger_ )
            pNetworkLogger_->Update();
        ::Sleep( 0 );
    }
}

//-----------------------------------------------------------------------------
// Name: SIM_App::Execute
// Created: NLD 2002-08-07
//-----------------------------------------------------------------------------
int SIM_App::Execute()
{
    if( bTestMode_ )
        return Test();
    try
    {
        Initialize();
        Run       ();
        Cleanup   ();
        return EXIT_SUCCESS;
    }
    catch( MT_ScipioException& exception )
    {
        std::stringstream strMsg;
        strMsg << "Context : "     << exception.GetContext()     << std::endl
               << "File : "        << exception.GetFile()        << std::endl
               << "Line : "        << exception.GetLine()        << std::endl
               << "Message : "     << exception.GetMsg()         << std::endl
               << "Description : " << exception.GetDescription() << std::endl;
        MessageBox( 0, strMsg.str().c_str(), "Scipio - Invalid input data - Please check ODB data and launch the SIM again", MB_ICONEXCLAMATION | MB_OK | MB_TOPMOST );
    }
    catch( MT_Exception& exception )
    {
        std::stringstream strMsg;
        strMsg << "Context : " << exception.GetContext() << std::endl
               << "Code :"     << exception.GetCode()    << std::endl
               << "Message : " << exception.GetInfo()    << std::endl;
        MessageBox( 0, strMsg.str().c_str(), "Scipio - Invalid input data - Please check ODB data and launch the SIM again", MB_ICONEXCLAMATION | MB_OK | MB_TOPMOST );
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        MessageBox ( 0, exception.what(), "Scipio - Invalid input data - Please check ODB data and launch the SIM again", MB_ICONEXCLAMATION | MB_OK | MB_TOPMOST );
    }
    catch( std::bad_alloc& /*exception*/ )
    {
        MessageBox( 0, "Allocation error : not enough memory", "Scipio - Memory error", MB_ICONERROR | MB_OK | MB_TOPMOST );
    }
    catch( std::exception& exception )
    {
        MessageBox( 0, exception.what(), "Scipio - Exception standard", MB_ICONERROR | MB_OK | MB_TOPMOST );
    }
    return EXIT_FAILURE;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Test
// Created: MCO 2005-02-21
// -----------------------------------------------------------------------------
int SIM_App::Test()
{
    static const std::string prefix( "ERROR: " );
    try
    {
        Initialize();
        Cleanup   ();
        return EXIT_SUCCESS;
    }
    catch( MT_ScipioException& exception )
    {
        std::cerr << Wrap( exception.GetMsg(), prefix ) << std::endl
                  << Wrap( exception.GetDescription(), prefix ) << std::endl;
    }
    catch( MT_Exception& exception )
    {
        std::cerr << Wrap( exception.GetInfo(), prefix ) << std::endl;
    }
    catch( MT_ArchiveLogger_Exception& exception )
    {
        std::cerr << Wrap( exception.what(), prefix ) << std::endl;
    }
    catch( std::bad_alloc& /*exception*/ )
    {
        std::cerr << Wrap( "Allocation error : not enough memory", prefix )  << std::endl;
    }
    catch( std::exception& exception )
    {
        std::cerr << Wrap( exception.what(), prefix ) << std::endl;
    }
    return EXIT_FAILURE;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::IsAlreadyWrapped
// Created: MCO 2005-02-22
// -----------------------------------------------------------------------------
bool SIM_App::IsAlreadyWrapped( const std::string& content ) const
{
    return content.find( "WARNING" ) != std::string::npos || content.find( "ERROR" ) != std::string::npos || content.find( "INFO" ) != std::string::npos;
}

// -----------------------------------------------------------------------------
// Name: SIM_App::Wrap
// Created: MCO 2005-02-21
// -----------------------------------------------------------------------------
std::string SIM_App::Wrap( const std::string& content, const std::string& prefix ) const
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
        if( ! IsAlreadyWrapped( line ) )
            result += prefix;
        result += line;
    }
    return result;
}

//-----------------------------------------------------------------------------
// Name: App::ParseCmdArgs
// Created:  NLD 2002-02-18 
//-----------------------------------------------------------------------------


// #include <boost/program_options.hpp>

bool SIM_App::ParseCmdArgs( int nArgc, char** ppArgv, MIL_Config& startupConfig )
{
/*
    try
    {
        boost::program_options::options_description desc;
        desc.add_options()
                ( "help"                                    , "produce help message" )
                ( "nodiaarchive"               , ""   )
                ( "forceodbcomposition" , ""   )
                ( "diadebugserver", ""   )
                ( "decdebug"                       , ""   )
                ( "coredump"                       , ""   )
                ( "profiling"                     , ""   )
                ( "nocheckpointcrc"         , ""   )
                ( "test"                               , ""   )
                ( "exerciceID"                   , boost::program_options::value< int >()/*->default( 0 )* )
                ( "checkpoint"                   , boost::program_options::value< std::string >() )
                ( "conffile"                       , boost::program_options::value< std::string >() )
            ;
                

        boost::program_options::variables_map vm;
        boost::program_options::store ( boost::program_options::parse_command_line(nArgc, ppArgv, desc), vm );
        boost::program_options::notify( vm );    

        if( vm.count( "help" ) )
        {
            std::cout << desc << std::endl;
            return false;
        }
    }
    catch( std::exception& e )
    {
        std::cout << e.what() << std::endl;
    }*/


    MT_LOG_INFO_MSG( MT_FormatString( "%d arguments on command line", nArgc ).c_str() );

    const char* szCommandLine = "Usage : %s [-nodiaarchive] [-forceodbcomposition] [-diadebugserver] [-nbpathfindthreads #] [-exerciceID #] [-checkpoint IDEX__YYYY_MM_DD__HHhMMmSEC] [-nocheckpointcrc] [-test]";
    if( nArgc < 1 )
    {
        MT_LOG_INFO_MSG( MT_FormatString( szCommandLine, ppArgv[0] ).c_str() );
        return false;
    }

    int nArgNbr;
    for( nArgNbr = 0 ; nArgNbr < nArgc; ++nArgNbr )
    {
        MT_LOG_INFO_MSG( MT_FormatString( "\t Argument %d: %s", nArgNbr, ppArgv[nArgNbr] ).c_str() );
    }

    // Parse options
    for( nArgNbr = 1; nArgNbr < nArgc && *ppArgv[nArgNbr] == '-'; ++nArgNbr )
    {
        if( stricmp( ppArgv[nArgNbr], "-nodiaarchive") == 0 )
            startupConfig.SetUseDIAArchive( false );
        else if( stricmp( ppArgv[nArgNbr], "-forceodbcomposition") == 0 )
            startupConfig.SetForceODBAutomateComposition( true );
        else if( stricmp( ppArgv[nArgNbr], "-diadebugserver") == 0 )
            startupConfig.SetUseDiaDebugServer( true );
        else if( stricmp( ppArgv[nArgNbr], "-decdebug") == 0 )
            startupConfig.SetUseDecDebug( true );
        else if( stricmp( ppArgv[nArgNbr], "-pathdebug") == 0 )
            startupConfig.SetUsePathDebug( true );
        else if( stricmp( ppArgv[nArgNbr], "-coredump") == 0 )
            bCrashWithCoreDump_ = true;
        else if( stricmp( ppArgv[nArgNbr], "-profiling") == 0 )
            startupConfig.SetProfilingEnabled( true );
        else if( stricmp( ppArgv[nArgNbr], "-nocheckpointcrc") == 0 )
            startupConfig.SetUseCheckPointCRC( false );
        else if( stricmp( ppArgv[nArgNbr], "-exerciceID" ) == 0 )
        {
            ++nArgNbr;

            if( nArgNbr >= nArgc )
            {
                MT_LOG_ERROR_MSG( "Please specify the exerciceID" );
                return false;
            }
            startupConfig.SetExerciceID( atoi( ppArgv[nArgNbr] ) );
        }
        else if ( stricmp( ppArgv[nArgNbr], "-checkpoint" ) == 0 )
        {
            if ( ++nArgNbr >= nArgc )
            {
                MT_LOG_ERROR_MSG( "Please specify the checkpoint name" );
                return false;
            }
            startupConfig.SetCheckPointFileName( ppArgv[ nArgNbr ] );
        }
        else if ( stricmp( ppArgv[nArgNbr], "-conffile" ) == 0 )
        {
            if ( ++nArgNbr >= nArgc )
            {
                MT_LOG_ERROR_MSG( "Please specify the configuration file name" );
                return false;
            }
            const std::string strConfFile( ppArgv[ nArgNbr ] );

            std::string strConfFilePath;
            std::string strConfFileName;
            MT_ExtractFilePath( strConfFile, strConfFilePath );
            MT_ExtractFileName( strConfFile, strConfFileName );
            MT_ChangeDir( strConfFilePath );
            MT_LOG_INFO_MSG( MT_FormatString( "Changing current directory to : %s", MT_GetCurrentDir().c_str() ) );
            startupConfig.SetConfigFileName( strConfFileName );
        }
        else if ( stricmp( ppArgv[nArgNbr], "-test" ) == 0 )
        {
            bTestMode_ = true;
        }
        else if ( stricmp( ppArgv[nArgNbr], "-testdata" ) == 0 )
        {
            startupConfig_.SetDataTestMode( true );
            bTestMode_ = true;
        }

        else if( stricmp( ppArgv[nArgNbr], "-?"     ) == 0 ||
                 stricmp( ppArgv[nArgNbr], "--help" ) == 0 )
        {
            MT_LOG_INFO_MSG( MT_FormatString( szCommandLine, ppArgv[0] ).c_str() );
            return false;
        }
    }       
    return true;
}
