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
#include <windows.h>
#include <winbase.h>
#include <tlhelp32.h>
#include <xeumeuleu/xml.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem/operations.hpp>

using namespace plugins::bml;
namespace bfs = boost::filesystem;

struct PluginProcessHandler::InternalData
{
    PROCESS_INFORMATION pid_ ;
    InternalData() { ZeroMemory( &pid_ , sizeof( pid_ ) ); }
};

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler constructor
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
PluginProcessHandler::PluginProcessHandler( const dispatcher::Config& config, const std::string& process_name, xml::xistream& xis )
    : commandLine_  ( process_name )
    , workingDir_   ( "." )
    , internal_     ( new InternalData() )
{
    LoadSimulationConfig( config );
    LoadPluginConfig( xis, config );
    Start();
}
            

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler destructor
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
PluginProcessHandler::~PluginProcessHandler()
{
    Stop();
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
        DWORD errCode = GetLastError();
        const std::string error( "[BML::ProcessHandler]: Could not start process: " + commandLine_ + " error: " + boost::lexical_cast< std::string >( (unsigned long)errCode ) + "\"" );
        throw std::exception( error.c_str() );
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


#define TODO std::string( "" )

namespace 
{
    void ReadProfile( xml::xistream& xis, std::string& profile, std::string& password )
    {
        if ( profile.empty() && xis.attribute< bool >( "supervision" ) )
        {
            xis >> xml::attribute( "name", profile )
                >> xml::attribute( "password", password );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::LoadSimulationConfig
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
void PluginProcessHandler::LoadSimulationConfig( const dispatcher::Config& config )
{
    const std::string network( config.GetNetworkSimulationParameters() );
    std::string server = network.substr( 0, network.find( ':' ) ) + ":" + 
                            boost::lexical_cast< std::string >( config.GetNetworkClientsParameters() );
    AddArgument( "--sword.server=\"" + server + "\"" );
    AddArgument( "--sword.exercise=\"" + config.GetExerciseName() + "\"" );
        
    std::string profile;
    std::string password;
    try 
    {
        xml::xifstream xif( config.GetProfilesFile() );
        xif >> xml::start( "profiles" )
            >> xml::list( "profile", boost::bind( &::ReadProfile, _1, boost::ref( profile ), boost::ref( password ) ) );
    }
    catch( ... )
    {
        // NOTHING
    }
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
void PluginProcessHandler::LoadPluginConfig( xml::xistream& xis, const dispatcher::Config& config )
{
    // Load Server Info
    xis >> xml::start( "server" );
    {
        AddArgument( "--bml.server.url=\"" + xis.attribute< std::string >( "url" ) + "\"" );
        
        bool use_ssl = false;
        xis >> xml::optional >> xml::attribute( "ssl", use_ssl );
        if( use_ssl )
            AddArgument( "--bml.ssl=true" );
        
        bool use_log = true;
        xis >> xml::optional >> xml::attribute( "log", use_log );
        if( use_log )
        {
            AddArgument( "--log=true" );
            AddArgument( "--log.file=\"" + bfs::path( config.GetSessionDir() + "/sword_bml_service.log" ).native_file_string() + "\"" );
        }
        xis >> xml::start( "proxy" );
        AddArgument( "--bml.proxy.user=\"" + xis.attribute< std::string >( "user" ) + "\"" );
        AddArgument( "--bml.proxy.pass=\"" + xis.attribute< std::string >( "pass" ) + "\"" );
        xis >> xml::end;
    }
    xis >> xml::end
        >> xml::start( "reports" );

    bool use_report = false;
    xis >> xml::optional >> xml::attribute( "activate", use_report );

    if( use_report )
        AddArgument( "--bml.update.frequency=" + xis.attribute< std::string >( "frequency" ) );
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
