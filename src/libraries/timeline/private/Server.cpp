// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Server.h"
#include "moc_Server.cpp"

#include "Browser.h"

#include <tools/StdFileWrapper.h>

#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/lock_guard.hpp>
#include <boost/make_shared.hpp>

#ifdef _MSC_VER
#pragma warning( push, 0 )
#endif
#include <QtGui>
#ifdef _MSC_VER
#pragma warning( pop )
#endif

using namespace timeline;
namespace bpt = boost::posix_time;

namespace
{
    class Widget : public QWidget
    {
    public:
        Widget( Server& server, QWidget* parent )
            : QWidget( parent )
            , server_( server )
        {
            // NOTHING
        }

    protected:
        virtual void resizeEvent( QResizeEvent* event )
        {
            server_.Resize();
            QWidget::resizeEvent( event );
        }

    private:
        Server& server_;
    };
}

Server::Server( const Configuration& cfg )
    : cfg_    ( cfg )
    , logger_ ( cfg.server_log.IsEmpty() ? T_Logger() : [&]( const std::string& msg ){ Log( msg ); } )
    , frame_  ( new Widget( *this, cfg.widget ) )
    , browser_( new Browser( *this, frame_->winId(), cfg.url ) )
    , lock_   ( new boost::mutex() )
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
    layout->addWidget( frame_ );
    layout->setContentsMargins( 0, 0, 0, 0 );
}

Server::~Server()
{
    SignalWaiter waiter;
    connect( this, SIGNAL( Done() ), &waiter, SLOT( Signal() ) );
    browser_ = nullptr;
    delete frame_;
    // wait for all queued signals to be processed from current Qt event-loop
    // we do that by using a last signal from our thread
    while( !waiter.IsSignaled() )
        QCoreApplication::processEvents( QEventLoop::WaitForMoreEvents );
}

void Server::Start()
{
    browser_->Start();
}

void Server::Log( const std::string& msg )
{
    boost::lock_guard< boost::mutex > lock( *lock_ );
    if( log_ )
        *log_ << bpt::to_simple_string( bpt::second_clock::local_time() )
              << " " << msg << std::endl;
}

void Server::Reload()
{
    browser_->Reload();
}

void Server::Load( const std::string& url )
{
    browser_->Load( url );
}

void Server::Resize()
{
    browser_->Resize();
}

void Server::Quit()
{
    emit Done();
}

void Server::Center()
{
    browser_->Post( controls::CenterClient( logger_ ) );
}

void Server::UpdateQuery( const std::map< std::string, std::string >& parameters )
{
    browser_->Post( controls::UpdateQuery( logger_, parameters ) );
}

void Server::CreateEvents( const Events& events )
{
    browser_->Post( controls::CreateEvents( logger_, events ) );
}

void Server::SelectEvent( const std::string& uuid )
{
    browser_->Post( controls::SelectEvent( logger_, uuid ) );
}

void Server::ReadEvents()
{
    browser_->Post( controls::ReadEvents( logger_ ) );
}

void Server::ReadEvent( const std::string& uuid )
{
    browser_->Post( controls::ReadEvent( logger_, uuid ) );
}

void Server::UpdateEvent( const Event& event )
{
    browser_->Post( controls::UpdateEvent( logger_, event ) );
}

void Server::DeleteEvents( const std::vector< std::string >& uuids )
{
    browser_->Post( controls::DeleteEvents( logger_, uuids ) );
}

void Server::LoadEvents( const std::string& events )
{
    browser_->Post( controls::LoadEvents( logger_, events ) );
}

void Server::SaveEvents() const
{
    browser_->Post( controls::SaveEvents( logger_ ) );
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
