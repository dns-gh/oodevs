// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "ProgressPage.h"
#include "moc_ProgressPage.cpp"
#include "Application.h"
#include "frontend/Process_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Tools.h"

// -----------------------------------------------------------------------------
// Name: ProgressPage constructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ProgressPage::ProgressPage( Application& app, QStackedWidget* pages, Page_ABC& previous )
    : ContentPage( pages, previous, 0 )
    , app_       ( app )
{
    Q3VBox* box = new Q3VBox( this );
    box->layout()->setAlignment( Qt::AlignCenter );
    box->layout()->setSpacing( 10 );
    label_ = new QLabel( box );
    label_->setAlignment( Qt::AlignCenter );
    progressBar_ = new Q3ProgressBar( 100, box );
    progressBar_->setCenterIndicator( true );
    AddContent( box );
    timer_ = new QTimer( this );
    connect( timer_, SIGNAL( timeout() ), this, SLOT( UpdateProgress() ) );
}

// -----------------------------------------------------------------------------
// Name: ProgressPage destructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ProgressPage::~ProgressPage()
{
    timer_->stop();
    process_.reset();
}

// -----------------------------------------------------------------------------
// Name: ProgressPage::Attach
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void ProgressPage::Attach( boost::shared_ptr< frontend::Process_ABC > process )
{
    process_ = process;
    if( !timer_->isActive() )
        timer_->start( 200, false );
}

// -----------------------------------------------------------------------------
// Name: ProgressPage::UpdateProgress
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void ProgressPage::UpdateProgress()
{
    unsigned int percentage = 0;
    std::string message = "";
    if( ! process_.expired() )
    {
        boost::shared_ptr< frontend::Process_ABC > weak( process_ );
        if( weak.get() )
        {
            percentage = weak->GetPercentage();
            message = weak->GetStatus();
        }
    }
    label_->setText( message.c_str() );
    progressBar_->setProgress( percentage );
    setCursor( percentage < 100 ? Qt::WaitCursor : Qt::ArrowCursor );
    if( percentage == 100 )
    {
        timer_->stop();
        app_.GetMainWindow()->hide();
    }
}

// -----------------------------------------------------------------------------
// Name: ProgressPage::NotifyStopped
// Created: SBO 2010-11-10
// -----------------------------------------------------------------------------
void ProgressPage::NotifyStopped()
{
    QApplication::postEvent( qApp, new QCustomEvent( static_cast< QEvent::Type >( QEvent::User + 667 ), static_cast< void* >( this ) ) );
}

// -----------------------------------------------------------------------------
// Name: ProgressPage::DoNotifyStopped
// Created: JSR 2012-07-26
// -----------------------------------------------------------------------------
void ProgressPage::DoNotifyStopped()
{
    timer_->stop();
    process_.reset();
    app_.GetMainWindow()->show();
    app_.GetMainWindow()->activateWindow();
    Previous();
}

// -----------------------------------------------------------------------------
// Name: ProgressPage::NotifyError
// Created: SBO 2010-12-09
// -----------------------------------------------------------------------------
void ProgressPage::NotifyError( const std::string& error, std::string /*commanderEndpoint = "" */ )
{
    NotifyStopped();
    QString* message = new QString( error.c_str() );
    QApplication::postEvent( qApp, new QCustomEvent( static_cast< QEvent::Type >( QEvent::User + 666 ), static_cast< void* >( message ) ) );
}
