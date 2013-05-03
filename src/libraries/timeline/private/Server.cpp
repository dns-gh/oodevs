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
#include <boost/thread/thread.hpp>

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
            device_.TryWrite( &resize_[0], resize_.size() );
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
    , device_  ( new ipc::Device( uuid_, true, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , embedded_( Embedded_ABC::Factory( *device_, cfg.external ) )
{
    auto layout = new QVBoxLayout( cfg.widget );
    auto widget = new Widget( *device_, cfg.widget );
    layout->addWidget( widget );
    layout->setContentsMargins( 0, 0, 0, 0 );
    embedded_->Start( cfg_, uuid_ );
}

Server::~Server()
{
    embedded_.reset();
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
    device_->Write( &buffer[0], buffer.size(), boost::posix_time::seconds( 1 ) );
}

bool Server::CreateEvent( const Event& event )
{
    std::vector< uint8_t > buffer( controls::CreateEvent( 0, 0, event ) );
    controls::CreateEvent( &buffer[0], buffer.size(), event );
    return device_->Write( &buffer[0], buffer.size(), boost::posix_time::seconds( 1 ) );
}