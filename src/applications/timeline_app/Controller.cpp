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
#include <boost/bind.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/uuid/random_generator.hpp>
#include <boost/uuid/uuid_io.hpp>

using namespace timeline;

Controller::Controller( const Configuration& cfg )
    : ui_ ( new Ui::Main() )
    , command_( "" )
    , usePopups_( true )
{
    ui_->setupUi( &main_ );
    ui_->toolBar->addWidget( &url_ );
    url_.setText( QString::fromStdString( cfg.url ) );

    loadEventsDialog_.setWindowTitle( "Load events dialog" );
    loadEventsDialog_.setMinimumSize( 400, 500 );
    QVBoxLayout* layout = new QVBoxLayout( &loadEventsDialog_ );
    QPushButton* okButton = new QPushButton();
    okButton->setText( "Load" );
    layout->addWidget( &loadEventsText_ );
    layout->addWidget( okButton );
    QObject::connect( okButton, SIGNAL( clicked() ), this, SLOT( OnLoadEvents() ) );

    Configuration next = cfg;
    next.widget = ui_->centralwidget;
    ctx_ = timeline::MakeServer( next );
    QObject::connect( ui_->actionReload, SIGNAL( triggered() ), this, SLOT( OnReload() ) );
    QObject::connect( ui_->actionCreate, SIGNAL( triggered() ), this, SLOT( OnCreateEvent() ) );
    QObject::connect( ui_->actionDelete, SIGNAL( triggered() ), this, SLOT( OnDeleteEvent() ) );
    QObject::connect( ui_->actionTest, SIGNAL( triggered() ), this, SLOT( OnTestCrud() ) );
    QObject::connect( ui_->actionReadEvents, SIGNAL( triggered() ), this, SLOT( OnReadEvents() ) );
    QObject::connect( ui_->actionLoad, SIGNAL( triggered() ), this, SLOT( OnLoadActionTriggered() ) );
    QObject::connect( ui_->actionSave, SIGNAL( triggered() ), ctx_.get(), SLOT( SaveEvents() ) );
    QObject::connect( ui_->actionCenter, SIGNAL( triggered() ), this, SLOT( OnCenter() ) );
    QObject::connect( ui_->actionSelect, SIGNAL( triggered() ), this, SLOT( OnSelect() ) );
    QObject::connect( ui_->actionSwitchLayout, SIGNAL( triggered() ), this, SLOT( OnSwitchLayout() ) );
    ui_->actionSwitchLayout->setCheckable( true );

    QObject::connect( &url_, SIGNAL( editingFinished() ), this, SLOT( OnLoad() ) );
    QObject::connect( ctx_.get(), SIGNAL( Ready() ), this, SLOT( OnReady() ) );
    QObject::connect( ctx_.get(), SIGNAL( CreatedEvents( const timeline::Events&, const timeline::Error& ) ), this, SLOT( OnCreatedEvents( const timeline::Events&, const timeline::Error& ) ) );
    QObject::connect( ctx_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< timeline::Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< timeline::Event > ) ) );
    QObject::connect( ctx_.get(), SIGNAL( DeletedEvents( const std::vector< std::string >&, const timeline::Error& ) ), this, SLOT( OnDeletedEvents( const std::vector< std::string >&, const timeline::Error& ) ) );
    QObject::connect( ctx_.get(), SIGNAL( LoadedEvents( const timeline::Error& ) ), this, SLOT( OnLoadedEvents( const timeline::Error& ) ) );
    QObject::connect( ctx_.get(), SIGNAL( SavedEvents( const std::string&, const timeline::Error& ) ), this, SLOT( OnSavedEvents( const std::string&, const timeline::Error& ) ) );
    QObject::connect( ctx_.get(), SIGNAL( ActivatedEvent( const timeline::Event& ) ), this, SLOT( OnActivatedEvent( const timeline::Event& ) ) );
    QObject::connect( ctx_.get(), SIGNAL( ContextMenuEvent( boost::shared_ptr< timeline::Event >, const std::string& ) ), this, SLOT( OnContextMenuEvent( boost::shared_ptr< timeline::Event >, const std::string& ) ) );
    QObject::connect( ctx_.get(), SIGNAL( KeyDown( int ) ), this, SLOT( OnKeyDown( int ) ) );
    QObject::connect( ctx_.get(), SIGNAL( KeyPress( int ) ), this, SLOT( OnKeyPress( int ) ) );
    QObject::connect( ctx_.get(), SIGNAL( KeyUp( int ) ), this, SLOT( OnKeyUp( int ) ) );

    ctx_->Start();
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
        return edit.dateTime().toString( "yyyy-MM-ddTHH:mm:ssZ" ).toStdString();
    }

    int ToInteger( const QSpinBox& spinBox )
    {
        return spinBox.value();
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
        dst.uuid       = ToString( *ui.uuid );
        dst.name       = ToString( *ui.name );
        dst.info       = ToString( *ui.info );
        dst.begin      = ToString( *ui.begin );
        dst.end        = ToBool( *ui.endLabel ) ? std::max( ToString( *ui.end ), dst.begin ) : std::string();
        dst.parent     = ToString( *ui.parent );
        dst.error_code = ToInteger( *ui.error_code );
        dst.error_text = ToString( *ui.error_text );
        dst.read_only  = ToBool( *ui.read_only );
        dst.done       = ToBool( *ui.done );
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

void Controller::OnLoad()
{
    ctx_->Load( url_.text().toStdString() );
}

void Controller::OnReady()
{
    ui_->statusBar->showMessage( "Ready!" );
}

void Controller::OnCenter()
{
    ctx_->Center();
}

void Controller::OnSwitchLayout()
{
    auto query = boost::assign::map_list_of( "horizontal", ui_->actionSwitchLayout->isChecked() ? "true" : "false" );
    ctx_->UpdateQuery( query );
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
    ctx_->CreateEvents( boost::assign::list_of( event ) );
}

void Controller::OnCreatedEvents( const Events& events, const Error& error )
{
    for( auto it = events.begin(); it != events.end(); ++it )
        if( error.code == EC_OK )
            ui_->statusBar->showMessage( QString( "event %1 created" ).arg( QString::fromStdString( it->uuid ) ) );
        else
            ui_->statusBar->showMessage( QString( "unable to create event %1 (%2: %3)" )
                .arg( QString::fromStdString( it->uuid ) )
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
        ctx_->DeleteEvents( boost::assign::list_of( uuid_ ) );
}

void Controller::OnDeletedEvents( const std::vector< std::string >& uuids, const Error& error )
{
    for( auto it = uuids.begin(); it != uuids.end(); ++it )
        if( error.code == EC_OK )
            ui_->statusBar->showMessage( QString( "event %1 deleted" ).arg( QString::fromStdString( *it ) ) );
        else
            ui_->statusBar->showMessage( QString( "unable to delete event %1 (%2: %3)" )
                .arg( QString::fromStdString( *it ) )
                .arg( error.code )
                .arg( QString::fromStdString( error.text ) ) );
}

void Controller::OnTestCrud()
{
    const bool popups = usePopups_;
    const std::string uuid = boost::lexical_cast< std::string >( boost::uuids::random_generator()() );
    try
    {
        usePopups_ = false;
        Create( boost::assign::list_of( uuid ) );
        Delete( boost::assign::list_of( uuid ) );
        Read( boost::assign::list_of( uuid ) );
        Update( boost::assign::list_of( uuid ) );
    }
    catch( const std::exception& err )
    {
        ui_->statusBar->showMessage( err.what() );
    }
    usePopups_ = popups;
}

void Controller::OnActivatedEvent( const timeline::Event& event )
{
    ui_->statusBar->showMessage( QString( "event %1 activated" ).arg( QString::fromStdString( event.uuid ) ) );
}

void Controller::OnContextMenuEvent( boost::shared_ptr< timeline::Event > event, const std::string& time )
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
        menu.addAction( QString( "Event time %1" ).arg( QString::fromStdString( time ) ) );
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

void Controller::OnLoadEvents()
{
    ctx_->LoadEvents( loadEventsText_.toPlainText().toStdString() );
    loadEventsDialog_.hide();
}

void Controller::OnLoadActionTriggered()
{
    if( !usePopups_ )
        return;
    loadEventsDialog_.show();
}

void Controller::OnLoadedEvents( const timeline::Error& error )
{
    if( error.code != EC_OK )
        ui_->statusBar->showMessage( QString( "An error occured during while receiving GetEvents: %1" ).arg( error.text.c_str() ) );
    ui_->statusBar->showMessage( "Events successfully loaded" );
}

void Controller::OnSavedEvents( const std::string& events, const timeline::Error& error )
{
    if( error.code != EC_OK )
        ui_->statusBar->showMessage( QString( "An error occured during while receiving GetEvents: %1" ).arg( error.text.c_str() ) );
    ui_->statusBar->showMessage( "Events successfully saved" );

    if( !usePopups_ )
        return;
    QDialog dialog( &main_ );
    dialog.setMinimumSize( 400, 500 );
    QVBoxLayout layout( &dialog );
    QTextEdit textEdit;
    layout.addWidget( &textEdit );
    textEdit.setPlainText( QString::fromStdString( events ) );
    textEdit.setReadOnly( true );
    dialog.exec();
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
        void OnDeletedEvents( const std::vector< std::string >& uuids, const Error& error )
        {
            uuids_  = uuids;
            error_ = error;
            QTimer::singleShot( 0, &loop_, SLOT( quit() ) );
        }
        QEventLoop& loop_;
        std::vector< std::string > uuids_;
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

    bool Equals( const Events& lhs, const Events& rhs )
    {
        if( lhs.size() != rhs.size() )
            return false;
        Events copy = rhs;
        for( auto it = lhs.begin(); it != lhs.end(); ++it )
        {
            auto ju = std::find_if( copy.begin(), copy.end(), boost::bind( static_cast< bool(*)(const Event&,const Event&) >( &Equals ), boost::cref( *it ), _1 ) );
            if( ju == copy.end() )
                return false;
            copy.erase( ju );
        }
        return true;
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
            ",parent:%6"
            ",error_code:%7"
            ",error_text:%8"
            ",read_only:%9"
            ",done:%10"
            ",action:%11"
            "}" )
            .arg( QString::fromStdString( event.uuid ) )
            .arg( QString::fromStdString( event.name ) )
            .arg( QString::fromStdString( event.info ) )
            .arg( QString::fromStdString( event.begin ) )
            .arg( QString::fromStdString( event.end ) )
            .arg( QString::fromStdString( event.parent ) )
            .arg( event.error_code )
            .arg( QString::fromStdString( event.error_text ) )
            .arg( event.read_only )
            .arg( event.done )
            .arg( Dump( event.action ) );
    }

    QString Dump( const Events& events )
    {
        QStringList list;
        for( auto it = events.begin(); it != events.end(); ++it )
            list << Dump( *it );
        return "[" + list.join(",") + "]";
    }

    QString Dump( const Error& error )
    {
        return QString( "Error{"
            "code:%1,text:%2}" )
            .arg( error.code )
            .arg( QString::fromStdString( error.text ) );
    }

    void Check( const Error& value )
    {
        if( !Equals( value, Error() ) )
            throw std::runtime_error( QString( "invalid error %1" ).
                arg( Dump( value ) ).toStdString() );
    }

    void Check( const Event& lhs, const Event& rhs )
    {
        if( !Equals( lhs, rhs ) )
            throw std::runtime_error( QString( "invalid event %1:%2" ).
                arg( Dump( lhs ) ).
                arg( Dump( rhs ) ).
                toStdString() );
    }
}

int Controller::Delete( const std::vector< std::string >& args )
{
    if( args.size() != 1 )
        throw std::runtime_error( "usage: delete <uuid>" );
    QEventLoop wait;
    DeleteEvent deleter( wait );
    QObject::connect( ctx_.get(), SIGNAL( DeletedEvents( const std::vector< std::string >&, const timeline::Error& ) ), &deleter, SLOT( OnDeletedEvents( const std::vector< std::string >&, const timeline::Error& ) ) );
    ctx_->DeleteEvents( boost::assign::list_of( args[0] ) );
    wait.exec();
    Check( deleter.error_ );
    if( deleter.uuids_.size() != 1 )
        throw std::runtime_error( "invalid number of uuids" );
    if( deleter.uuids_.front() != args[0] )
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
        void OnCreatedEvents( const Events& events, const Error& error )
        {
            events_  = events;
            error_ = error;
            QTimer::singleShot( 0, &loop_, SLOT( quit() ) );
        }
        static std::pair< Events, Error > Create( Server_ABC& ctx, const Event& event )
        {
            QEventLoop wait;
            CreateEvent creator( wait );
            QObject::connect( &ctx, SIGNAL( CreatedEvents( const timeline::Events&, const timeline::Error& ) ), &creator, SLOT( OnCreatedEvents( const timeline::Events&, const timeline::Error& ) ) );
            ctx.CreateEvents( boost::assign::list_of( event ) );
            wait.exec();
            return std::make_pair( creator.events_, creator.error_ );
        }
        QEventLoop& loop_;
        Events events_;
        Error error_;
    };
}

int Controller::Create( const std::vector< std::string >& args )
{
    if( args.size() != 1 )
        throw std::runtime_error( "usage: create <uuid>" );
    Action action( "zomgjohndoe", true, "hello world" );
    Event event( args[0], "some_name", "some_info", "2013-01-01T11:00:04Z", std::string(), false, action );
    const auto pair = CreateEvent::Create( *ctx_, event );
    Check( pair.second );
    if( pair.first.size() != 1 )
        throw std::runtime_error( "invalid number of events" );
    // if input uuid is empty, use the server generated uuid
    if( event.uuid.empty() )
        event.uuid = pair.first.front().uuid;
    Check( pair.first.front(), event );
    return 0;
}

namespace
{
    struct CheckReadEvent : public OnSignal_ABC
    {
        CheckReadEvent( QEventLoop& loop )
            : OnSignal_ABC()
            , loop_       ( loop )
        {
            // NOTHING
        }
        void OnReadEvents( const Events& events, const Error& error )
        {
            events_  = events;
            error_ = error;
            QTimer::singleShot( 0, &loop_, SLOT( quit() ) );
        }
        void OnReadEvent( const Event& event, const Error& error )
        {
            events_.push_back( event );
            error_ = error;
            QTimer::singleShot( 0, &loop_, SLOT( quit() ) );
        }
        QEventLoop& loop_;
        Events events_;
        Error error_;
    };

    std::pair< Events, Error > ReadEvents( Server_ABC& ctx, const std::string& uuid = std::string() )
    {
        QEventLoop wait;
        CheckReadEvent read( wait );
        QObject::connect( &ctx, SIGNAL( GetEvents( const timeline::Events&, const timeline::Error& ) ), &read, SLOT( OnReadEvents( const timeline::Events&, const timeline::Error& ) ) );
        QObject::connect( &ctx, SIGNAL( GetEvent ( const timeline::Event&,  const timeline::Error& ) ), &read, SLOT( OnReadEvent ( const timeline::Event&,  const timeline::Error& ) ) );
        if( uuid.empty() )
            ctx.ReadEvents();
        else
            ctx.ReadEvent( uuid );
        wait.exec();
        return std::make_pair( read.events_, read.error_ );
    }

    void ResetEvents( Controller& control, Server_ABC& ctx, const Events& events )
    {
        const auto pair = ReadEvents( ctx );
        for( auto it = pair.first.begin(); it != pair.first.end(); ++it )
            control.Delete( boost::assign::list_of( it->uuid ) );
        for( auto it = events.begin(); it != events.end(); ++it )
            CreateEvent::Create( ctx, *it );
    }
}

int Controller::Read( const std::vector< std::string >& args )
{
    if( args.size() != 1 )
        throw std::runtime_error( "usage: read <uuid>" );
    const Events ref = boost::assign::list_of
        ( Event( args[0], "some_name", "some_info", "2013-01-01T11:00:04Z", std::string(), false, Action() ) )
        ( Event( boost::lexical_cast< std::string >( boost::uuids::random_generator()() ), "some_name2", "some_info2", "2017-02-03T17:24:13Z", std::string(), false, Action() ) );
    ResetEvents( *this, *ctx_, ref );
    auto pair = ReadEvents( *ctx_ );
    Check( pair.second );
    if( !Equals( pair.first, ref ) )
        throw std::runtime_error( QString( "invalid events %1:%2" ).
            arg( Dump( pair.first ) ).
            arg( Dump( ref ) ).toStdString() );
    pair = ReadEvents( *ctx_, args[0] );
    Check( pair.second );
    if( pair.first.empty() )
        throw std::runtime_error( "missing event" );
    Check( *pair.first.begin(), ref[0] );
    return 0;
}

namespace
{
    struct UpdateEvent : public OnSignal_ABC
    {
        UpdateEvent( QEventLoop& loop )
            : OnSignal_ABC()
            , loop_       ( loop )
        {
            // NOTHING
        }
        void OnUpdatedEvent( const Event& event, const Error& error )
        {
            event_  = event;
            error_ = error;
            QTimer::singleShot( 0, &loop_, SLOT( quit() ) );
        }
        static std::pair< Event, Error > Update( Server_ABC& ctx, const Event& event )
        {
            QEventLoop wait;
            UpdateEvent update( wait );
            QObject::connect( &ctx, SIGNAL( UpdatedEvent( const timeline::Event&, const timeline::Error& ) ), &update, SLOT( OnUpdatedEvent( const timeline::Event&, const timeline::Error& ) ) );
            ctx.UpdateEvent( event );
            wait.exec();
            return std::make_pair( update.event_, update.error_ );
        }
        QEventLoop& loop_;
        Event event_;
        Error error_;
    };
}

int Controller::Update( const std::vector< std::string >& args )
{
    if( args.size() != 1 )
        throw std::runtime_error( "usage: read <uuid>" );
    const Event event( args[0], "some_name", "some_info",
        "2013-01-01T11:00:04Z", std::string(), false, Action() );
    ResetEvents( *this, *ctx_, boost::assign::list_of( event ) );
    Event next = event;
    next.name = "updated_name";
    next.info = "updated_info";
    next.begin = "2013-01-01T12:00:04Z";
    next.end = "2013-01-01T13:00:04Z";
    next.done = false;
    next.action.target = "protocol://host:port";
    next.action.apply = true;
    next.action.payload = "updated_payload";
    const auto pair = UpdateEvent::Update( *ctx_, next );
    Check( pair.second );
    Check( pair.first, next );
    return 0;
}

namespace
{
    struct CheckSaveLoadEvents : public OnSignal_ABC
    {
        CheckSaveLoadEvents( QEventLoop& loop )
            : OnSignal_ABC()
            , loop_( loop )
        {
            // NOTHING
        }

        void OnLoadedEvents( const timeline::Error& error )
        {
            error_ = error;
            QTimer::singleShot( 0, &loop_, SLOT( quit() ) );
        }
        void OnSavedEvents( const std::string& events, const timeline::Error& error )
        {
            events_  = events;
            error_ = error;
            QTimer::singleShot( 0, &loop_, SLOT( quit() ) );
        }
        QEventLoop& loop_;
        std::string events_;
        Error error_;
    };

    size_t GetSize( Server_ABC& ctx )
    {
        const auto pair = ReadEvents( ctx );
        return pair.first.size();
    }
}

int Controller::SaveLoad( const std::vector< std::string >& args )
{
    if( args.size() != 0 )
        throw std::runtime_error( "usage: saveload" );
    std::string savedEvents = "";
    {
        QEventLoop wait;
        CheckSaveLoadEvents saver( wait );
        QObject::connect( ctx_.get(), SIGNAL( SavedEvents( const std::string&, const timeline::Error& ) ), &saver, SLOT( OnSavedEvents( const std::string&, const timeline::Error& ) ) );
        ctx_->SaveEvents();
        wait.exec();
        Check( saver.error_ );
        savedEvents = saver.events_;
    }
    if( savedEvents.empty() )
        throw std::runtime_error( "Save events failed" );
    ResetEvents( *this, *ctx_, Events() );
    if( GetSize( *ctx_ ) > 0 )
        throw std::runtime_error( "Reset events failed" );
    {
        QEventLoop wait;
        CheckSaveLoadEvents loader( wait );
        QObject::connect( ctx_.get(), SIGNAL( LoadedEvents( const timeline::Error& ) ), &loader, SLOT( OnLoadedEvents( const timeline::Error& ) ) );
        ctx_->LoadEvents( savedEvents );
        wait.exec();
        Check( loader.error_ );
    }
    return 0;
}

int Controller::Execute( const std::string& command, const std::vector< std::string >& args )
{
    usePopups_ = false;
    command_ = command;
    WaitReady();
    if( command == "ready" )
        return 0;
    if( command == "delete" )
        return Delete( args );
    if( command == "create" )
        return Create( args );
    if( command == "read" )
        return Read( args );
    if( command == "update" )
        return Update( args );
    if( command == "saveload" )
        return SaveLoad( args );
    throw std::runtime_error( "unexpected command " + command );
}

void Controller::OnSelect()
{
    if( !usePopups_ )
        return;
    QDialog dialog( &main_ );
    dialog.setMinimumSize( 300, 100 );
    QHBoxLayout layout( &dialog );
    QComboBox comboBox;
    QPushButton button( "ok" );
    auto pair = ReadEvents( *ctx_ );
    for( auto it = pair.first.begin(); it != pair.first.end(); ++it )
        comboBox.addItem( it->uuid.c_str() );
    layout.addWidget( new QLabel( "uuid:" ) );
    layout.addWidget( &comboBox );
    layout.addWidget( &button );
    QObject::connect( &button, SIGNAL( clicked() ), &dialog, SLOT( accept() ) );
    const int ret = dialog.exec();
    QString uuid = comboBox.currentText();
    if( ret != QDialog::Accepted || uuid.isEmpty() )
        return;
    ctx_->SelectEvent( uuid.toStdString() );
}

void Controller::OnReadEvents()
{
    if( !usePopups_ )
        return;
    auto pair = ReadEvents( *ctx_ );
    QDialog dialog( &main_ );
    dialog.setMinimumSize( 400, 500 );
    QVBoxLayout layout( &dialog );
    QListWidget list;
    layout.addWidget( &list );
    for( auto it = pair.first.begin(); it != pair.first.end(); ++it )
        list.addItem( QString( "Event %1: %2: %3" ).arg( list.count() ).arg( it->name.c_str() ).arg( it->uuid.c_str() ) );
    dialog.exec();
}
