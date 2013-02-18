// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichDockWidget.h"
#include "clients_kernel/Controllers.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichDockWidget constructor
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
RichDockWidget::RichDockWidget( kernel::Controllers& controllers, QWidget* parent, const QString& objectName, const QString& windowTitle /* = "" */ )
    : QDockWidget( parent )
    , controllers_( controllers )
{
    setObjectName( objectName );
    setWindowTitle( windowTitle );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RichDockWidget destructor
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
RichDockWidget::~RichDockWidget()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RichDockWidget::SetVisible
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
void RichDockWidget::SetVisible( bool visible )
{
    setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: RichDockWidget::ForceEnabled
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
void RichDockWidget::ForceEnabled( bool enabled )
{
    setEnabled( enabled );
    setFeatures( ( enabled ) ? QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable : QDockWidget::NoDockWidgetFeatures );
    QAction* action = toggleViewAction();
    action->setEnabled( false );
    action->setCheckable( enabled );
    action->setChecked( enabled );
}

// -----------------------------------------------------------------------------
// Name: RichDockWidget::EnsureIsEnabled
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
void RichDockWidget::EnsureIsEnabled()
{
    setEnabled( true );
    setFeatures( QDockWidget::AllDockWidgetFeatures );
    QAction* action = toggleViewAction();
    action->setEnabled( true );
    action->setCheckable( true );
    action->setChecked( isVisible() );
}

// -----------------------------------------------------------------------------
// Name: RichDockWidget::IsVisible
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
bool RichDockWidget::IsVisible() const
{
    return isVisible();
}
