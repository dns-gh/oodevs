// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "PluginProcessHandler.h"
#include "dispatcher/Logger_ABC.h"
#include "dispatcher/StaticModel.h"
#include "dispatcher/Config.h"
#include "protocol/Simulation.h"
#include <windows.h>
#include <winbase.h>
#include <tlhelp32.h>
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/noncopyable.hpp>

using namespace plugins::bml;
namespace bfs = boost::filesystem;

struct PluginProcessHandler::InternalData
{
    PROCESS_INFORMATION pid_ ;
    InternalData() { ZeroMemory( &pid_ , sizeof( pid_ ) ); }
};

class PluginProcessHandler::PluginConfig 
{
public:
    PluginProcessHandler::PluginConfig( const std::string& data, const std::string& session, dispatcher::Logger_ABC& logger )
        : data_ ( data )
        , exercise_ ( bfs::path( bfs::path( session, bfs::native ).parent_path().parent_path().parent_path() ).native_directory_string() )
        , session_ ( session )
        , logger_ ( logger )
    {
        // NOTHING
    }

    std::string GetSimulationHost() const 
    {
        xml::xifstream xis( session_ );

        xis >> xml::start( "session" ) >> xml::start( "config" )
                >> xml::start( "dispatcher" ) >> xml::start( "network" );
        std::string network( xis.attribute< std::string >( "client" ) );

        return network.substr( 0, network.find( ':' ) ) + ":" + 
                            boost::lexical_cast< std::string >( xis.attribute< std::string >( "server" ) );
    }
    
    std::string GetExerciseName() const
    {
        return bfs::path( exercise_, bfs::native ).leaf();
    }
    
    std::string GetSessionDir() const 
    {
        return bfs::path( session_, bfs::native ).parent_path().string();
    }
        
    std::string GetPhysicalDir() const 
    {
        return bfs::path( data_, bfs::native ).parent_path().string();
    }

    void LoadProfile( std::string& profile, std::string& password ) const
    {
        xml::xifstream xis( exercise_ + "/exercise.xml" );
        
        xis >> xml::start( "exercise" ) 
                >> xml::start( "profiles" ) ;
        LoadProfile( exercise_ + "/" + xis.attribute< std::string >( "file" ), profile, password );
    }


private:
    void LoadProfile( const std::string& file, std::string& profile, std::string& password ) const
    {
        try 
        {
            xml::xifstream xif( file );
            xif >> xml::start( "profiles" )
                >> xml::list( "profile", boost::bind( &PluginProcessHandler::PluginConfig::ReadProfileInfo, *this, _1, boost::ref( profile ), boost::ref( password ) ) );
        }
        catch( const xml::exception& e ) 
        {
            logger_.LogWarning( LOG_MESSAGE( e.what() ) );
        }
        catch( ... )
        {
            logger_.LogWarning( LOG_MESSAGE( "Unable to load profile information. Try to load anonymous profile." ) );
        }
    }

    void ReadProfileInfo( xml::xistream& xis, std::string& profile, std::string& password ) const
    {
        if ( profile.empty() && xis.attribute< bool >( "supervision" ) )
        {
            xis >> xml::attribute( "name", profile )
                >> xml::attribute( "password", password );
        }
    }

private:
    const std::string data_;
    const std::string exercise_;
    const std::string session_;
    dispatcher::Logger_ABC& logger_;
};

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler constructor
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
PluginProcessHandler::PluginProcessHandler( const dispatcher::Config& config, const std::string& process_name, dispatcher::Logger_ABC& logger, xml::xistream& xis )
    : logger_ ( logger )
    , processName_ ( process_name )
    , workingDir_  ( config.BuildPluginDirectory( "bml" ) )
    , sessionDir_  ( config.GetSessionFile() )
    , physicalDir_ ( config.GetPhysicalFile() )
    , commandLine_ ( workingDir_ + "/" + processName_ )
    , config_      ( new PluginConfig( physicalDir_, sessionDir_, logger ) )
    , xis_         ( new xml::xibufferstream( xis ) )
{
    LoadSimulationConfig( *config_ );
    LoadPluginConfig( *xis_, *config_ );
    logger_.LogInfo( LOG_MESSAGE( "Wait the first simulation tick before before launching " + processName_ + " process." ) );
}

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler destructor
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
PluginProcessHandler::~PluginProcessHandler()
{
    if( internal_.get() )
        Stop();
}

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::Receive
// Created: JCR 2011-11-18
// -----------------------------------------------------------------------------
void PluginProcessHandler::Receive( const sword::SimToClient& message )
{
    if( message.message().has_control_end_tick() && !internal_.get() )
    {
        try
        {
            logger_.LogInfo( LOG_MESSAGE( "Starting " + processName_ + " process." ) );
            LoadLoginProfile();
            internal_.reset( new InternalData() );
            Start();
            logger_.LogInfo( LOG_MESSAGE( "Started." ) );
        }
        catch ( std::exception& e )
        {
            logger_.LogError( LOG_MESSAGE( e.what() ) );
        }
    }
}

namespace 
{
    std::string GetLastErrorMessage()
    {
        LPVOID lpMsgBuf;
        FormatMessage(
            FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
            NULL,
            GetLastError(),
            MAKELANGID( LANG_NEUTRAL, SUBLANG_DEFAULT ),
            (LPTSTR) &lpMsgBuf,
            0, NULL );
        const std::string result( static_cast< char* >( lpMsgBuf ) );
        LocalFree( lpMsgBuf );
        return boost::algorithm::erase_last_copy( result, "\r\n" );
    }
}

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::Start
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
void PluginProcessHandler::Start()
{
    STARTUPINFO startupInfo = { sizeof( STARTUPINFOA ), 0, 0, 0,
        (DWORD)CW_USEDEFAULT, (DWORD)CW_USEDEFAULT, (DWORD)CW_USEDEFAULT, (DWORD)CW_USEDEFAULT,
        0, 0, 0,
        0,
        0, 0, 0,
        0, 0, 0
    };
 
    if( !CreateProcessA( 0,                             // lpApplicationName
                 (LPSTR)commandLine_.c_str(),           // lpCommandLine
                 0,                                     // lpProcessAttributes
                 0,                                     // lpThreadAttributes
                 TRUE,                                  // bInheritHandles
                 CREATE_NEW_CONSOLE,                    // dwCreationFlags
                 0,                                     // lpEnvironment
                 workingDir_.c_str(),                   // lpCurrentDirectory
                 &startupInfo,                          // lpStartupInfo
                 &internal_->pid_ ) )                   // lpProcessInformation
    {
        const std::string error( "Could not start process: " + commandLine_ + " error: " + GetLastErrorMessage() + "\"" );
        throw std::runtime_error( error );
    }
    static const std::string JOB_NAME = "";
    if ( HANDLE jobObject = OpenJobObject( JOB_OBJECT_ALL_ACCESS, TRUE, JOB_NAME.c_str() ) )
        AssignProcessToJobObject( jobObject, internal_->pid_.hProcess );
}

namespace 
{
    BOOL CALLBACK CloseWndProc( HWND hwnd, LPARAM /*lParam*/ )
    {
        if( IsWindow( hwnd ) )
        {
            DWORD_PTR result;
            SendMessageTimeout( hwnd, WM_CLOSE, 0, 0, SMTO_ABORTIFHUNG | SMTO_BLOCK, 0, &result );
        }
        return TRUE;
    }
}

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::Stop
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
void PluginProcessHandler::Stop()
{
    if( internal_.get() && internal_->pid_.hProcess )
    {
        HANDLE hThreadSnap = INVALID_HANDLE_VALUE;
        THREADENTRY32 te32;

        // Take a snapshot of all running threads
        hThreadSnap = CreateToolhelp32Snapshot( TH32CS_SNAPTHREAD, internal_->pid_.dwThreadId );
        if( hThreadSnap == INVALID_HANDLE_VALUE )
            return;
        te32.dwSize = sizeof( THREADENTRY32 );
        if( !Thread32First( hThreadSnap, &te32 ) )
        {
            CloseHandle( hThreadSnap ); // Must clean up the snapshot object!
            return;
        }
        do
        {
            if( te32.th32OwnerProcessID == internal_->pid_.dwProcessId )
                EnumThreadWindows( te32.th32ThreadID, &::CloseWndProc, 0 );
        }
        while( Thread32Next( hThreadSnap, &te32 ) );
        EnumThreadWindows( internal_->pid_.dwThreadId, &::CloseWndProc, 0 );
        TerminateProcess( internal_->pid_.hProcess, 0 );
    }
}
    /*
      --sword.server arg (=localhost:10001) Sword server (host:port)
      --sword.data arg                      Sword data directory
      --sword.exercise arg                  Sword exercise name
      --sword.profile arg (=Godseye)        Sword profile
      --sword.password arg                  Sword password
      --sword.waitconnection arg (=0)       Wait indefinitely for an available
                                            sword server connection.

      --bml.server.url arg                  BML server service url
      --bml.proxy.user arg                  BML proxy server user name
      --bml.proxy.pass arg                  BML proxy server password

      --bml.update.frequency arg (=60s)     BML poll frequency ?
      --bml.ssl arg (=1)                    Use SSL to connect to BML service

      --log arg (=1)                        Activate log
      --log.file arg                        Log to file

      --ini arg                             load options from ini file (toutes
    ces options sont écrites et chargées dans un .ini)
    */


// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::LoadSimulationConfig
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
void PluginProcessHandler::LoadSimulationConfig( const PluginConfig& config )
{
    AddArgument( "--sword.server=\"" + config.GetSimulationHost() + "\"" );
    AddArgument( "--sword.exercise=\"" + config.GetExerciseName() + "\"" );
    AddArgument( "--sword.data-dir=\"" + config.GetPhysicalDir() + "\"" );
}

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::LoadLoginProfile
// Created: JCR 2011-11-24
// -----------------------------------------------------------------------------
void PluginProcessHandler::LoadLoginProfile()
{
    std::string profile;
    std::string password;
    
    config_->LoadProfile( profile, password );
    if( profile.empty() )
    {
        AddArgument( "--sword.profile=\"anonymous\"" );
        AddArgument( "--sword.password=\"\"" );
    }
    else
    {
        AddArgument( "--sword.profile=\"" + profile + "\"" );
        AddArgument( "--sword.password=\"" + password + "\"" );
    }
}


// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::LoadPluginConfig
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
void PluginProcessHandler::LoadPluginConfig( xml::xistream& xis, const PluginConfig& config )
{
    bool use_ssl = false;
    bool use_log = false;
    bool use_report = false;
    std::string url;
    std::string proxy_user;
    std::string proxy_pass;
    std::string report_frequency;

    // Load Server Info
    xis >> xml::start( "server" )
            >> xml::attribute( "url", url )
            >> xml::optional >> xml::attribute( "ssl", use_ssl )
            >> xml::optional >> xml::attribute( "log", use_log )
            >> xml::start( "proxy" )
                >> xml::attribute( "user", proxy_user )
                >> xml::attribute( "pass", proxy_pass )
            >> xml::end
        >> xml::end
        >> xml::start( "reports" )
            >> xml::attribute( "activate", use_report )
            >> xml::optional >> xml::attribute( "frequency", report_frequency )
        >> xml::end;
    
    std::string sessionDir( config.GetSessionDir() );
    AddArgument( "--bml.server.url=\"" + url + "\"" );
    AddArgument( "--bml.ssl=" + std::string( use_ssl ? "true" : "false" ) );
    AddArgument( "--log=" + std::string( use_log ? "true" : "false" ) );
    if( use_log )
        AddArgument( "--log.file=\"" + bfs::path( sessionDir + "/sword_bml_service.log" ).native_file_string() + "\"" );
    AddArgument( "--bml.proxy.user=\"" + proxy_user + "\"" );
    AddArgument( "--bml.proxy.pass=\"" + proxy_pass + "\"" );
    if( use_report )
        AddArgument( "--bml.update.frequency=" + report_frequency );
}

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::AddArgument
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
void PluginProcessHandler::AddArgument( const std::string& arg )
{
    if( !commandLine_.empty() )
        commandLine_ += ' ';
    commandLine_ += arg;
}
