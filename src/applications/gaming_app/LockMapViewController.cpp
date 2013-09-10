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
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/AgentExtensions.h"
#include "clients_kernel/ContextMenu.h"
#include "clients_kernel/Positions.h"
#include "clients_gui/Tools.h"
#include "clients_gui/View_ABC.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LockMapViewController constructor
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
LockMapViewController::LockMapViewController( Controllers& controllers, gui::View_ABC& view )
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
// Name: LockMapViewController::LockViewOnAgent
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::LockViewOnAgent()
{
    locked_ = selected_;
    view_.CenterOn( locked_->Get< kernel::Positions >().GetPosition() );
    center_ = view_.GetCenter();
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::UnlockViewOnAgent
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::UnlockViewOnAgent()
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
// Name: LockMapViewController::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::NotifyContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu )
{
    if( const kernel::Agent_ABC* pAgent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        NotifyContextMenu( *pAgent, menu );
    else if ( const kernel::Automat_ABC* pAutomat = dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        NotifyContextMenu( *pAutomat, menu );
    else if ( const kernel::Formation_ABC* pFormation = dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        NotifyContextMenu( *pFormation, menu );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::UpdateContextMenu
// Created: MMC 2013-09-09
// -----------------------------------------------------------------------------
void LockMapViewController::UpdateContextMenu( kernel::ContextMenu& menu )
{
    if( locked_ && locked_ == selected_ )
        menu.InsertItem( "Interface", tools::translate( "LockMapViewController", "Unlock view on the unit" ), this, SLOT( UnlockViewOnAgent() ) );
    else
        menu.InsertItem( "Interface", tools::translate( "LockMapViewController", "Lock view on the unit" ), this, SLOT( LockViewOnAgent() ) );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::NotifyContextMenu( const kernel::Agent_ABC& agent, kernel::ContextMenu& menu )
{
    selected_ = &agent;
    UpdateContextMenu( menu );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    selected_ = &automat;
    UpdateContextMenu( menu );
}

// -----------------------------------------------------------------------------
// Name: LockMapViewController::NotifyContextMenu
// Created: MMC 2013-09-05
// -----------------------------------------------------------------------------
void LockMapViewController::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    selected_ = &formation;
    UpdateContextMenu( menu );
}
