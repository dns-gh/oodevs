// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LogisticMagicInterface.h"
#include "moc_LogisticMagicInterface.cpp"

#include "actions/ActionsModel.h"
#include "clients_gui/LogisticBase.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface constructor
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
LogisticMagicInterface::LogisticMagicInterface( QObject* parent, kernel::Controllers& controllers, actions::ActionsModel& actionsModel )
    : QObject( parent )
    , controllers_( controllers )
    , actionsModel_( actionsModel )
    , selectedEntity_( controllers.controller_ )
{
    controllers_.Register( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface destructor
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
LogisticMagicInterface::~LogisticMagicInterface()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::NotifyContextMenu
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::NotifyContextMenu( const kernel::Automat_ABC& automat, kernel::ContextMenu& menu )
{
    if( !automat.Get< gui::LogisticBase >().IsBase() )
        return;
    selectedEntity_ = &automat;
    AddSetMaintenanceManual( menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::NotifyContextMenu
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::NotifyContextMenu( const kernel::Formation_ABC& formation, kernel::ContextMenu& menu )
{
    if( !formation.Get< gui::LogisticBase >().IsBase() )
        return;
    selectedEntity_ = &formation;
    AddSetMaintenanceManual( menu );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::AddSetMaintenanceManual
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::AddSetMaintenanceManual( kernel::ContextMenu& menu )
{
    if( !selectedEntity_ )
        return;
    if( selectedEntity_->Get< gui::LogisticBase >().IsMaintenanceManual() )
        menu.InsertItem( "Command", tr( "Switch to automated maintenance" ), this, SLOT( SwitchMaintenance() ) );
    else
        menu.InsertItem( "Command", tr( "Switch to manual maintenance" ), this, SLOT( SwitchMaintenance() ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticMagicInterface::SwitchMaintenance
// Created: ABR 2014-01-21
// -----------------------------------------------------------------------------
void LogisticMagicInterface::SwitchMaintenance()
{
    if( !selectedEntity_ )
        return;
    actionsModel_.PublishLogMaintenanceSetManualAction( *selectedEntity_,
                                                        !selectedEntity_->Get< gui::LogisticBase >().IsMaintenanceManual() );
}
