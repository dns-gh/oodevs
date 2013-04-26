// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Context.h"

#include <tools/IpcDevice.h>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/thread/thread.hpp>

#include "moc_Context.cpp"

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <QtGui>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

using namespace timeline::ui;
namespace ipc = tools::ipc;

namespace
{
    class Widget : public QWidget
    {
    public:
        Widget( ipc::Device& device, QWidget* parent )
            : QWidget( parent )
            , device_( device )
        {
            // NOTHING
        }

    protected:
        virtual void resizeEvent( QResizeEvent* event )
        {
            char token = 0;
            device_.TryWrite( &token, sizeof token );
            QWidget::resizeEvent( event );
        }

    private:
        ipc::Device& device_;
    };

    QString FromPath( const tools::Path& path )
    {
        return QString::fromStdWString( path.ToUnicode() );
    }
}

Context::Context( const Configuration& cfg )
    : cfg_   ( cfg )
    , uuid_  ( boost::lexical_cast< std::string >( boost::uuids::random_generator()() ) )
    , device_( new ipc::Device( uuid_, true, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
    , core_  ( new QProcess() )
{
    auto layout = new QVBoxLayout( cfg.widget );
    auto widget = new Widget( *device_, cfg.widget );
    layout->addWidget( widget );
    layout->setContentsMargins( 0, 0, 0, 0 );
    QPalette palette;
    palette.setColor( QPalette::Background, Qt::red );
    widget->setAutoFillBackground( true );
    widget->setPalette( palette );
    StartProcess();
}

std::auto_ptr< Context_ABC > timeline::ui::MakeContext( const Configuration& cfg )
{
    return std::auto_ptr< Context_ABC >( new Context( cfg ) );
}

Context::~Context()
{
    if( !core_.get() )
        return;
    core_->terminate();
    core_->waitForFinished( 4000 );
    core_->kill();
    core_.reset();
}

void Context::StartProcess()
{
    connect( core_.get(), SIGNAL( error( QProcess::ProcessError ) ), SLOT( OnError( QProcess::ProcessError ) ) );
    core_->setWorkingDirectory( FromPath( cfg_.rundir ) );
    QStringList args;
    args << QString::number( reinterpret_cast< int >( cfg_.widget->winId() ) )
         << QString::fromStdString( uuid_ )
         << QString::fromStdString( cfg_.target );
    qDebug() << args;
    core_->start( FromPath( cfg_.binary ), args );
}

namespace
{
    const char* GetError( QProcess::ProcessError error )
    {
        switch( error )
        {
            case QProcess::FailedToStart: return "QProcess:FailedToStart";
            case QProcess::Crashed:       return "QProcess::Crashed";
            case QProcess::Timedout:      return "QProcess::Timedout";
            case QProcess::WriteError:    return "QProcess::WriteError";
            case QProcess::ReadError:     return "QProcess::ReadError";
            case QProcess::UnknownError:  return "QProcess::UnknownError";
        }
        return "Unknown process error";
    }
}

void Context::OnError( QProcess::ProcessError error )
{
    qDebug() << GetError( error );
}