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

using namespace timeline;

Controller::Controller( const Configuration& cfg )
    : ui_ ( new Ui::Main() )
{
    qRegisterMetaType< boost::shared_ptr< Event > >( "boost::shared_ptr< Event >" );
    qRegisterMetaType< std::string >( "std::string" );
    qRegisterMetaType< Event >( "Event" );
    qRegisterMetaType< Error >( "Error" );
    ui_->setupUi( &main_ );
    Configuration next = cfg;
    next.widget = ui_->centralwidget;
    ctx_ = timeline::MakeServer( next );
    QObject::connect( ui_->actionReload, SIGNAL( triggered() ), this, SLOT( OnReload() ) );
    QObject::connect( ui_->actionCreate, SIGNAL( triggered() ), this, SLOT( OnCreateEvent() ) );
    QObject::connect( ui_->actionDelete, SIGNAL( triggered() ), this, SLOT( OnDeleteEvent() ) );
    QObject::connect( ctx_.get(), SIGNAL( CreatedEvent( const Event&, const Error& ) ), this, SLOT( OnCreatedEvent( const Event&, const Error& ) ) );
    QObject::connect( ctx_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< Event > ) ) );
    QObject::connect( ctx_.get(), SIGNAL( DeletedEvent( const std::string&, const Error& ) ), this, SLOT( OnDeletedEvent( const std::string&, const Error& ) ) );
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
        uuid_ = event->uuid;
    if( event )
        ui_->statusBar->showMessage( QString( "event %1 selected" ).arg( QString::fromStdString( uuid_ ) ) );
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

void Controller::WaitReady() const
{
    QEventLoop wait;
    QObject::connect( ctx_.get(), SIGNAL( Ready() ), &wait, SLOT( quit() ) );
    wait.exec();
}

int Controller::Execute( const std::string& command )
{
    WaitReady();
    if( command == "ready" )
        return 0;
    throw std::runtime_error( "unexpected command " + command );
}