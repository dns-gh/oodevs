// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "SessionTray.h"

#include "TrayIcon.h" 
#include "TrayMenu.h" 
#include "Session.h" 
#include "SessionStatus.h" 
#include "clients_kernel/Controllers.h" 
#include "clients_kernel/Controller.h" 

// -----------------------------------------------------------------------------
// Name: SessionTray constructor
// Created: RDS 2008-09-26
// -----------------------------------------------------------------------------
SessionTray::SessionTray( QWidget *parent, kernel::Controllers& controllers )
    : controllers_ ( controllers ) 
    , trayMenu_ ( *new TrayMenu( parent ) )
    , trayIcon_ ( *new TrayIcon( QPixmap( "resources/images/selftraining/csword.png" ), "Show window", &trayMenu_ ) ) 
{
    controllers_.controller_.Register( *this ) ; 
    trayIcon_.show(); 
    QObject::connect(&trayIcon_, SIGNAL( clicked( const QPoint&) ), parent, SLOT( Maximize() ) );
}

// -----------------------------------------------------------------------------
// Name: SessionTray destructor
// Created: RDS 2008-09-26
// -----------------------------------------------------------------------------
SessionTray::~SessionTray()
{
    trayIcon_.hide(); 
}

// -----------------------------------------------------------------------------
// Name: SessionTray::NotifyUpdated
// Created: RDS 2008-09-26
// -----------------------------------------------------------------------------
void SessionTray::NotifyUpdated( const SessionStatus& status )
{
    switch( status.GetStatus() ) 
    {
    case SessionStatus::GUI_OPENED: 
        break ; 
    case SessionStatus::GUI_CLOSED: 
        break ; 
    }
}

