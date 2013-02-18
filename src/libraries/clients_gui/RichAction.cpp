// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichAction.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController.h"
#pragma warning( push, 0 )
#include <QtGui/qaction.h>
#pragma warning( pop )

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichAction constructor
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
RichAction::RichAction( QAction* action, kernel::Controllers& controllers )
    : action_( action )
    , modeController_( controllers.modes_ )
{
    modeController_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RichAction destructor
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
RichAction::~RichAction()
{
    modeController_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RichAction::SetVisible
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void RichAction::SetVisible( bool visible )
{
    action_->setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: RichAction::ForceEnabled
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void RichAction::ForceEnabled( bool enabled )
{
    action_->setEnabled( enabled );
}

// -----------------------------------------------------------------------------
// Name: RichAction::EnsureIsEnabled
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void RichAction::EnsureIsEnabled()
{
    action_->setEnabled( true );
}

// -----------------------------------------------------------------------------
// Name: RichAction::IsVisible
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
bool RichAction::IsVisible() const
{
    return action_->isVisible();
}
