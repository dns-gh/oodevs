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
#include "clients_kernel/Tools.h"
#include "clients_gui/GLView_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LockMapViewController constructor
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
LockMapViewController::LockMapViewController( Controllers& controllers, gui::GLView_ABC& view )
    : controllers_( controllers )
    , view_( view )
    , locked_( controllers )
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
// Name: LockMapViewController Clear
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::Clear()
{
    locked_ = 0;
    selected_ = 0;
    center_ = geometry::Point2f();
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::LockViewOnEntity
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::LockViewOnEntity()
{
    locked_ = selected_;
    view_.CenterOn( locked_->Get< kernel::Positions >().GetPosition() );
    center_ = view_.GetCenter();
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::UnlockViewOnEntity
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::UnlockViewOnEntity()
{
    Clear();
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::NotifyUpdated
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::NotifyUpdated( const Simulation::sEndTick& /*endTick*/ )
{
    if( !locked_ )
        return;
    if( view_.GetCenter().SquareDistance( center_ ) < 1.f )
    {
        view_.CenterOn( locked_->Get< kernel::Positions >().GetPosition() );
        center_ = view_.GetCenter();
        return;
    }
    Clear();
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::UpdateContextMenu
// Created: MMC 2013-09-09
// -----------------------------------------------------------------------------
void LockMapViewController::UpdateContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    selected_ = &entity;
    if( locked_ && locked_ == selected_ )
        menu.InsertItem( "Interface", tools::translate( "LockMapViewController", "Unlock view on entity" ), this, SLOT( UnlockViewOnEntity() ) );
    else
        menu.InsertItem( "Interface", tools::translate( "LockMapViewController", "Lock view on entity" ), this, SLOT( LockViewOnEntity() ) );
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
