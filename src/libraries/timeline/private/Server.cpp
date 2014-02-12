// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Server.h"

#include "Embedded_ABC.h"
#include "controls/controls.h"

#include <tools/IpcDevice.h>
#include <tools/StdFileWrapper.h>

#include <boost/bind.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/make_shared.hpp>
#include <boost/thread.hpp>

#include "moc_Server.cpp"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <QtGui>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

using namespace timeline;
namespace bpt = boost::posix_time;
namespace ipc = tools::ipc;

namespace
{
    class Widget : public QWidget
    {
    public:
        Widget( ipc::Device& device, const controls::T_Logger& log, QWidget* parent )
            : QWidget( parent )
            , device_( device )
            , log_   ( log )
        {
            controls::TryResizeClient( device_, log_ );
        }

    protected:
        virtual void resizeEvent( QResizeEvent* event )
        {
            try
            {
                controls::TryResizeClient( device_, log_ );
            }
            catch( ... )
            {
                // NOTHING
            }
            QWidget::resizeEvent( event );
        }

    private:
        ipc::Device& device_;
        const controls::T_Logger log_;
    };
}

Server::Server( const Configuration& cfg )
    : cfg_     ( cfg )
    , uuid_    ( boost::lexical_cast< std::string >( boost::uuids::random_generator()() ) )
    , logger_  ( cfg.server_log.IsEmpty() ? controls::T_Logger() : [&]( const std::string& msg ){ Log( msg, false ); } )
    , lock_    ( new boost::mutex() )
    , write_   ( new ipc::Device( uuid_ + "_write", true, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , read_    ( new ipc::Device( uuid_ + "_read",  true, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , embedded_( Embedded_ABC::Factory( *write_, logger_, cfg.external ).release() )
{
    if( !cfg_.server_log.IsEmpty() )
        log_.reset( new tools::Ofstream( cfg_.server_log, std::ios::out | std::ios::binary ) );
    qRegisterMetaType< boost::shared_ptr< Event > >( "boost::shared_ptr< timeline::Event >" );
    qRegisterMetaType< std::string >( "std::string" );
    qRegisterMetaType< std::vector< std::string > >( "std::vector< std::string >" );
    qRegisterMetaType< Event >( "timeline::Event" );
    qRegisterMetaType< Events >( "timeline::Events" );
    qRegisterMetaType< Error >( "timeline::Error" );
    auto layout = new QVBoxLayout( cfg.widget );
    auto widget = new Widget( *write_, logger_, cfg.widget );
    layout->addWidget( widget );
    layout->setContentsMargins( 0, 0, 0, 0 );
}

Server::~Server()
{
    embedded_.reset();
    thread_->interrupt();
    thread_->join();
}

void Server::Start()
{
    thread_.reset( new boost::thread( &Server::Run, this ) );
    embedded_->Start( cfg_, uuid_ );
}

namespace
{
    const char* GetError( QProcess::ProcessError error )
    {
        switch( error )
        {
            case QProcess::FailedToStart: return "QProcess::FailedToStart";
            case QProcess::Crashed:       return "QProcess::Crashed";
            case QProcess::Timedout:      return "QProcess::Timedout";
            case QProcess::WriteError:    return "QProcess::WriteError";
            case QProcess::ReadError:     return "QProcess::ReadError";
            case QProcess::UnknownError:  return "QProcess::UnknownError";
        }
        return "Unknown process error";
    }
}

void Server::Log( const std::string& msg )
{
    boost::lock_guard< boost::mutex > lock( *lock_ );
    if( log_ )
        *log_ << bpt::to_simple_string( bpt::second_clock::local_time() )
              << msg << std::endl;
}

void Server::Log( const std::string& msg, bool read )
{
    Log( ( read ? " read:  " : " write: " ) + msg );
}

void Server::OnError( QProcess::ProcessError error )
{
    const auto err = GetError( error );
    qDebug() << err;
    Log( std::string( "error: " ) + err );
}

void Server::Reload()
{
    controls::ReloadClient( *write_, logger_ );
}

void Server::Load( const std::string& url )
{
    controls::LoadClient( *write_, logger_, url );
}

void Server::Center()
{
    controls::CenterClient( *write_, logger_ );
}

void Server::UpdateQuery( const std::map< std::string, std::string >& parameters )
{
    controls::UpdateQuery( *write_, logger_, parameters );
}

bool Server::CreateEvents( const Events& events )
{
    for( auto it = events.begin(); it != events.end(); ++it )
        if( !it->IsValid() )
            return false;
    return controls::CreateEvents( *write_, logger_, events );
}

bool Server::SelectEvent( const std::string& uuid )
{
    return controls::SelectEvent( *write_, logger_, uuid );
}

bool Server::ReadEvents()
{
    return controls::ReadEvents( *write_, logger_ );
}

bool Server::ReadEvent( const std::string& uuid )
{
    return controls::ReadEvent( *write_, logger_, uuid );
}

bool Server::UpdateEvent( const Event& event )
{
    if( !event.IsValid() )
        return false;
    return controls::UpdateEvent( *write_, logger_, event );
}

bool Server::DeleteEvents( const std::vector< std::string >& uuids )
{
    return controls::DeleteEvents( *write_, logger_, uuids );
}

void Server::LoadEvents( const std::string& events )
{
    controls::LoadEvents( *write_, logger_, events );
}

void Server::SaveEvents() const
{
    controls::SaveEvents( *write_, logger_ );
}

void Server::Run()
{
    try
    {
        controls::T_Logger logger;
        if( log_ )
            logger = [&]( const std::string& msg ){ Log( msg, true ); };
        std::vector< uint8_t > buffer( ipc::DEFAULT_MAX_PACKET_SIZE );
        while( !thread_->interruption_requested() )
            if( const size_t read = read_->TimedRead( &buffer[0], buffer.size(), boost::posix_time::milliseconds( 50 ) ) )
            {
                if( read > buffer.size() )
                    buffer.resize( read );
                else
                {
                    controls::ParseServer( *this, &buffer[0], read, logger );
                    buffer.resize( ipc::DEFAULT_MAX_PACKET_SIZE );
                }
            }
    }
    catch( const boost::thread_interrupted& )
    {
        // NOTHING
    }
    catch( const std::exception& err )
    {
        qDebug() << err.what();
    }
    catch( ... )
    {
        qDebug() << "unexpected exception";
    }
}

void Server::OnReadyServer()
{
    emit Ready();
}

void Server::OnCreatedEvents( const Events& events, const Error& error )
{
    emit CreatedEvents( events, error );
}

void Server::OnReadEvents( const Events& events, const Error& error )
{
    emit GetEvents( events, error );
}

void Server::OnReadEvent( const Event& event, const Error& error )
{
    emit GetEvent( event, error );
}

void Server::OnUpdatedEvent( const Event& event, const Error& error )
{
    emit UpdatedEvent( event, error );
}

void Server::OnDeletedEvents( const std::vector< std::string >& uuids, const Error& error )
{
    emit DeletedEvents( uuids, error );
}

void Server::OnLoadedEvents( const Error& error )
{
    emit LoadedEvents( error );
}

void Server::OnSavedEvents( const std::string& events, const Error& error )
{
    emit SavedEvents( events, error );
}

void Server::OnSelectedEvent( const Event& event )
{
    emit SelectedEvent( boost::make_shared< Event >( event ) );
}

void Server::OnDeselectedEvent()
{
    emit SelectedEvent( boost::shared_ptr< Event >() );
}

void Server::OnActivatedEvent( const Event& event )
{
    emit ActivatedEvent( event );
}

void Server::OnContextMenuEvent( const Event& event )
{
    emit ContextMenuEvent( boost::make_shared< Event >( event ), event.begin );
}

void Server::OnContextMenuBackground( const std::string& time )
{
    emit ContextMenuEvent( boost::shared_ptr< Event >(), time );
}

void Server::OnKeyDown( int key )
{
    emit KeyDown( key );
}

void Server::OnKeyPress( int key )
{
    emit KeyPress( key );
}

void Server::OnKeyUp( int key )
{
    emit KeyUp( key );
}
