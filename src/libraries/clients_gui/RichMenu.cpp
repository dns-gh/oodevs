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
#include "ObjectNameManager.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/ModeController.h"

using namespace gui;

// -----------------------------------------------------------------------------
// Name: RichMenu constructor
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
RichMenu::RichMenu( const QString& objectName, QWidget* parent, kernel::Controllers& controllers, const QString& title /*= ""*/ )
    : kernel::ContextMenu( parent )
    , modeController_( controllers.modes_ )
{
    if( !title.isEmpty() )
        setTitle( title );
    modeController_.Register( *this );
    ObjectNameManager::getInstance()->SetObjectName( this, objectName );
}

// -----------------------------------------------------------------------------
// Name: RichMenu destructor
// Created: ABR 2012-05-14
// -----------------------------------------------------------------------------
RichMenu::~RichMenu()
{
    modeController_.Unregister( *this );
    ObjectNameManager::getInstance()->RemoveRegisteredName( objectName() );
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

// -----------------------------------------------------------------------------
// Name: RichMenu::IsVisible
// Created: ABR 2012-05-16
// -----------------------------------------------------------------------------
bool RichMenu::IsVisible() const
{
    if( QAction* action = menuAction() )
        return action->isVisible();
    return false;
}
