// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "TimelineDockWidget.h"
#include "moc_TimelineDockWidget.cpp"
#include "Config.h"
#include "clients_kernel/Tools.h"
#include "timeline/api.h"
#include "MT_Tools/MT_Logger.h"

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget constructor
// Created: ABR 2013-05-14
// -----------------------------------------------------------------------------
TimelineDockWidget::TimelineDockWidget( QWidget* parent, kernel::Controllers& controllers, const Config& config )
    : gui::RichDockWidget( controllers, parent, "timeline-dock-widget" )
    , cfg_( new timeline::Configuration() )
{
    setCaption( tr( "Actions timeline" ) );

    // Configuration
    cfg_->url = "http://" + config.GetTimelineUrl() + "/?lang=" + tools::readLang(); // $$$$ ABR 2013-05-24: Timeline server must keep this parameter when it automatically add session id
    cfg_->debug_port = config.GetTimelineDebugPort();
    cfg_->rundir = "cef";
    cfg_->binary = "cef/timeline_client.exe";
    cfg_->external = true;

    if( !cfg_->binary.IsRegularFile() )
        Error( tr( "Invalid timeline binary '%1'" ).arg( QString::fromStdWString( cfg_->binary.ToUnicode() ) ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget destructor
// Created: ABR 2013-05-14
// -----------------------------------------------------------------------------
TimelineDockWidget::~TimelineDockWidget()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Error
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineDockWidget::Error( const QString& text )
{
    MT_LOG_ERROR_MSG( text.toStdString() );
    //QMessageBox::error( 0, tr( "Error" ), QString::fromStdString( text ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Connect
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void TimelineDockWidget::Connect()
{
    mainWidget_ = new QWidget( this );
    mainWidget_->setObjectName( "timeline-widget" );
    setWidget( mainWidget_ );

    cfg_->widget = mainWidget_;

    server_ = MakeServer( *cfg_ );

    connect( this, SIGNAL( CreateEventSignal( const timeline::Event& ) ), server_.get(), SLOT( CreateEvent( const timeline::Event& ) ) );
    connect( this, SIGNAL( DeleteEventSignal( const std::string& ) ), server_.get(), SLOT( DeleteEvent( const std::string& ) ) );

    connect( server_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
    connect( server_.get(), SIGNAL( DeletedEvent( const std::string&, const timeline::Error& ) ), this, SLOT( OnDeletedEvent( const std::string&, const timeline::Error& ) ) );

    connect( server_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< timeline::Event > ) ) );
    connect( server_.get(), SIGNAL( ActivatedEvent( const timeline::Event& ) ), this, SLOT( OnActivatedEvent( const timeline::Event& ) ) );
    connect( server_.get(), SIGNAL( ContextMenuEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnContextMenuEvent( boost::shared_ptr< timeline::Event > ) ) );
    connect( server_.get(), SIGNAL( KeyUp( int ) ), this, SLOT( OnKeyUp( int ) ) );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::Disconnect
// Created: ABR 2013-05-15
// -----------------------------------------------------------------------------
void TimelineDockWidget::Disconnect()
{
    if( server_.get() )
    {
        disconnect( this, SIGNAL( CreateEventSignal( const timeline::Event& ) ), server_.get(), SLOT( CreateEvent( const timeline::Event& ) ) );
        disconnect( this, SIGNAL( DeleteEventSignal( const std::string& ) ), server_.get(), SLOT( DeleteEvent( const std::string& ) ) );

        disconnect( server_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
        disconnect( server_.get(), SIGNAL( DeletedEvent( const std::string&, const timeline::Error& ) ), this, SLOT( OnDeletedEvent( const std::string&, const timeline::Error& ) ) );

        disconnect( server_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< timeline::Event > ) ) );
        disconnect( server_.get(), SIGNAL( ActivatedEvent( const timeline::Event& ) ), this, SLOT( OnActivatedEvent( const timeline::Event& ) ) );
        disconnect( server_.get(), SIGNAL( ContextMenuEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnContextMenuEvent( boost::shared_ptr< timeline::Event > ) ) );
        disconnect( server_.get(), SIGNAL( KeyUp( int ) ), this, SLOT( OnKeyUp( int ) ) );

        server_.reset();
        delete mainWidget_;
        cfg_->widget = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::CreateEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineDockWidget::CreateEvent( const timeline::Event& event )
{
    creationRequestedEvents_.push_back( event.uuid );
    emit CreateEventSignal( event );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::DeleteEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineDockWidget::DeleteEvent( const std::string& uuid )
{
    deletionRequestedEvents_.push_back( uuid );
    emit DeleteEventSignal( uuid );
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnCreatedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnCreatedEvent( const timeline::Event& event, const timeline::Error& error )
{
    auto it = std::find( creationRequestedEvents_.begin(), creationRequestedEvents_.end(), event.uuid );
    if( it != creationRequestedEvents_.end() )
    {
        if( error.code != timeline::EC_OK )
            Error( tr( "An error occurred during event creation process: %1" ).arg( QString::fromStdString( error.text ) ) );
        creationRequestedEvents_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnDeletedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnDeletedEvent( const std::string& uuid, const timeline::Error& error )
{
    auto it = std::find( deletionRequestedEvents_.begin(), deletionRequestedEvents_.end(), uuid );
    if( it != deletionRequestedEvents_.end() )
    {
        if( error.code != timeline::EC_OK )
            Error( tr( "An error occurred during event deletion process: %1" ).arg( QString::fromStdString( error.text ) ) );
        deletionRequestedEvents_.erase( it );
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnSelectedEvent
// Created: ABR 2013-05-17
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnSelectedEvent( boost::shared_ptr< timeline::Event > event )
{
    selected_ = event;
    //if( selected_.get() )
    // {
    //      Transform Event.Action.payload to binary
    //      Transform binary into proto object
    //      Retrieve missions parameters
    //      Display them on map
    // }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnActivatedEvent
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnActivatedEvent( const timeline::Event& /*event*/ )
{
    // Display mission panel filled with this event
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnContextMenuEvent
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnContextMenuEvent( boost::shared_ptr< timeline::Event > event )
{
    QMenu menu;
    if( event )
    {
        const QString editText = tr( "Edit" );
        const QString deleteText = tr( "Delete" );
        menu.addAction( editText );
        menu.addAction( deleteText );
        if( QAction* resultingAction = menu.exec( QCursor::pos() ) )
        {
            if( event && resultingAction->text() == editText )
                OnActivatedEvent( *event );
            else if( event && resultingAction->text() == deleteText )
                DeleteEvent( event->uuid );
        }
    }
    else
    {
        QString plannedMissionText = tr( "Planned a mission" );
        menu.addAction( plannedMissionText );
        if( QAction* resultingAction = menu.exec( QCursor::pos() ) )
        {
            if( resultingAction->text() == plannedMissionText )
            {
                // Open mission panel in planning mode
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: TimelineDockWidget::OnKeyUp
// Created: ABR 2013-05-24
// -----------------------------------------------------------------------------
void TimelineDockWidget::OnKeyUp( int key )
{
    if( selected_ && key == VK_DELETE )
        DeleteEvent( selected_->uuid );
}
