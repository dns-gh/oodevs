// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "InfoDock.h"
#include "InfoWidget.h"
#include "clients_kernel/Controllers.h"
#include "gaming/Tools.h"

// -----------------------------------------------------------------------------
// Name: InfoDock constructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoDock::InfoDock( QWidget* parent, kernel::Controllers& controllers, const kernel::Profile_ABC& profile, gui::EntitySymbols& icons,
                    gui::ItemFactory_ABC& itemFactory, const StaticModel& staticModel, actions::ActionsModel& actionsModel, const kernel::Time_ABC& simulation )
    : QDockWidget( "info", parent )
    , controllers_( controllers )
{
    setObjectName( "infoDock" );
    setWidget( new InfoWidget( this, controllers_, profile, icons, itemFactory, staticModel, actionsModel, simulation ) );
    setCaption( tools::translate( "InfoDock", "Info" ) );
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InfoDock destructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoDock::~InfoDock()
{
    controllers_.Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoDock::NotifySelected
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
void InfoDock::NotifySelected( const kernel::Entity_ABC* /*entity*/ )
{
//    setShown( entity ); // $$$$ SBO 2007-02-08: à voir
}
