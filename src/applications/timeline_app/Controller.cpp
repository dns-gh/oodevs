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
    ui_->setupUi( &main_ );
    Configuration next = cfg;
    next.widget = ui_->centralwidget;
    ctx_ = timeline::MakeServer( next );
    QObject::connect( ui_->actionReload, SIGNAL( triggered() ), ctx_.get(), SLOT( Reload() ) );
    QObject::connect( ui_->actionCreate, SIGNAL( triggered() ), this, SLOT( OnCreateEvent() ) );
    QObject::connect( ctx_.get(), SIGNAL( SelectedEvent( boost::shared_ptr< Event > ) ), this, SLOT( OnSelectedEvent( boost::shared_ptr< Event > ) ) );
    main_.show();
}

Controller::~Controller()
{
    // NOTHING
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

void Controller::OnSelectedEvent( boost::shared_ptr< Event > event )
{
    qDebug() << event;
}