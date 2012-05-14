// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "RichMenu.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController_ABC.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichMenu constructor
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
RichMenu::RichMenu( QWidget* parent, kernel::Controllers& controllers, const QString& title /*= ""*/ )
    : kernel::ContextMenu( parent )
    , controllers_( controllers )
{
    if( !title.isEmpty() )
        setTitle( title );
    if( controllers_.modes_ )
        controllers_.modes_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: RichMenu destructor
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
RichMenu::~RichMenu()
{
    if( controllers_.modes_ )
        controllers_.modes_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: RichMenu::SetVisible
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void RichMenu::SetVisible( bool visible )
{
    if( QAction* action = menuAction() )
        action->setVisible( visible );
}

// -----------------------------------------------------------------------------
// Name: RichMenu::ForceEnabled
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void RichMenu::ForceEnabled( bool enabled )
{
    if( QAction* action = menuAction() )
        action->setEnabled( enabled );
}

// -----------------------------------------------------------------------------
// Name: RichMenu::EnsureIsEnabled
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
void RichMenu::EnsureIsEnabled()
{
    if( QAction* action = menuAction() )
        action->setEnabled( true );
}

