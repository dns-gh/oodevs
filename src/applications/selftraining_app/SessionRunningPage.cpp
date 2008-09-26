// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "MessageDialog.h" 
#include "SessionRunningPage.h"
#include "Session.h" 

#include "moc_SessionRunningPage.cpp" 

#include <qtimer.h> 
#include <qprogressbar.h>

// -----------------------------------------------------------------------------
// Name: SessionRunningPage constructor
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
SessionRunningPage::SessionRunningPage( QWidgetStack* pages, Page_ABC& previous, const tools::GeneralConfig& config, boost::shared_ptr< Session > sessionStatus) 
    : ContentPage    ( pages, tr( "SessionRunningPage" ) , previous )
    , config_        ( config ) 
    , sessionStatus_ ( sessionStatus ) 
{
    QVBox* box = new QVBox( this ) ; 
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->layout()->setAlignment( QWidget::AlignCenter ) ; 
    box->layout()->setSpacing( 10 ) ; 
    status_  = new QLabel(box); 
    status_->setAlignment( QWidget::AlignCenter ); 
    status_->setBackgroundOrigin( QWidget::WindowOrigin );
    timer_ = new QTimer( this ) ; 
    connect( timer_, SIGNAL(timeout()), this, SLOT(UpdateProgress()) );
    progressBar_ = new QProgressBar( 100, box ); 
    progressBar_->setCenterIndicator( true ) ; 
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
    progressBar_->setProgress( 0 ) ;
    progressBar_->show(); 
    timer_->start( 200, FALSE); 
}

// -----------------------------------------------------------------------------
// Name: SessionRunningPage::StopProgessListening
// Created: RDS 2008-09-11
// -----------------------------------------------------------------------------
void SessionRunningPage::StopProgressListening()
{
    progressBar_->setProgress( 0 ) ; 
    progressBar_->hide(); 
    timer_->stop(); 
}

// -----------------------------------------------------------------------------
// Name: SessionRunningPage::UpdateProgress
// Created: RDS 2008-09-10
// -----------------------------------------------------------------------------
void SessionRunningPage::UpdateProgress()
{
    progressBar_->setProgress( sessionStatus_.get() ? sessionStatus_->GetPercentage() : 0  ) ; 
}

// -----------------------------------------------------------------------------
// Name: SessionRunningPage::SetSession
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void SessionRunningPage::SetSession( Session* session )
{
    if (sessionStatus_->HasRunningProcess()) 
    {
        MessageDialog message( this, tr( "Running Sessions" ), tr( "Running session detected. Close ?" ), QMessageBox::Yes, QMessageBox::No );
        if( message.exec() != QMessageBox::Yes )
            return ; 
    }
    sessionStatus_.reset( session ) ; 
    if ( sessionStatus_.get() )
    {
        connect( sessionStatus_.get(), SIGNAL( Feedback( const QString& ) ), this, SLOT( OnSessionPhase( const QString& ) ) );     
        sessionStatus_->Start(); 
        StartProgressListening(); 
    }
}
// -----------------------------------------------------------------------------
// Name: SessionRunningPage::OnSessionPhase
// Created: RDS 2008-09-08
// -----------------------------------------------------------------------------
void SessionRunningPage::OnSessionPhase( const QString& point )
{
    if     ( point == "start_simulation" ) 
    {
        status_->setText( tr("Starting simulation") ); 
        setCursor( QCursor( Qt::WaitCursor ) ); 
    }
    else if ( point == "end_simulation" ) 
    {
        status_->setText( tr("Simulation started") ); 
    }
    else if ( point == "start_gui" ) 
    {
        StopProgressListening(); 
        status_->setText( tr("Running gaming") ); 
        setCursor( QCursor( Qt::ArrowCursor ) ); 
        Previous(); 
        qApp->mainWidget()->hide(); 
    }
    else if ( point == "end_gui" ) 
    {
        status_->setText( "" ); 
        sessionStatus_->StopSimulation(); 
        qApp->mainWidget()->show(); 
        qApp->mainWidget()->setActiveWindow(); 
    }
}


