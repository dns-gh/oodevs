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

// -----------------------------------------------------------------------------
// Name: InfoDock constructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoDock::InfoDock( QWidget* parent, kernel::Controllers& controllers, gui::SymbolIcons& icons, gui::ItemFactory_ABC& itemFactory )
    : QDockWindow( parent )
    , controllers_( controllers )
{
    setOrientation( Qt::Horizontal );
    setWidget( new InfoWidget( this, controllers_, icons, itemFactory ) );
    setResizeEnabled( true );
    setCloseMode( QDockWindow::Always );
    setCaption( tr( "Info" ) );
    controllers_.Register( *this );
    hide();
}

// -----------------------------------------------------------------------------
// Name: InfoDock destructor
// Created: SBO 2007-02-02
// -----------------------------------------------------------------------------
InfoDock::~InfoDock()
{
    controllers_.Remove( *this );
}

// -----------------------------------------------------------------------------
// Name: InfoDock::NotifySelected
// Created: SBO 2007-02-05
// -----------------------------------------------------------------------------
void InfoDock::NotifySelected( const kernel::Entity_ABC* entity )
{
//    setShown( entity ); // $$$$ SBO 2007-02-08: à voir
}
