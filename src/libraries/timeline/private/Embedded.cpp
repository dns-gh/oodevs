// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************
#include "Embedded_ABC.h"
#include "moc_embedded_abc.cpp"

#include "controls/controls.h"

#include <tools/Helpers.h>
#include <tools/IpcDevice.h>
#include <tools/Path.h>
#include <QWidget>
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread.hpp>

#ifdef USE_EMBEDDED_CORE
#include <timeline_core/api.h>
#endif

#include <windows.h>

using namespace timeline;
namespace ipc = tools::ipc;

namespace
{
    QString FromPath( const tools::Path& path )
    {
        return QString::fromStdWString( path.ToUnicode() );
    }

    void StopClient( ipc::Device& device, const controls::T_Logger& log )
    {
        controls::QuitClient( device, log, 4*1000 );
    }

    std::runtime_error Win32Exception( const std::string& err )
    {
        return std::runtime_error( err + " winapi error #" + boost::lexical_cast< std::string >( GetLastError() ) );
    }

    boost::shared_ptr< void > MakeAutoKill( HANDLE process )
    {
        HANDLE job = CreateJobObject( NULL, NULL );
        if( !job )
            throw Win32Exception( "unable to create job object" );
        boost::shared_ptr< void > rpy( job, CloseHandle );
        JOBOBJECT_EXTENDED_LIMIT_INFORMATION jeli;
        memset( &jeli, 0, sizeof jeli );
        jeli.BasicLimitInformation.LimitFlags = JOB_OBJECT_LIMIT_KILL_ON_JOB_CLOSE;
        bool valid = SetInformationJobObject( job, JobObjectExtendedLimitInformation, &jeli, sizeof jeli );
        if( !valid )
            throw Win32Exception( "unable to set information job object" );
        valid = AssignProcessToJobObject( job, process );
        if( !valid )
            throw Win32Exception( "unable to assign process to job object" );
        return rpy;
    }

    class External : public Embedded_ABC
    {
    public:
        External( ipc::Device& device, const controls::T_Logger& log )
            : device_( device )
            , log_   ( log )
        {
            // NOTHING
        }

        virtual ~External()
        {
            StopClient( device_, log_ );
            WaitForSingleObjectEx( process_.get(), 4*1000, false );
            TerminateProcess( process_.get(), static_cast< unsigned >( -1 ) );
        }

        std::vector< wchar_t > GetArguments( const Configuration& cfg, const std::string& uuid )
        {
            QStringList list;
            list << QString::number( reinterpret_cast< int >( cfg.widget->winId() ) )
                 << QString::fromStdString( uuid )
                 << QString::fromStdString( cfg.url );
            if( cfg.debug_port )
                list << "--debug_port" << QString::number( cfg.debug_port );
            if( !cfg.client_log.IsEmpty() )
                list << "--log" << QString::fromStdString( cfg.client_log.ToUTF8() );
            std::vector< std::wstring > args;
            args.push_back( cfg.binary.ToUnicode() );
            for( auto it = list.begin(); it != list.end(); ++it )
                args.push_back( it->toStdWString() );
            return tools::ArgsToCommandLine( args );
        }

        virtual void Start( const Configuration& cfg, const std::string& uuid )
        {
            PROCESS_INFORMATION pi;
            STARTUPINFOW si;
            memset( &pi, 0, sizeof pi );
            memset( &si, 0, sizeof si );
            si.cb = sizeof si;
            si.dwFlags = STARTF_USESHOWWINDOW;
            si.wShowWindow = false;
            auto args = GetArguments( cfg, uuid );
            bool valid = CreateProcessW( cfg.binary.ToUnicode().c_str(),
                &args[0], NULL, NULL, false,
                CREATE_SUSPENDED | CREATE_BREAKAWAY_FROM_JOB,
                NULL, cfg.rundir.ToUnicode().c_str(), &si, &pi );
            if( !valid )
                throw Win32Exception( "unable to create process" );
            if( pi.hProcess )
                process_.reset( pi.hProcess, CloseHandle );
            boost::shared_ptr< void > thread;
            if( pi.hThread )
                thread.reset( pi.hThread, CloseHandle );
            job_ = MakeAutoKill( pi.hProcess );
            const int err = ResumeThread( pi.hThread );
            if( err == -1 )
                throw Win32Exception( "unable to resume thread" );
        }

    private:
        ipc::Device&              device_;
        const controls::T_Logger  log_;
        boost::shared_ptr< void > process_;
        boost::shared_ptr< void > job_;
    };

#ifdef USE_EMBEDDED_CORE
    class Internal : public Embedded_ABC
    {
    public:
        Internal( ipc::Device& device, const controls::T_Logger& log )
            : device_( device )
            , log_   ( log )
        {
            // NOTHING
        }

        virtual ~Internal()
        {
            StopClient( device_, log_ );
            thread_->join();
        }

        virtual void Start( const Configuration& cfg, const std::string& uuid )
        {
            core::Configuration next;
            next.wid = reinterpret_cast< int >( cfg.widget->winId() );
            next.uuid = uuid;
            next.url = cfg.url;
            next.debug_port = cfg.debug_port;
            next.log = cfg.client_log.ToUTF8();
            client_ = core::MakeClient( next, log_ );
            thread_.reset( new boost::thread( &core::Client_ABC::Run, client_.get() ) );
        }

    private:
        ipc::Device& device_;
        controls::T_Logger log_;
        std::auto_ptr< timeline::core::Client_ABC > client_;
        std::auto_ptr< boost::thread > thread_;
    };
#endif
}

std::auto_ptr< Embedded_ABC > Embedded_ABC::Factory( ipc::Device& device, const controls::T_Logger& log )
{
#ifdef USE_EMBEDDED_CORE
    return std::auto_ptr< Embedded_ABC >( new Internal( device, log ) );
#else
    return std::auto_ptr< Embedded_ABC >( new External( device, log ) );
#endif
}