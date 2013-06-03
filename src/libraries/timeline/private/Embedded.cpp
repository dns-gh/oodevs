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

#include <tools/IpcDevice.h>
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

    void StopClient( ipc::Device& device )
    {
        std::vector< uint8_t > quit( controls::QuitClient( 0, 0 ) );
        controls::QuitClient( &quit[0], quit.size() );
        device.TimedWrite( &quit[0], quit.size(), boost::posix_time::seconds( 4 ) );
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
        External( ipc::Device& device )
            : device_( device )
        {
            // NOTHING
        }

        virtual ~External()
        {
            StopClient( device_ );
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
            const auto join = list.join( " " ).toStdWString();
            std::vector< wchar_t > args;
            args.push_back( L' ' );
            std::copy( join.begin(), join.end(), std::back_inserter( args ) );
            args.push_back( 0 );
            return args;
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
        boost::shared_ptr< void > process_;
        boost::shared_ptr< void > job_;
    };

#ifdef USE_EMBEDDED_CORE
    class Internal : public Embedded_ABC
    {
    public:
        Internal( ipc::Device& device )
            : device_( device )
        {
            // NOTHING
        }

        virtual ~Internal()
        {
            StopClient( device_ );
            thread_->join();
        }

        virtual void Start( const Configuration& cfg, const std::string& uuid )
        {
            core::Configuration next;
            next.wid = reinterpret_cast< int >( cfg.widget->winId() );
            next.uuid = uuid;
            next.url = cfg.url;
            next.debug_port = cfg.debug_port;
            client_ = core::MakeClient( next );
            thread_.reset( new boost::thread( &core::Client_ABC::Run, client_.get() ) );
        }

    private:
        ipc::Device& device_;
        std::auto_ptr< timeline::core::Client_ABC > client_;
        std::auto_ptr< boost::thread > thread_;
    };
#endif
}

std::auto_ptr< Embedded_ABC > Embedded_ABC::Factory( ipc::Device& device, bool external )
{
#ifdef USE_EMBEDDED_CORE
    if( !external )
        return std::auto_ptr< Embedded_ABC >( new Internal( device ) );
#endif
    if( !external )
        throw std::runtime_error( "not implemented" );
    return std::auto_ptr< Embedded_ABC >( new External( device ) );
}