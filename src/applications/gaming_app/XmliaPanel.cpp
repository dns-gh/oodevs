// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "XmliaPanel.h"
#include "XmliaWidget.h"

// -----------------------------------------------------------------------------
// Name: XmliaPanel constructor
// Created: RPD 2008-06-11
// -----------------------------------------------------------------------------
XmliaPanel::XmliaPanel( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, CommandHandler& handler )
    : QDockWindow( parent, "XMLIA" )
{
    setWidget( new XmliaWidget( this, controllers, publisher, handler ) );
    setResizeEnabled( true );
    setSizePolicy( QSizePolicy::Maximum, QSizePolicy::Maximum );
    setCloseMode( QDockWindow::Always );
    setCaption( tr( "XMLIA messages" ) );
    hide();
}

// -----------------------------------------------------------------------------
// Name: XmliaPanel destructor
// Created: RPD 2008-06-11
// -----------------------------------------------------------------------------
XmliaPanel::~XmliaPanel()
{
    // NOTHING
}
