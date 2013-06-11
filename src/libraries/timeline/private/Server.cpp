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
    std::vector< uint8_t > buffer( controls::ReloadClient( 0, 0 ) );
    controls::ReloadClient( &buffer[0], buffer.size() );
    write_->Write( &buffer[0], buffer.size() );
}

void Server::Load( const std::string& url )
{
    std::vector< uint8_t > buffer( controls::LoadClient( 0, 0, url ) );
    controls::LoadClient( &buffer[0], buffer.size(), url );
    write_->Write( &buffer[0], buffer.size() );
}

bool Server::CreateEvent( const Event& event )
{
    if( !event.IsValid() )
        return false;
    std::vector< uint8_t > buffer( controls::CreateEvent( 0, 0, event ) );
    controls::CreateEvent( &buffer[0], buffer.size(), event );
    return write_->Write( &buffer[0], buffer.size() );
}

bool Server::DeleteEvent( const std::string& uuid )
{
    std::vector< uint8_t > buffer( controls::DeleteEvent( 0, 0, uuid ) );
    controls::DeleteEvent( &buffer[0], buffer.size(), uuid );
    return write_->Write( &buffer[0], buffer.size() );
}

void Server::Run()
{
    try
    {
        std::vector< uint8_t > buffer( ipc::DEFAULT_MAX_PACKET_SIZE );
        while( !thread_->interruption_requested() )
            if( const size_t read = read_->TimedRead( &buffer[0], buffer.size(), boost::posix_time::milliseconds( 50 ) ) )
                controls::ParseServer( *this, &buffer[0], read );
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

void Server::OnSelectedEvent( const Event& event )
{
    emit SelectedEvent( boost::make_shared< Event >( event ) );
}

void Server::OnDeselectedEvent()
{
    emit SelectedEvent( boost::shared_ptr< Event >() );
}

void Server::OnDeletedEvent( const std::string& uuid, const Error& error )
{
    emit DeletedEvent( uuid, error );
}

void Server::OnActivatedEvent( const Event& event )
{
    emit ActivatedEvent( event );
}

void Server::OnContextMenuEvent( const Event& event )
{
    emit ContextMenuEvent( boost::make_shared< Event >( event ) );
}

void Server::OnContextMenuBackground()
{
    emit ContextMenuEvent( boost::shared_ptr< Event >() );
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
