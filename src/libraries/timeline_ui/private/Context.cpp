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
#include <boost/date_time/posix_time/posix_time_duration.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

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
        {
            // NOTHING
        }

    protected:
        virtual void resizeEvent( QResizeEvent* event )
        {
            char token = 0;
            device_.Write( &token, sizeof token, boost::posix_time::time_duration() );
            QWidget::resizeEvent( event );
        }

    private:
        ipc::Device& device_;
    };

    std::string MakeUuid()
    {
        return boost::lexical_cast< std::string >( boost::uuids::random_generator()() );
    }
}

Context::Context( const Configuration& cfg )
    : device_( new ipc::Device( MakeUuid(), true, ipc::DEFAULT_MAX_PACKETS, ipc::DEFAULT_MAX_PACKET_SIZE ) )
{
    auto layout = new QVBoxLayout( cfg.widget );
    auto widget = new Widget( *device_, cfg.widget );
    layout->addWidget( widget );
    layout->setContentsMargins( 0, 0, 0, 0 );
    QPalette palette;
    palette.setColor( QPalette::Background, Qt::red );
    widget->setAutoFillBackground( true );
    widget->setPalette( palette );
}

std::auto_ptr< Context_ABC > timeline::MakeContext( const Configuration& cfg )
{
    return std::auto_ptr< Context_ABC >( new Context( cfg ) );
}

Context::~Context()
{
    // NOTHING
}
