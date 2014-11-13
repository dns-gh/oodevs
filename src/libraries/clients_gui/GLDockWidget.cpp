// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "GLDockWidget.h"
#include "moc_GLDockWidget.cpp"
#include "GLStackedWidget.h"
#include "GLView_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: GLDockWidget constructor
// Created: ABR 2014-06-12
// -----------------------------------------------------------------------------
GLDockWidget::GLDockWidget( kernel::Controllers& controllers,
                            QMainWindow& parent,
                            const std::shared_ptr< GLStackedWidget >& stackedWidget )
    : RichDockWidget( controllers, &parent, QString( "GLDockWidget_%1" ).arg( stackedWidget->GetProxy()->GetID() ) )
    , parent_( parent )
    , stackedWidget_( stackedWidget )
    , toggleViewAction_( new QAction( this ) )
{
    setWindowTitle( stackedWidget_->GetProxy()->GetName() );
    setWidget( stackedWidget_.get() );
    setFloating( true );
    setMinimumSize( 200, 200 );
    auto center = parent.geometry().center();
    setGeometry( center.x() - 200, center.y() - 200, 400, 400 );
    setProperty( "GLDockWidget", QVariant( true ) );
    parent_.addDockWidget( Qt::LeftDockWidgetArea, this );
    
    NotifyModeChanged( controllers_.GetCurrentMode(), false, true );

    toggleViewAction_->setCheckable( true );
    toggleViewAction_->setChecked( true );
    toggleViewAction_->setText( windowTitle() );
    connect( toggleViewAction_.get(), SIGNAL( triggered( bool ) ), SLOT( setVisible( bool ) ) );
}

// -----------------------------------------------------------------------------
// Name: GLDockWidget destructor
// Created: ABR 2014-06-12
// -----------------------------------------------------------------------------
GLDockWidget::~GLDockWidget()
{
    parent_.removeDockWidget( this );
}

// -----------------------------------------------------------------------------
// Name: GLDockWidget::GetStackedWidget
// Created: ABR 2014-06-12
// -----------------------------------------------------------------------------
std::shared_ptr< GLStackedWidget > GLDockWidget::GetStackedWidget() const
{
    return stackedWidget_;
}

// -----------------------------------------------------------------------------
// Name: GLDockWidget::closeEvent
// Created: ABR 2014-06-12
// -----------------------------------------------------------------------------
void GLDockWidget::closeEvent( QCloseEvent *event )
{
    QDockWidget::closeEvent( event );
    emit OnClose( *this );
}

// -----------------------------------------------------------------------------
// Name: GLDockWidget::ToggleViewAction
// Created: ABR 2014-06-19
// -----------------------------------------------------------------------------
QAction* GLDockWidget::ToggleViewAction() const
{
    return toggleViewAction_.get();
}
