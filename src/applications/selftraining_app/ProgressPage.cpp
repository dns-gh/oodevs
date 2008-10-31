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
#include "frontend/Process_ABC.h"
#include "clients_kernel/Controllers.h"
#include <qprogressbar.h>
#include <qtimer.h>

// -----------------------------------------------------------------------------
// Name: ProgressPage constructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ProgressPage::ProgressPage( QWidgetStack* pages, Page_ABC& previous, const QString& title, kernel::Controllers& controllers )
    : ContentPage( pages, title, previous )
    , controllers_( controllers )
{
    QVBox* box = new QVBox( this );
    box->setBackgroundOrigin( QWidget::WindowOrigin );
    box->layout()->setAlignment( QWidget::AlignCenter );
    box->layout()->setSpacing( 10 );
    label_ = new QLabel( box );
    label_->setAlignment( QWidget::AlignCenter );
    label_->setBackgroundOrigin( QWidget::WindowOrigin );
    progressBar_ = new QProgressBar( 100, box );
    progressBar_->setCenterIndicator( true );
    AddContent( box );
    timer_ = new QTimer( this );
    connect( timer_, SIGNAL( timeout() ), this, SLOT( UpdateProgress() ) );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: ProgressPage destructor
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
ProgressPage::~ProgressPage()
{
    timer_->stop();
    process_.reset();
    controllers_.Unregister( *this );
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
    const unsigned int percentage = process_.get() ? process_->GetPercentage() : 0;
    progressBar_->setProgress( percentage );
    label_->setText( process_.get() ? process_->GetStatus() : "" );
    setCursor( percentage < 100 ? Qt::WaitCursor : Qt::ArrowCursor );
    if( percentage == 100 )
    {
        timer_->stop();
        qApp->mainWidget()->hide();
    }
}

// -----------------------------------------------------------------------------
// Name: ProgressPage::NotifyUpdated
// Created: SBO 2008-10-14
// -----------------------------------------------------------------------------
void ProgressPage::NotifyUpdated( const boost::shared_ptr< frontend::Process_ABC >& process )
{
    if( process.get() == process_.get() )
        UpdateProgress();
}

// -----------------------------------------------------------------------------
// Name: ProgressPage::NotifyDeleted
// Created: SBO 2008-10-15
// -----------------------------------------------------------------------------
void ProgressPage::NotifyDeleted( const boost::shared_ptr< frontend::Process_ABC >& process )
{
    if( process.get() == process_.get() )
    {
        timer_->stop();
        process_.reset();
        qApp->mainWidget()->show();
        qApp->mainWidget()->setActiveWindow();
        Previous();
    }
}
