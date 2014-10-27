// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LockMapViewController.h"
#include "moc_LockMapViewController.cpp"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentKnowledge_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Positions.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LockMapViewController constructor
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
LockMapViewController::LockMapViewController( Controllers& controllers,
                                              gui::GLView_ABC& view )
    : controllers_( controllers )
    , view_( view )
    , selected_( controllers )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController destructor
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
LockMapViewController::~LockMapViewController()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::LockViewOnEntity
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::OnLockViewOnEntity()
{
    LockViewOnEntity( selected_ );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::UnlockViewOnEntity
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::OnUnlockViewOnEntity()
{
    LockViewOnEntity( 0 );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::LockViewOnEntity
// Created: ABR 2014-07-25
// -----------------------------------------------------------------------------
void LockMapViewController::LockViewOnEntity( const kernel::Entity_ABC* entity )
{
    selected_ = 0;
    view_.GetOptions().SetLockedEntity( entity );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::UpdateContextMenu
// Created: MMC 2013-09-09
// -----------------------------------------------------------------------------
void LockMapViewController::UpdateContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = &entity;
    if( view_.GetOptions().GetLockedEntity() == &entity )
        menu.InsertItem( "Interface", tr( "Unlock view on entity" ), this, SLOT( OnUnlockViewOnEntity() ) );
    else
        menu.InsertItem( "Interface", tr( "Lock view on entity" ), this, SLOT( OnLockViewOnEntity() ) );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController:: NotifyContextMenu( const kernel::AgentKnowledge_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu )
{
    UpdateContextMenu( entity, menu );
}
