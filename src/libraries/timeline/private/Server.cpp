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
#include <boost/bind.hpp>
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
namespace ipc = tools::ipc;

namespace
{
    class Widget : public QWidget
    {
    public:
        Widget( ipc::Device& device, QWidget* parent )
            : QWidget( parent )
            , device_( device )
            , resize_( controls::ResizeClient( 0, 0 ) )
        {
            controls::ResizeClient( &resize_[0], resize_.size() );
        }

    protected:
        virtual void resizeEvent( QResizeEvent* event )
        {
            try
            {
                device_.TryWrite( &resize_[0], resize_.size() );
            }
            catch( ... )
            {
                // NOTHING
            }
            QWidget::resizeEvent( event );
        }

    private:
        ipc::Device& device_;
        std::vector< uint8_t > resize_;
    };
}

Server::Server( const Configuration& cfg )
    : cfg_     ( cfg )
    , uuid_    ( boost::lexical_cast< std::string >( boost::uuids::random_generator()() ) )
    , write_   ( new ipc::Device( uuid_ + "_write", true, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , read_    ( new ipc::Device( uuid_ + "_read",  true, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , embedded_( Embedded_ABC::Factory( *write_, cfg.external ) )
{
    qRegisterMetaType< boost::shared_ptr< Event > >( "boost::shared_ptr< timeline::Event >" );
    qRegisterMetaType< std::string >( "std::string" );
    qRegisterMetaType< Event >( "timeline::Event" );
    qRegisterMetaType< Events >( "timeline::Events" );
    qRegisterMetaType< Error >( "timeline::Error" );
    auto layout = new QVBoxLayout( cfg.widget );
    auto widget = new Widget( *write_, cfg.widget );
    layout->addWidget( widget );
    layout->setContentsMargins( 0, 0, 0, 0 );
    embedded_->Start( cfg_, uuid_ );
    thread_.reset( new boost::thread( &Server::Run, this ) );
}

Server::~Server()
{
    embedded_.reset();
    thread_->interrupt();
    thread_->join();
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

void Server::OnError( QProcess::ProcessError error )
{
    qDebug() << GetError( error );
}

void Server::Reload()
{
    Write( *write_, &controls::ReloadClient );
}

void Server::Load( const std::string& url )
{
    Write( *write_, boost::bind( &controls::LoadClient, _1, _2, url ) );
}

void Server::Center()
{
    Write( *write_, &controls::CenterClient );
}

void Server::UpdateQuery( const std::map< std::string, std::string >& parameters )
{
    Write( *write_, boost::bind( &controls::UpdateQuery, _1, _2, boost::cref( parameters ) ) );
}

bool Server::CreateEvent( const Event& event )
{
    if( !event.IsValid() )
        return false;
    return Write( *write_, boost::bind( &controls::CreateEvent, _1, _2, event ) );
}

bool Server::ReadEvents()
{
    return Write( *write_, static_cast< size_t ( * )( void*, size_t ) >( &controls::ReadEvents ) );
}

bool Server::ReadEvent( const std::string& uuid )
{
    return Write( *write_, boost::bind( &controls::ReadEvent, _1, _2, uuid ) );
}

bool Server::UpdateEvent( const Event& event )
{
    if( !event.IsValid() )
        return false;
    return Write( *write_, boost::bind( &controls::UpdateEvent, _1, _2, event ) );
}

bool Server::DeleteEvent( const std::string& uuid )
{
    return Write( *write_, boost::bind( &controls::DeleteEvent, _1, _2, uuid ) );
}

void Server::LoadEvents( const std::string& events )
{
    Write( *write_, boost::bind( &controls::LoadEvents, _1, _2, events ) );
}

void Server::SaveEvents() const
{
    Write( *write_, &controls::SaveEvents );
}

void Server::Run()
{
    try
    {
        std::vector< uint8_t > buffer( ipc::DEFAULT_MAX_PACKET_SIZE );
        while( !thread_->interruption_requested() )
            if( const size_t read = read_->TimedRead( &buffer[0], buffer.size(), boost::posix_time::milliseconds( 50 ) ) )
            {
                if( read > buffer.size() )
                    buffer.resize( read );
                else
                {
                    controls::ParseServer( *this, &buffer[0], read );
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

void Server::OnCreatedEvent( const Event& event, const Error& error )
{
    emit CreatedEvent( event, error );
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

void Server::OnDeletedEvent( const std::string& uuid, const Error& error )
{
    emit DeletedEvent( uuid, error );
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
