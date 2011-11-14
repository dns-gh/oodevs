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
#include "clients_gui/Tools.h"
#include "tools/GeneralConfig.h"

// -----------------------------------------------------------------------------
// Name: SessionTray constructor
// Created: RDS 2008-09-26
// -----------------------------------------------------------------------------
SessionTray::SessionTray( QWidget *parent )
    : trayMenu_ ( *new TrayMenu( parent ) )
    , trayIcon_ ( *new TrayIcon( QPixmap( tools::GeneralConfig::BuildResourceChildFile( "images/gui/logo32x32.png" ).c_str() ), "", &trayMenu_ ) )
{
    trayIcon_.show();
    QObject::connect( &trayIcon_, SIGNAL( clicked( const QPoint& ) ), parent, SLOT( Maximize() ) );
}

// -----------------------------------------------------------------------------
// Name: SessionTray destructor
// Created: RDS 2008-09-26
// -----------------------------------------------------------------------------
SessionTray::~SessionTray()
{
    trayIcon_.hide();
    delete &trayMenu_;
    delete &trayIcon_;
}

// -----------------------------------------------------------------------------
// Name: SessionTray::OnLanguageChanged
// Created: ABR 2011-11-09
// -----------------------------------------------------------------------------
void SessionTray::OnLanguageChanged()
{
    trayIcon_.setToolTip( tools::translate( "SessionTray", "Show window" ) );
}


