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

class PluginProcessHandler::ProfileInfo
{
public:
	explicit PluginProcessHandler::ProfileInfo( const std::string& path ) 
		: config_path_ ( path ) {}
	
	void LoadProfile( dispatcher::Logger_ABC& logger )
	{
		/* TODO concurrency with gaming application */
		try 
		{
			xml::xifstream xif( config_path_ );
			xif >> xml::start( "profiles" )
				>> xml::list( "profile", boost::bind( &ProfileInfo::ReadProfile, *this, _1 ) );
		}
		catch( const xml::exception& e ) 
		{
			logger.LogWarning( LOG_MESSAGE( e.what() ) );
		}
		catch( ... )
		{
			logger.LogWarning( LOG_MESSAGE( "Unable to load profile information. Try to load anonymous profile." ) );
		}
	}

private:
	void ReadProfile( xml::xistream& xis )
    {
        if ( profile_.empty() && xis.attribute< bool >( "supervision" ) )
        {
            xis >> xml::attribute( "name", profile_ )
                >> xml::attribute( "password", password_ );
        }
    }

private:
	ProfileInfo& operator=( const ProfileInfo& );

public:
	std::string profile_;
	std::string password_;

private:
	const std::string config_path_;
};

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler constructor
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
PluginProcessHandler::PluginProcessHandler( const dispatcher::Config& config, const std::string& process_name, dispatcher::Logger_ABC& logger, xml::xistream& xis )
    : logger_ ( logger )
	, processName_ ( process_name )
	, workingDir_  ( config.BuildPluginDirectory( "bml" ) )
    , commandLine_ ( workingDir_ + "/" + processName_ )
	, config_	   ( config )
	, profile_     ( new ProfileInfo( config_.GetProfilesFile() ) )
	, internal_    ()
{
	LoadSimulationConfig( config );
	LoadPluginConfig( xis, config );
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

namespace 
{
	struct LocalLogger : public dispatcher::Logger_ABC
	{
		void LogInfo( const std::string& message )
		{
			std::fstream out( "bml.log", std::ios_base::out | std::ios_base::app );
			if ( !out.bad() )
				out << "Info " << message << std::endl;
			out.close();
		}

		void LogError( const std::string& message ) 
		{
			std::fstream out( "bml.log", std::ios_base::out | std::ios_base::app );
			if ( !out.bad() )
				out << "Error " << message << std::endl;
			out.close();
		}
		
		void LogWarning( const std::string& message )
		{
			std::fstream out( "bml.log", std::ios_base::out | std::ios_base::app );
			if ( !out.bad() )
				out << "Warning " << message << std::endl;
			out.close();
		}
	};
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


#define TODO std::string( "" )

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::LoadSimulationConfig
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
void PluginProcessHandler::LoadSimulationConfig( const dispatcher::Config& config )
{
    std::string network( config.GetNetworkSimulationParameters() );
	std::string server( network.substr( 0, network.find( ':' ) ) + ":" + 
                            boost::lexical_cast< std::string >( config.GetNetworkClientsParameters() ) );

	AddArgument( "--sword.server=\"" + server + "\"" );
	AddArgument( "--sword.exercise=\"" + config.GetExerciseName() + "\"" );
	AddArgument( "--sword.data-dir=\"" + bfs::path( config.GetPhysicalFile(), bfs::native ).branch_path().string() + "\"" );
}

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::LoadLoginProfile
// Created: JCR 2011-11-24
// -----------------------------------------------------------------------------
void PluginProcessHandler::LoadLoginProfile()
{
	profile_->LoadProfile( logger_ );
	if( profile_.get() || profile_->profile_.empty() )
    {
        AddArgument( "--sword.profile=\"anonymous\"" );
        AddArgument( "--sword.password=\"\"" );
    }
    else
    {
        AddArgument( "--sword.profile=\"" + profile_->profile_ + "\"" );
        AddArgument( "--sword.password=\"" + profile_->password_ + "\"" );
    }
}

// -----------------------------------------------------------------------------
// Name: PluginProcessHandler::LoadPluginConfig
// Created: JCR 2011-10-27
// -----------------------------------------------------------------------------
void PluginProcessHandler::LoadPluginConfig( xml::xistream& xis, const dispatcher::Config& config )
{
	bool use_ssl = false;
	bool use_log = true;
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
	if( use_ssl )
		AddArgument( "--bml.ssl=true" );
	if( use_log )
	{
		AddArgument( "--log=true" );
		AddArgument( "--log.file=\"" + bfs::path( sessionDir + "/sword_bml_service.log" ).native_file_string() + "\"" );
	}
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
