// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichToolBar.h"
#include "clients_kernel/Controllers.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichToolBar constructor
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
RichToolBar::RichToolBar( kernel::Controllers& controllers, QWidget* parent, const QString& objectName, const QString& label /* = "" */ )
    : QToolBar( parent )
    , controllers_( controllers )
    , windowMenuVisibility_( true )
{
    setObjectName( objectName );
    setLabel( label );
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RichToolBar destructor
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
RichToolBar::~RichToolBar()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RichToolBar::SetVisible
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
void RichToolBar::SetVisible( bool visible )
{
    setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: RichToolBar::ForceEnabled
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
void RichToolBar::ForceEnabled( bool enabled )
{
    setEnabled( enabled );
    QAction* action = toggleViewAction();
    action->setEnabled( false );
    action->setCheckable( enabled );
    action->setChecked( enabled );
}

// -----------------------------------------------------------------------------
// Name: RichToolBar::EnsureIsEnabled
// Created: ABR 2012-05-11
// -----------------------------------------------------------------------------
void RichToolBar::EnsureIsEnabled()
{
    setEnabled( true );
    QAction* action = toggleViewAction();
    action->setEnabled( true );
    action->setCheckable( true );
    action->setChecked( isVisible() );
}

// -----------------------------------------------------------------------------
// Name: RichToolBar::IsVisible
// Created: ABR 2012-05-15
// -----------------------------------------------------------------------------
bool RichToolBar::IsVisible() const
{
    return isVisible();
}

// -----------------------------------------------------------------------------
// Name: RichToolBar::SetMenuVisibility
// Created: ABR 2013-02-18
// -----------------------------------------------------------------------------
void RichToolBar::SetMenuVisibility( bool windowMenuVisibility )
{
    windowMenuVisibility_ = windowMenuVisibility;
}

// -----------------------------------------------------------------------------
// Name: RichToolBar::NotifyModeChanged
// Created: ABR 2013-02-18
// -----------------------------------------------------------------------------
void RichToolBar::NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode )
{
    kernel::DisplayableModesObserver_ABC::NotifyModeChanged( newMode, useDefault, firstChangeToSavedMode );
    QAction* action = toggleViewAction();
    assert( action != 0 );
    action->setVisible( windowMenuVisibility_ && !( newMode & GetHiddenModes() || newMode & GetVisibleModes() ) );
}

// -----------------------------------------------------------------------------
// Name: RichToolBar::SetReadOnly
// Created: NPT 2013-02-27
// -----------------------------------------------------------------------------
void RichToolBar::SetReadOnly( bool readOnly )
{
    setEnabled( readOnly );
}
