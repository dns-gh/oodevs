// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "SessionRunningPage.h"
#include "moc_SessionRunningPage.cpp" 
#include "MessageDialog.h" 
#include "Session.h" 
#include "SessionStatus.h" 
#include "clients_kernel/Controllers.h" 
#include "clients_kernel/Controller.h" 
#include "clients_gui/Tools.h"
#include <qtimer.h> 
#include <qprogressbar.h>

// -----------------------------------------------------------------------------
// Name: SessionRunningPage constructor
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
SessionRunningPage::SessionRunningPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, kernel::Controllers& controllers, boost::shared_ptr< Session > sessionStatus )
    : ContentPage( pages, tools::translate( "SessionRunningPage", "Run Session" ) , previous )
    , config_( config )
    , sessionStatus_( sessionStatus )
    , controllers_( controllers )
{
    controllers_.controller_.Register( *this );
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->layout()->setAlignment( QWidget::AlignCenter );
    box->layout()->setSpacing( 10 );
    status_  = new QLabel( box );
    status_->setAlignment( QWidget::AlignCenter );
    status_->setBackgroundOrigin( QWidget::WindowOrigin );
    timer_ = new QTimer( this );
    connect( timer_, SIGNAL( timeout() ), this, SLOT( UpdateProgress() ) );
    progressBar_ = new QProgressBar( 100, box );
    progressBar_->setCenterIndicator( true );
    AddContent( box );
}

// -----------------------------------------------------------------------------
// Name: SessionRunningPage destructor
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
SessionRunningPage::~SessionRunningPage()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: SessionRunningPage::StartProgressListening
// Created: RDS 2008-09-11
// -----------------------------------------------------------------------------
void SessionRunningPage::StartProgressListening()
{
    progressBar_->setProgress( 0 );
    progressBar_->show();
    timer_->start( 200, false );
}

// -----------------------------------------------------------------------------
// Name: SessionRunningPage::StopProgessListening
// Created: RDS 2008-09-11
// -----------------------------------------------------------------------------
void SessionRunningPage::StopProgressListening()
{
    progressBar_->setProgress( 0 );
    progressBar_->hide();
    timer_->stop();
}

// -----------------------------------------------------------------------------
// Name: SessionRunningPage::UpdateProgress
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
void SessionRunningPage::UpdateProgress()
{
    progressBar_->setProgress( sessionStatus_.get() ? sessionStatus_->GetPercentage() : 0  );
}

// -----------------------------------------------------------------------------
// Name: SessionRunningPage::SetSession
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void SessionRunningPage::SetSession( Session* session )
{
    if( sessionStatus_.get() && sessionStatus_->HasRunningProcess())
    {
        MessageDialog message( this, tools::translate( "SessionRunningPage", "Running Sessions" ), tools::translate( "SessionRunningPage", "Running session detected. Close ?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() != QMessageBox::Yes )
            return;
    }
    sessionStatus_.reset( session );
    if( sessionStatus_.get() )
    {
        sessionStatus_->Start();
        StartProgressListening();
    }
}

// -----------------------------------------------------------------------------
// Name: SessionRunningPage::NotifyUpdated
// Created: RDS 2008-09-29
// -----------------------------------------------------------------------------
void SessionRunningPage::NotifyUpdated( const SessionStatus& status )
{
    switch( status.GetStatus() )
    {
    case SessionStatus::SIM_STARTED:
        status_->setText( tools::translate( "SessionRunningPage", "Starting simulation" ) );
        setCursor( QCursor( Qt::WaitCursor ) );
        break;
    case SessionStatus::SIM_AVAILABLE:
        status_->setText( tools::translate( "SessionRunningPage", "Simulation started" ) );
        break;
    case SessionStatus::GUI_OPENED:
        StopProgressListening();
        status_->setText( tools::translate( "SessionRunningPage", "Running gaming" ) );
        setCursor( QCursor( Qt::ArrowCursor ) );
        Previous();
        qApp->mainWidget()->hide();
        break;
    case SessionStatus::GUI_CLOSED:
        status_->setText( "" );
        if ( sessionStatus_.get() )
            sessionStatus_->StopSimulation();
        qApp->mainWidget()->show();
        qApp->mainWidget()->setActiveWindow();
        break; 
    }
}
