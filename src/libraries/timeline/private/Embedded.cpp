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
#include <boost/thread.hpp>

#ifndef _WIN64
#define USE_EMBEDDED_CORE
#endif

#ifdef USE_EMBEDDED_CORE
#include <timeline_core/api.h>
#endif

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
        device.Write( &quit[0], quit.size(), boost::posix_time::seconds( 4 ) );
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
            process_.waitForFinished( 4*1000 );
            process_.kill();
        }

        virtual void Start( const Configuration& cfg, const std::string& uuid )
        {
            connect( &process_, SIGNAL( error( QProcess::ProcessError ) ), SIGNAL( Error( QProcess::ProcessError ) ) );
            process_.setWorkingDirectory( FromPath( cfg.rundir ) );
            QStringList args;
            args << QString::number( reinterpret_cast< int >( cfg.widget->winId() ) )
                 << QString::fromStdString( uuid )
                 << QString::fromStdString( cfg.url );
            if( cfg.debug_port )
                args << "--debug_port" << QString::number( cfg.debug_port );
            process_.start( FromPath( cfg.binary ), args );
        }

    private:
        ipc::Device& device_;
        QProcess     process_;
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
            next.single_process = false;
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