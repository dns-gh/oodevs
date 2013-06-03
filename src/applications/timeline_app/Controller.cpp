// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "Controller.h"

#include "ui_main.h"
#include "ui_create_event.h"
#include "moc_Controller.cpp"

#include <boost/assign/list_of.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace timeline;

Controller::Controller( const Configuration& cfg )
    : ui_ ( new Ui::Main() )
{
    ui_->setupUi( &main_ );
    Configuration next = cfg;
    next.widget = ui_->centralwidget;
    ctx_ = timeline::MakeServer( next );
    QObject::connect( ui_->actionReload, SIGNAL( triggered() ), this, SLOT( OnReload() ) );
    QObject::connect( ui_->actionCreate, SIGNAL( triggered() ), this, SLOT( OnCreateEvent() ) );
    QObject::connect( ui_->actionDelete, SIGNAL( triggered() ), this, SLOT( OnDeleteEvent() ) );
    QObject::connect( ui_->actionTestCreate, SIGNAL( triggered() ), this, SLOT( OnTestCreate() ) );
    QObject::connect( ctx_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), this, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
    QObject::connect( ctx_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< timeline::Event > ) ) );
    QObject::connect( ctx_.get(), SIGNAL( DeletedEvent( const std::string&, const timeline::Error& ) ), this, SLOT( OnDeletedEvent( const std::string&, const timeline::Error& ) ) );
    QObject::connect( ctx_.get(), SIGNAL( ActivatedEvent( const timeline::Event& ) ), this, SLOT( OnActivatedEvent( const timeline::Event& ) ) );
    QObject::connect( ctx_.get(), SIGNAL( ContextMenuEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnContextMenuEvent( boost::shared_ptr< timeline::Event > ) ) );
    QObject::connect( ctx_.get(), SIGNAL( KeyDown( int ) ), this, SLOT( OnKeyDown( int ) ) );
    QObject::connect( ctx_.get(), SIGNAL( KeyPress( int ) ), this, SLOT( OnKeyPress( int ) ) );
    QObject::connect( ctx_.get(), SIGNAL( KeyUp( int ) ), this, SLOT( OnKeyUp( int ) ) );
}

Controller::~Controller()
{
    // NOTHING
}

void Controller::Show()
{
    main_.show();
}

namespace
{
    std::string ToString( const QLineEdit& edit )
    {
        return edit.text().toStdString();
    }

    bool ToBool( const QCheckBox& edit )
    {
        return edit.checkState() == Qt::Checked;
    }

    std::string ToString( const QDateTimeEdit& edit )
    {
        return edit.dateTime().toTimeSpec( Qt::UTC ).toString( "yyyy-MM-ddTHH:mm:ssZ" ).toStdString();
    }

    void WriteDummyEvent( Ui::CreateEvent& ui )
    {
        ui.name->setText( "some name" );
        ui.info->setText( "some info" );
        ui.begin->setDateTime( QDateTime::currentDateTime() );
        ui.done->setCheckState( Qt::Unchecked );
    }

    void ReadEvent( Event& dst, const Ui::CreateEvent& ui )
    {
        dst.uuid  = ToString( *ui.uuid );
        dst.name  = ToString( *ui.name );
        dst.info  = ToString( *ui.info );
        dst.begin = ToString( *ui.begin );
        dst.end   = ToBool( *ui.endLabel ) ? std::max( ToString( *ui.end ), dst.begin ) : std::string();
        dst.done  = ToBool( *ui.done );
        const std::string target = ToString( *ui.target );
        if( target.empty() )
            return;
        dst.action.target  = target;
        dst.action.apply   = ToBool( *ui.apply );
        dst.action.payload = ToString( *ui.payload );
    }
}

void Controller::OnReload()
{
    uuid_.clear();
    ui_->actionDelete->setEnabled( false );
    ctx_->Reload();
}

void Controller::OnCreateEvent()
{
    Ui::CreateEvent ui;
    QDialog dialog;
    ui.setupUi( &dialog );
    WriteDummyEvent( ui );
    QObject::connect( ui.buttonBox, SIGNAL( accepted() ), &dialog, SLOT( accept() ) );
    QObject::connect( ui.buttonBox, SIGNAL( rejected() ), &dialog, SLOT( reject() ) );
    const int ret = dialog.exec();
    if( ret != QDialog::Accepted )
        return;
    Event event;
    ReadEvent( event, ui );
    ctx_->CreateEvent( event );
}

void Controller::OnCreatedEvent( const Event& event, const Error& error )
{
    if( error.code == EC_OK )
        ui_->statusBar->showMessage( QString( "event %1 created" ).arg( QString::fromStdString( event.uuid ) ) );
    else
        ui_->statusBar->showMessage( QString( "unable to create event %1 (%2: %3)" )
            .arg( QString::fromStdString( event.uuid ) )
            .arg( error.code )
            .arg( QString::fromStdString( error.text ) ) );
}

void Controller::OnSelectedEvent( boost::shared_ptr< Event > event )
{
    if( event )
    {
        uuid_ = event->uuid;
        ui_->statusBar->showMessage( QString( "event %1 selected" ).arg( QString::fromStdString( uuid_ ) ) );
    }
    else
        ui_->statusBar->showMessage( "event deselected" );
    ui_->actionDelete->setEnabled( event );
}

void Controller::OnDeleteEvent()
{
    if( uuid_.empty() )
        ui_->statusBar->showMessage( "unable to delete unknown event" );
    else
        ctx_->DeleteEvent( uuid_ );
}

void Controller::OnDeletedEvent( const std::string& uuid, const Error& error )
{
    if( error.code == EC_OK )
        ui_->statusBar->showMessage( QString( "event %1 deleted" ).arg( QString::fromStdString( uuid ) ) );
    else
        ui_->statusBar->showMessage( QString( "unable to delete event %1 (%2: %3)" )
            .arg( QString::fromStdString( uuid ) )
            .arg( error.code )
            .arg( QString::fromStdString( error.text ) ) );
}

void Controller::OnTestCreate()
{
    const std::string uuid = boost::lexical_cast< std::string >( boost::uuids::random_generator()() );
    try
    {
        Create( boost::assign::list_of( uuid ) );
    }
    catch( const std::exception& err )
    {
        ui_->statusBar->showMessage( err.what() );
    }
}

void Controller::OnActivatedEvent( const timeline::Event& event )
{
    ui_->statusBar->showMessage( QString( "event %1 activated" ).arg( QString::fromStdString( event.uuid ) ) );
}

void Controller::OnContextMenuEvent( boost::shared_ptr< timeline::Event > event )
{
    if( event )
    {
        ui_->statusBar->showMessage( QString( "context menu is requested on event %1" ).arg( QString::fromStdString( event->uuid ) ) );
        QMenu menu;
        menu.addAction( "Event context menu" );
        menu.addSeparator();
        menu.addAction( QString( "Event id: %1" ).arg( QString::fromStdString( event->uuid ) ) );
        menu.exec( QCursor::pos() );
    }
    else
    {
        ui_->statusBar->showMessage( "context menu is requested on the background" );
        QMenu menu;
        menu.addAction( "Background context menu" );
        menu.exec( QCursor::pos() );
    }
}

void Controller::OnKeyDown( int key )
{
    ui_->statusBar->showMessage( QString( "key 0x%1 down" ).arg( key, 0, 16 ) );
}

void Controller::OnKeyPress( int key )
{
    ui_->statusBar->showMessage( QString( "key 0x%1 press" ).arg( key, 0, 16 ) );
}

void Controller::OnKeyUp( int key )
{
    ui_->statusBar->showMessage( QString( "key 0x%1 up" ).arg( key, 0, 16 ) );
}

void Controller::WaitReady() const
{
    QEventLoop wait;
    QObject::connect( ctx_.get(), SIGNAL( Ready() ), &wait, SLOT( quit() ) );
    wait.exec();
}

namespace
{
    struct DeleteEvent : public OnSignal_ABC
    {
        DeleteEvent( QEventLoop& loop )
            : OnSignal_ABC()
            , loop_       ( loop )
        {
            // NOTHING
        }
        void OnDeletedEvent( const std::string& uuid, const Error& error )
        {
            uuid_  = uuid;
            error_ = error;
            QTimer::singleShot( 0, &loop_, SLOT( quit() ) );
        }
        QEventLoop& loop_;
        std::string uuid_;
        Error error_;
    };

    bool Equals( const Action& lhs, const Action& rhs )
    {
        return lhs.target  == rhs.target
            && lhs.apply   == rhs.apply
            && lhs.payload == rhs.payload;
    }

    bool Equals( const Event& lhs, const Event& rhs )
    {
        return lhs.uuid    == rhs.uuid
            && lhs.name    == rhs.name
            && lhs.info    == rhs.info
            && lhs.begin   == rhs.begin
            && lhs.end     == rhs.end
            && lhs.done    == rhs.done
            && Equals( lhs.action, rhs.action );
    }

    bool Equals( const Error& lhs, const Error& rhs )
    {
        return lhs.code == rhs.code
            && lhs.text == rhs.text;
    }

    QString Dump( const Action& action )
    {
        return QString( "Action{"
            "target:%1"
            ",apply:%2"
            ",payload:%3"
            "}" )
            .arg( QString::fromStdString( action.target ) )
            .arg( action.apply )
            .arg( QString::fromStdString( action.payload ) );
    }

    QString Dump( const Event& event )
    {
        return QString( "Event{"
            "uuid:%1"
            ",name:%2"
            ",info:%3"
            ",begin:%4"
            ",end:%5"
            ",done:%6"
            ",action:%7"
            "}" )
            .arg( QString::fromStdString( event.uuid ) )
            .arg( QString::fromStdString( event.name ) )
            .arg( QString::fromStdString( event.info ) )
            .arg( QString::fromStdString( event.begin ) )
            .arg( QString::fromStdString( event.end ) )
            .arg( event.done )
            .arg( Dump( event.action ) );
    }

    QString Dump( const Error& error )
    {
        return QString( "Error{"
            "code:%1,text:%2}" )
            .arg( error.code )
            .arg( QString::fromStdString( error.text ) );
    }
}

int Controller::Delete( const std::vector< std::string >& args )
{
    if( args.size() != 1 )
        throw std::runtime_error( "usage: delete <uuid>" );
    QEventLoop wait;
    DeleteEvent deleter( wait );
    QObject::connect( ctx_.get(), SIGNAL( DeletedEvent( const std::string&, const timeline::Error& ) ), &deleter, SLOT( OnDeletedEvent( const std::string&, const timeline::Error& ) ) );
    ctx_->DeleteEvent( args[0] );
    wait.exec();
    if( !Equals( deleter.error_, Error() ) )
        throw std::runtime_error( QString( "invalid error %1" ).
            arg( Dump( deleter.error_ ) ).toStdString() );
    if( deleter.uuid_ != args[0] )
        throw std::runtime_error( "invalid uuid" );
    return 0;
}

namespace
{
    struct CreateEvent : public OnSignal_ABC
    {
        CreateEvent( QEventLoop& loop )
            : OnSignal_ABC()
            , loop_       ( loop )
        {
            // NOTHING
        }
        void OnCreatedEvent( const Event& event, const Error& error )
        {
            event_  = event;
            error_ = error;
            QTimer::singleShot( 0, &loop_, SLOT( quit() ) );
        }
        QEventLoop& loop_;
        Event event_;
        Error error_;
    };
}

int Controller::Create( const std::vector< std::string >& args )
{
    if( args.size() != 1 )
        throw std::runtime_error( "usage: create <uuid>" );
    QEventLoop wait;
    CreateEvent creator( wait );
    QObject::connect( ctx_.get(), SIGNAL( CreatedEvent( const timeline::Event&, const timeline::Error& ) ), &creator, SLOT( OnCreatedEvent( const timeline::Event&, const timeline::Error& ) ) );
    Action action( "zomgjohndoe", true, "c29tZV91bmtub3duX3BheWxvYWQ=" );
    Event event( args[0], "some_name", "some_info", "2013-01-01T11:00:04Z", std::string(), false, action );
    ctx_->CreateEvent( event );
    wait.exec();
    if( !Equals( creator.error_, Error() ) )
        throw std::runtime_error( QString( "invalid error %1" ).
            arg( Dump( creator.error_ ) ).toStdString() );
    // if input uuid is empty, use the server generated uuid
    if( event.uuid.empty() )
        event.uuid = creator.event_.uuid;
    if( !Equals( creator.event_, event ) )
        throw std::runtime_error( QString( "invalid event %1:%2" ).
            arg( Dump( creator.event_ ) ).
            arg( Dump( event ) ).
            toStdString() );
    return 0;
}

int Controller::Execute( const std::string& command, const std::vector< std::string >& args )
{
    WaitReady();
    if( command == "ready" )
        return 0;
    if( command == "delete" )
        return Delete( args );
    if( command == "create" )
        return Create( args );
    throw std::runtime_error( "unexpected command " + command );
}